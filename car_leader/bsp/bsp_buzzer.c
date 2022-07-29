/*
 * bsp_buzzer.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "bsp_buzzer.h"

void bsp_buzzer_init(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);

    bsp_buzzer_set(0);
}


void bsp_buzzer_set(bool state){
    if(state){
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xff);
    }else{
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
    }
}
