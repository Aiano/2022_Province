/*
 * follow_line.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "follow_line.h"
#include "bsp_key.h"
#include "main.h"

// switch task
uint8_t now_task = 1;
uint8_t task_num = 4;

// common use
int16_t target_value = 120;
int16_t delta_value = 0;
uint32_t ultrasonic_value;

// task1
int16_t task1_target_distance = 320; // 20 cm
// 3200 0.3m/s
int16_t task1_avg_speed = 3200; // 7000 0.5
int16_t task1_temp_avg_speed;
int16_t task1_Kp = 12;
int16_t task1_u = 0;


void task1(int16_t now_value){
    ultrasonic_value = bsp_adc_read();
//    ultrasonic_value = 320;
    if(ultrasonic_value > 400) ultrasonic_value = 400;

//    UARTSend((uint8_t *)"hello\n", 16);
    if(!running_state || ultrasonic_value < 100){ // 420 -> 20cm
        bsp_can_set_speed(0, 0);
    }else{
        task1_temp_avg_speed = ultrasonic_value * task1_avg_speed / task1_target_distance;
        if(task1_temp_avg_speed > 3400) task1_temp_avg_speed = 3400; // avg_speed limit

        delta_value = target_value - now_value;
        task1_u = task1_Kp * delta_value;
        if(task1_u > 1000) task1_u = 1000; // rotate limit
        bsp_can_set_speed(task1_temp_avg_speed + task1_u, task1_temp_avg_speed - task1_u);
    }
}

// task2
int16_t task2_target_distance = 320; // 20 cm
// 3200 0.3m/s
int16_t task2_avg_speed = 5300; // 7000 0.5
int16_t task2_base_speed = 2000;
int16_t task2_temp_avg_speed;
int16_t task2_Kp = 13;
int16_t task2_u = 0;
bool task2_track = 0;

void task2(int16_t now_value){
    ultrasonic_value = bsp_adc_read();
    if(!task2_track && ultrasonic_value > 400){
        ultrasonic_value = 400; // ultrasonic distance limit
    }else if(task2_track && ultrasonic_value > task2_target_distance){
        ultrasonic_value = task2_target_distance;
    }

    // track
    if(ultrasonic_value < task2_target_distance) task2_track = 1;

    if(!running_state || ultrasonic_value < 80){
        bsp_can_set_speed(0, 0);
    }else{
        task2_temp_avg_speed = ultrasonic_value * (task2_avg_speed - task2_base_speed) / task2_target_distance + task2_base_speed;
        if(task2_temp_avg_speed > 5500) task1_temp_avg_speed = 5500; // avg_speed limit

        delta_value = target_value - now_value;
        task2_u = task2_Kp * delta_value;
        if(task2_u > 4000) task2_u = 4000; // rotate limit
        bsp_can_set_speed(task2_temp_avg_speed + task2_u, task2_temp_avg_speed - task2_u);
    }
}

void task3(){

}

void task4(){

}
