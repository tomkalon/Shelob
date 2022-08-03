/*
 * stepper.h
 *
 *  Created on: 23 lip 2022
 *      Author: Tomasz Kaliński
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tim.h"

#define STEPPER_TIM_CLK							16000000
#define STEPPER_TIM_PSC							99
#define STEPPER_GUILD_SCREW_THREAD_PITCH 		200 		// 1 -> 0,01mm; 1mm -> 100 // screw pitch (stepper guild)
#define STEPPER_GUILD_STEPS_PER_REV 			200			// guild steps per revolution
#define STEPPER_GUILD_PWM_PULSE 				50			// stepper guild / pwm pulse value / in %
#define STEPPER_ROTATOR_STEPS_PER_REV 			200			// rotator steps per revolution
#define STEPPER_ROTATOR_PWM_PULSE 				50			// stepper rotator / pwm pulse value / in %

// MENU/SETTINGS/STEPS LABELS
typedef enum {
	STEPPER_DIRECTION_CW	= 0,
	STEPPER_DIRECTION_CCW 	= 1
} STEPPER_DIRECTION;

typedef enum {
	STEPPER_TURN_ON		= 0,
	STEPPER_TURN_OFF 	= 1
} STEPPER_ENABLE;

typedef enum {
	STEPPER_SPEED_SLOWEST	= 1599,
	STEPPER_SPEED_SLOW		= 999,
	STEPPER_SPEED_NORMAL	= 799,
	STEPPER_SPEED_FAST		= 499,
	STEPPER_SPEED_FASTEST	= 399
} STEPPER_SPEED;

uint8_t Steppers_Control_Init(uint16_t width, uint16_t turns, uint16_t diameter, uint16_t speed);

#endif /* INC_STEPPER_H_ */
