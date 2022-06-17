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
#include <stdio.h>
#include <string.h>
#include "usart.h"

// UART
void printUART(unsigned char str[]);
void printUART_INT(uint16_t integer);

#endif /* INC_MISC_H_ */
