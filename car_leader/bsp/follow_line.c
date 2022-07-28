/*
 * follow_line.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "follow_line.h"
#include "bsp_key.h"
#include "main.h"

int16_t target_value = 120;
int16_t delta_value = 0;
int16_t avg_speed = 6000; // 7000 0.5
int16_t Kp = 15;
int16_t u = 0;
uint32_t ultrasonic_value;

void follow_line(int16_t now_value){
    ultrasonic_value = bsp_adc_read();
//    ultrasonic_value = 200;

//    UARTSend((uint8_t *)"hello\n", 16);
    if(!bsp_key_flag || ultrasonic_value < 420){ // 420 -> 20cm
        bsp_can_set_speed(0, 0);
    }else{
        delta_value = target_value - now_value;
        u = Kp * delta_value;
        bsp_can_set_speed(avg_speed + u, avg_speed - u);
    }
}
