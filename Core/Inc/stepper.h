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

// MENU/SETTINGS/STEPS LABELS
typedef enum {
	STEPPER_DIRECTION_CW	= 0,
	STEPPER_DIRECTION_CCW 	= 1
} STEPPER_DIRECTION;

typedef enum {
	STEPPER_TURN_ON		= 0,
	STEPPER_TURN_OFF 	= 1
} STEPPER_ENABLE;

void Steppers_Control_Init(uint16_t width, uint16_t turns, uint16_t diameter, uint16_t speed);


#endif /* INC_STEPPER_H_ */
