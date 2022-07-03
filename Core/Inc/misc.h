/*
 * misc.h
 *
 *  Created on: May 4, 2022
 *      Author: Tomasz Kaliński
 */

#ifndef INC_MISC_H_
#define INC_MISC_H_

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

// UART
void printUART(unsigned char str[]);
void printUART_INT(uint16_t integer);

#endif /* INC_MISC_H_ */
