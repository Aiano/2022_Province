/*
 * follow_line.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "follow_line.h"
#include "main.h"

int16_t target_value = 120;
int16_t delta_value = 0;
int16_t avg_speed = 1000;
int16_t Kp = 10;
int16_t u = 0;
uint32_t ultrasonic_value = 0;

void follow_line(int16_t now_value){
    ultrasonic_value = bsp_adc_read();

    UARTSend((uint8_t *)"hello\n", 16);
    if(ultrasonic_value < 100){
        bsp_can_set_speed(0, 0);
    }else{
        delta_value = target_value - now_value;
        u = Kp * delta_value;
        bsp_can_set_speed(avg_speed + u, avg_speed - u);
    }
}
