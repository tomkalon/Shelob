/*
 * misc.c
 *
 *  Created on: May 4, 2022
 *      Author: Tomasz Kaliński
 */

#include "misc.h"

// UART
void printUART(unsigned char * str)
{
	uint8_t length = strlen(str);
	HAL_UART_Transmit(&huart2, str, length, 10);
}
