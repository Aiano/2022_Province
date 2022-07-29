/*
 * follow_line.h
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */

#ifndef BSP_FOLLOW_LINE_H_
#define BSP_FOLLOW_LINE_H_

#include "bsp_can.h"
#include "bsp_adc.h"
//#include "utils/uartstdio.h"
#include "uart_parser.h"

extern uint8_t now_task;
extern uint8_t task_num;

void task1(int16_t now_value);

#endif /* BSP_FOLLOW_LINE_H_ */
