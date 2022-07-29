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
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "main.h"

extern int16_t uart1_received_value;
extern uint8_t uart1_received_command;

void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UARTInit();

#endif /* BSP_UART_PARSER_H_ */
