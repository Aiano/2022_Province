/*
 * follow_line.c
 *
 *  Created on: 2022年7月28日
 *      Author: Aiano
 */


#include "follow_line.h"
#include "bsp_key.h"
#include "bsp_buzzer.h"
#include "bsp_led.h"
#include "main.h"

// switch task
uint8_t now_task = 1;
uint8_t task_num = 4;

// common use
int16_t target_value = 120;
int16_t delta_value = 0;
uint32_t ultrasonic_value;

// 3200 -> 0.3m/s
int16_t task1_avg_speed = 3200;
int16_t task1_Kp = 12;
int16_t task1_u = 0;


void task1(int16_t now_value){
    ultrasonic_value = bsp_adc_read();

    if(!running_state || ultrasonic_value < 420){ // 领头车防撞
        bsp_can_set_speed(0, 0);
    }else{
        if(uart1_received_command == 'S' || uart1_received_command == 'A'){
            bsp_buzzer_set(1);
            // bsp_led_set(BSP_LED_1, 1);
            bsp_can_set_speed(0, 0);
            running_state = 0;
            UART2Send("A1\n", 3);

        }else if(uart1_received_command == 'L'){
            delta_value = target_value - now_value;
            task1_u = task1_Kp * delta_value;
            bsp_can_set_speed(task1_avg_speed + task1_u, task1_avg_speed - task1_u);
        }
    }
}

// 5300 -> 0.5m/s
int16_t task2_avg_speed = 5300;
int16_t task2_Kp = 12;
int16_t task2_u = 0;
uint8_t task2_circle = 0;
uint8_t task2_last_received_command = 'L';

void task2(int16_t now_value){
    ultrasonic_value = bsp_adc_read();

    if(!running_state || ultrasonic_value < 420){ // 领头车防撞
        bsp_can_set_speed(0, 0);
    }else{
        if(uart1_received_command == 'A'){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0xff);
        }
        if(task2_last_received_command == 'L' && (uart1_received_command == 'S' || uart1_received_command == 'A')){

            task2_circle++;
            if(task2_circle == 2){
                bsp_buzzer_set(1);
                bsp_can_set_speed(0, 0);
                running_state = 0;
                UART2Send("A1\n", 3);
            }

        }else if(uart1_received_command == 'L'){
            delta_value = target_value - now_value;
            task2_u = task2_Kp * delta_value;
            bsp_can_set_speed(task2_avg_speed + task2_u, task2_avg_speed - task2_u);
        }
        task2_last_received_command = uart1_received_command;
    }
}

// 5300 -> 0.5m/s
int16_t task3_avg_speed = 3200;
int16_t task3_inner_acc_speed = 5300;
int16_t task3_Kp = 12;
int16_t task3_u = 0;
uint8_t task3_circle = 0;
uint8_t task3_last_received_command = 'L';
uint16_t task3_inner_circle_u = 800;
uint16_t task3_is_inner = 0;

void task3(int16_t now_value){
    ultrasonic_value = bsp_adc_read();

    if(!running_state || ultrasonic_value < 420){ // 领头车防撞
        bsp_can_set_speed(0, 0);
    }else{
        if(task3_last_received_command == 'L' && (uart1_received_command == 'S' || uart1_received_command == 'A')){

            task3_circle++;
            if(task3_circle >= 3){
                bsp_buzzer_set(1);
                bsp_can_set_speed(0, 0);
                running_state = 0;
                UART2Send("A1\n", 3);
            }

        }else if(uart1_received_command == 'L'){
            if(task3_last_received_command == 'X' && task3_circle == 2){
                task3_is_inner++;
            }
            delta_value = target_value - now_value;
            task3_u = task3_Kp * delta_value;

            if(task3_is_inner == 1){
                bsp_can_set_speed(task3_inner_acc_speed + task3_u, task3_inner_acc_speed - task3_u);
            }else{
                bsp_can_set_speed(task3_avg_speed + task3_u, task3_avg_speed - task3_u);
            }
        }else if(uart1_received_command == 'X'){ // 岔路口
            if(task3_circle >= 2){
                bsp_can_set_speed(task3_inner_acc_speed + task3_inner_circle_u, task3_inner_acc_speed - task3_inner_circle_u);
            }else{
                bsp_can_set_speed(task3_avg_speed, task3_avg_speed);
            }
        }
        task3_last_received_command = uart1_received_command;
    }
}

void task4(){

}
