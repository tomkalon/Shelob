/*
 * stepper.c
 *
 *  Created on: 23 lip 2022
 *      Author: Tomasz Kaliński
 */

#include <stepper.h>

volatile uint16_t stepperGuidePosition = 0, stepperRotatorPosition = 0, stepperNmberOfTurns = 0, CompleteProgress = 0;
volatile uint16_t displayRefreshCounter = 0;	// INT counter from TIMER_STEPPER_ROTATOR


void Steppers_Control_Init(uint16_t width, uint16_t turns, uint16_t diameter, uint16_t speed)
{

	uint16_t timerStepper_clk 				= 0;
	uint8_t guideShiftPerStep 				= STEPPER_GUILD_SCREW_THREAD_PITCH / STEPPER_GUILD_STEPS_PER_REV; 	// e.g. 2mm & 200SPR -> 200 / 200 = 1 (0,01mm precision)
	uint16_t guideStepsForDiameter			= diameter / guideShiftPerStep;										// e.g. for winding diameter 0,5mm -> 50 / 1 = 50 steps
	uint16_t isDiameterIndivisible			= diameter % guideShiftPerStep;										// e.g. for diameter 0,11 is FALSE, for 0,527 is TRUE (too little precision)
	uint16_t guideShiftPerRotatorRevolution = STEPPER_ROTATOR_STEPS_PER_REV / guideStepsForDiameter;			// e.g. value of gears ratio -> 200(SPR) / 50(STEPS to get DIAMETER) = 4;
	uint16_t isGearsRatioInaccurate			= STEPPER_ROTATOR_STEPS_PER_REV % guideStepsForDiameter;			// e.g. for 50 steps is FALSE, for 70 steps is TRUE (necessary corrections)

	uint16_t rotator_arr					= 799;									// stepper rotator auto-reload register value
	uint16_t guide_arr						= guideShiftPerRotatorRevolution - 1;	// stepper guide auto-reload register value

	if(isDiameterIndivisible)
	{

	}

	if(isGearsRatioInaccurate)
	{

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
	__HAL_TIM_SET_AUTORELOAD(&TIMER_STEPPER_GUIDE, guide_arr);
}
