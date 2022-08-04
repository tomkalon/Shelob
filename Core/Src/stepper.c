/*
 * stepper.c
 *
 *  Created on: 23 lip 2022
 *      Author: Tomasz Kaliński
 */

#include <stepper.h>

volatile uint16_t stepperGuidePosition = 0, stepperRotatorPosition = 0, stepperNmberOfTurns = 0, CompleteProgress = 0;

volatile uint16_t displayRefreshCounter 			= 0;	// INT counter from TIMER_STEPPER_ROTATOR

volatile uint16_t guideShiftPerRotatorRev			= 0;	// NORMAL MODE: interval time
volatile uint8_t adjustedIntervalCount 				= 0;	// ADJUSTED MODE: standard interval time
volatile uint16_t adjustedIntervalCountIncreased	= 0;	// ADJUSTED MODE: increased interval time
volatile uint8_t adjustedStandardCountDifference	= 0;	// ADJUSTED MODE: difference between occurrence of standard and increased interval times
volatile bool indivisibleByTwo						= 0;	// ADJUSTED MODE: that difference is indivisible by 2
volatile bool moreStandardInterval					= 0;	// ADJUSTED MODE: if 1, more standard than increased intervals
volatile uint8_t alternatelyStart					= 0;	// ADJUSTED MODE: after that, the interval is changed
uint8_t intervalAdjustingMode						= 0;	// if 0, there is no necessity to make interval correction

volatile uint16_t rotator_arr						= 799;	// stepper rotator auto-reload register value
volatile uint16_t guide_arr							= 0;	// stepper guide auto-reload register value

volatile uint16_t timerStepper_clk 					= 0;	// stepper rotator timer work frequency

uint8_t Steppers_Control_Init(uint16_t width, uint16_t turns, uint16_t diameter, uint16_t speed)
{
	uint8_t guideShiftPerStep 				= STEPPER_GUILD_SCREW_THREAD_PITCH / STEPPER_GUILD_STEPS_PER_REV; 	// e.g. 2mm & 200SPR -> 200 / 200 = 1 (0,01mm precision)
	uint16_t guideStepsForDiameter			= diameter / guideShiftPerStep;										// e.g. for winding diameter 0,5mm -> 50 / 1 = 50 steps
	guideShiftPerRotatorRev 				= STEPPER_ROTATOR_STEPS_PER_REV / guideStepsForDiameter;			// e.g. value of gears ratio -> 200(SPR) / 50(STEPS to get DIAMETER) = 4;
	adjustedIntervalCountIncreased			= STEPPER_ROTATOR_STEPS_PER_REV % guideStepsForDiameter;			// e.g. for 50 steps is FALSE, for 70 steps is TRUE (necessary corrections)

	guide_arr								= guideShiftPerRotatorRev - 1;

	if (adjustedIntervalCountIncreased)
	{
		adjustedIntervalCount = guideStepsForDiameter - adjustedIntervalCountIncreased;
		adjustedStandardCountDifference = abs(adjustedIntervalCount- adjustedIntervalCountIncreased);
		if (!adjustedStandardCountDifference)
		{
			intervalAdjustingMode = 1;
		}
		else if (adjustedStandardCountDifference == 1)
		{
			intervalAdjustingMode = 2;
			if (adjustedIntervalCount > adjustedIntervalCountIncreased) { moreStandardInterval = 1; }
		}
		else
		{
			intervalAdjustingMode = 3;
			if (adjustedIntervalCount > adjustedIntervalCountIncreased) { moreStandardInterval = 1; }
			alternatelyStart = adjustedStandardCountDifference / 2;
			if (adjustedStandardCountDifference % 2) { moreStandardInterval = 1; }
		}
	}
	else
	{
		__HAL_TIM_SET_AUTORELOAD(&TIMER_STEPPER_GUIDE, guide_arr);
	}

	switch (speed)
	{
		case 1:
			rotator_arr = STEPPER_SPEED_SLOWEST;
			break;
		case 2:
			rotator_arr = STEPPER_SPEED_SLOW;
			break;
		case 3:
			rotator_arr = STEPPER_SPEED_NORMAL;
			break;
		case 4:
			rotator_arr = STEPPER_SPEED_FAST;
			break;
		case 5:
			rotator_arr = STEPPER_SPEED_FASTEST;
			break;
	}

	timerStepper_clk = STEPPER_TIM_CLK / ((STEPPER_TIM_PSC + 1) / (rotator_arr + 1));
	__HAL_TIM_SET_AUTORELOAD(&TIMER_STEPPER_ROTATOR, rotator_arr);

	return intervalAdjustingMode;
}

void Steppers_Run(uint8_t mode)
{
	if (!mode)
	{
		HAL_TIM_Base_Start_IT(&htim22);
		HAL_TIM_PWM_Start_IT(&htim21, TIM_CHANNEL_1);
	}

}
