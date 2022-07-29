/*
 * follow_line.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "follow_line.h"
#include "bsp_key.h"
#include "bsp_buzzer.h"
#include "bsp_led.h"
#include "main.h"

uint8_t now_task = 1;
uint8_t task_num = 4;

int16_t target_value = 120;
int16_t delta_value = 0;

// 3200 -> 0.3m/s

int16_t task1_avg_speed = 3200; // 7000 0.5


int16_t Kp = 12;
int16_t u = 0;
uint32_t ultrasonic_value;

void task1(int16_t now_value){
    ultrasonic_value = bsp_adc_read();

    if(!running_state || ultrasonic_value < 420){ // 420 -> 20cm
        bsp_can_set_speed(0, 0);
    }else{
        if(uart1_received_command == 'S' || uart1_received_command == 'A'){
            bsp_buzzer_set(1);
            // bsp_led_set(BSP_LED_1, 1);
            bsp_can_set_speed(0, 0);
            running_state = 0;
            UART2Send("A1\n", 3);

        }else{
            delta_value = target_value - now_value;
            u = Kp * delta_value;
            bsp_can_set_speed(task1_avg_speed + u, task1_avg_speed - u);
        }
    }
}

void task2(){

}

void task3(){

}

void task4(){

}
