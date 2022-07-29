/*
 * bsp_led.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "bsp_led.h"
#include "main.h"

void bsp_led_init(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

    bsp_led_set(BSP_LED_1, 0);
}
void bsp_led_set(BSP_LED_t led, bool state){
    uint8_t value = state?0xff:0x00;

    switch(led){
    case BSP_LED_1:
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, value);
        break;
    case BSP_LED_2:
        break;
    }
}
