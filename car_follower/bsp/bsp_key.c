/*
 * bsp_key.c
 *
 *  Created on: 2022��7��28��
 *      Author: Aiano
 */


#include "bsp_key.h"
#include "uart_parser.h"

bool bsp_key_flag = 0;

//�жϴ������
void io_interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, s);

    if((s&GPIO_PIN_4) == GPIO_PIN_4) // ȷ������Ҫ��io�������ж�
    {
        SysCtlDelay(80 * 1000 * 10);
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){ // �������
            bsp_key_flag = !bsp_key_flag;
            UART2Send("R\n", 2);
        }
//        while(!)//�ȴ������ɿ�
//            ;

    }
}


void bsp_key_init(){
    //��������
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {}

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);//����PF1Ϊ��������
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);//����PF2Ϊ���������

    //����PF4Ϊ��������ģʽ
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //�����ж�
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, io_interrupt);

    //�����ж�
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    IntEnable(INT_GPIOF);
    IntMasterEnable();
}
