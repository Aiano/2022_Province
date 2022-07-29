/*
 * bsp_key.c
 *
 *  Created on: 2022年7月28日
 *      Author: Aiano
 */


#include "bsp_key.h"
#include "uart_parser.h"

bool bsp_key_flag = 0;

//中断处理程序
void io_interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, s);

    if((s&GPIO_PIN_4) == GPIO_PIN_4) // 确认是想要的io触发的中断
    {
        SysCtlDelay(80 * 1000 * 10);
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){ // 软件消抖
            bsp_key_flag = !bsp_key_flag;
            UART2Send("R\n", 2);
        }
//        while(!)//等待按键松开
//            ;

    }
}


void bsp_key_init(){
    //开启外设
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {}

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);//设置PF1为输出，红灯
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);//设置PF2为输出，蓝灯

    //设置PF4为上拉输入模式
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //配置中断
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, io_interrupt);

    //开启中断
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    IntEnable(INT_GPIOF);
    IntMasterEnable();
}
