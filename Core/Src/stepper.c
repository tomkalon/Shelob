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
	uint16_t rotator_ARR = 799;
	uint16_t guide_ARR = 0;

	switch (speed)
	{
		case 1:
			rotator_ARR = 1599;
			break;
		case 2:
			rotator_ARR = 999;
			break;
		case 3:
			rotator_ARR = 799;
			break;
		case 4:
			rotator_ARR = 499;
			break;
		case 5:
			rotator_ARR = 399;
			break;
	}
	__HAL_TIM_SET_AUTORELOAD(&TIMER_STEPPER_ROTATOR, rotator_ARR);
	__HAL_TIM_SET_AUTORELOAD(&TIMER_STEPPER_GUIDE, guide_ARR);
}
