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
int16_t task1_target_distance = 420;
int16_t task1_avg_speed = 4500; // 7000 0.5
int16_t task1_temp_avg_speed;
int16_t Kp = 15;
int16_t u = 0;
uint32_t ultrasonic_value;

void task1(int16_t now_value){
    ultrasonic_value = bsp_adc_read();
//    ultrasonic_value = 200;

//    UARTSend((uint8_t *)"hello\n", 16);
    if(!bsp_key_flag || ultrasonic_value < 200){ // 420 -> 20cm
        bsp_can_set_speed(0, 0);
    }else{
        task1_temp_avg_speed = ultrasonic_value * task1_avg_speed / task1_target_distance;
        if(task1_temp_avg_speed > 8000) task1_temp_avg_speed = 8000;

        delta_value = target_value - now_value;
        u = Kp * delta_value;
        bsp_can_set_speed(task1_temp_avg_speed + u, task1_temp_avg_speed - u);
    }
}
