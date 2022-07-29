/*
 * bsp_key.c
 *
 *  Created on: 2022��7��28��
 *      Author: Aiano
 */


#include "bsp_key.h"
#include "uart_parser.h"
#include "follow_line.h"


//�жϴ������
void io_interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);
    GPIOIntClear(GPIO_PORTF_BASE, s);

    if((s&GPIO_PIN_4) == GPIO_PIN_4) // ȷ������Ҫ��io�������ж�
    {
        SysCtlDelay(80 * 1000 * 10);
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){ // �������
            running_state = !running_state;
            if(running_state){
                UART2Send("R1\n", 3); // Run
            }else{
                UART2Send("A1\n", 3); // Stop
            }

        }

    }

    if((s&GPIO_PIN_0) == GPIO_PIN_0){
        SysCtlDelay(80 * 1000 * 10);
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){
            now_task++;
            if(now_task > task_num) now_task = 1;
        }
    }
}


void bsp_key_init(){
    //��������
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {}

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);//����PF1Ϊ��������
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);//����PF2Ϊ���������


    //����SW1����

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

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


    //����SW2����

    // ����PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x00;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);

    //����PF0Ϊ��������ģʽ
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // �����ж�
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
//    GPIOIntRegister(GPIO_PORTF_BASE, io_interrupt2);

    // �����ж�
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);
    IntEnable(INT_GPIOF);
    IntMasterEnable();
}
