/*
 * uart_parser.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "uart_parser.h"
#include "follow_line.h"


uint8_t uart_receive_count = 0;
//uint8_t uart_receive_a_char;
uint8_t uart_receive_buffer[20];
int16_t uart_received_value;


//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UART1IntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART1_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART1_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        uart_receive_buffer[uart_receive_count ++] = ROM_UARTCharGetNonBlocking(UART1_BASE);
        if(uart_receive_buffer[uart_receive_count - 1] == 10){
            uart_received_value = atoi(uart_receive_buffer + 1);
            follow_line(uart_received_value);
            //ROM_UARTCharPutNonBlocking(UART1_BASE, (uint8_t) uart_received_value);
            uart_receive_count = 0;
        }
    }
}

//void
//UART2IntHandler(void)
//{
//    uint32_t ui32Status;
//
//    //
//    // Get the interrrupt status.
//    //
//    ui32Status = ROM_UARTIntStatus(UART2_BASE, true);
//
//    //
//    // Clear the asserted interrupts.
//    //
//    ROM_UARTIntClear(UART2_BASE, ui32Status);
//
//    //
//    // Loop while there are characters in the receive FIFO.
//    //
//    while(ROM_UARTCharsAvail(UART2_BASE))
//    {
//        //
//        // Read the next character from the UART and write it back to the UART.
//        //
//        uart_receive_buffer[uart_receive_count ++] = ROM_UARTCharGetNonBlocking(UART2_BASE);
//        if(uart_receive_buffer[uart_receive_count - 1] == 10){
//            uart_received_value = atoi(uart_receive_buffer + 1);
//            follow_line(uart_received_value);
//            ROM_UARTCharPutNonBlocking(UART1_BASE, (uint8_t) uart_received_value);
//            uart_receive_count = 0;
//        }
//    }
//}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPutNonBlocking(UART1_BASE, *pui8Buffer++);
    }
}

void UARTInit(){
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();


    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);


    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    //
    // Set GPIO B0 and B1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //
    // Set GPIO D6 and D7 as UART pins.
    //
//    GPIOPinConfigure(GPIO_PD6_U2RX);
//    GPIOPinConfigure(GPIO_PD7_U2TX);
//    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

//    ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(), 115200,
//                                    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
//                                     UART_CONFIG_PAR_NONE));
    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART1);
//    ROM_IntEnable(INT_UART2);

    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
//    ROM_UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
    //
    // Prompt for text to be entered.
    //
    UARTSend((uint8_t *)"UART initialization finished.\n", 16);


}

