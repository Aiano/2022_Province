/*
 * follow_line.h
 *
 *  Created on: 2022??7??28??
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

#define delay_ms(n); SysCtlDelay(n*(SysCtlClockGet()/3000));

void task1(int16_t now_value);
void task2(int16_t now_value);
void task3(int16_t now_value);
void task4(int16_t now_value);

#endif /* BSP_FOLLOW_LINE_H_ */
