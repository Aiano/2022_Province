/*
 * uart_parser.h
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */

#ifndef BSP_UART_PARSER_H_
#define BSP_UART_PARSER_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

extern int16_t uart_received_value;

void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UARTInit();

#endif /* BSP_UART_PARSER_H_ */
