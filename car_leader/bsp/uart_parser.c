/*
 * uart_parser.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "uart_parser.h"
#include "follow_line.h"


uint8_t uart1_receive_count = 0;
uint8_t uart1_receive_buffer[20];
uint8_t uart1_received_command;
int16_t uart1_received_value;

uint8_t uart2_receive_count = 0;
uint8_t uart2_receive_buffer[20];
uint8_t uart2_received_command;
int16_t uart2_received_value;

extern bool bsp_key_flag;


//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
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

void
UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
//
// Loop while there are more characters to send.
//
    while(ui32Count--)
    {
//
// Write the next character to the UART.
//
//ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
        ROM_UARTCharPutNonBlocking(UART2_BASE, *pui8Buffer++);
    }
}

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
        uart1_receive_buffer[uart1_receive_count ++] = ROM_UARTCharGetNonBlocking(UART1_BASE);
        if(uart1_receive_buffer[uart1_receive_count - 1] == 10){
            uart1_received_value = atoi(uart1_receive_buffer + 1);
            uart1_received_command = uart1_receive_buffer[0];

            //ROM_UARTCharPutNonBlocking(UART1_BASE, (uint8_t) uart1_received_value);
            uart1_receive_count = 0;
        }
    }
}


void
UART2IntHandler(void)
{
    uint32_t ui32Status;

// Get the interrrupt status.
    ui32Status = ROM_UARTIntStatus(UART2_BASE, true);

// Clear the asserted interrupts.
    ROM_UARTIntClear(UART2_BASE, ui32Status);

// Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART2_BASE))
    {
// Read the next character from the UART and write it back to the UART.
        uart2_receive_buffer[uart2_receive_count ++] = ROM_UARTCharGetNonBlocking(UART2_BASE);
        if(uart2_receive_buffer[uart2_receive_count - 1] == 10){
            uart2_received_value = atoi(uart2_receive_buffer + 1);
            uart2_received_command = uart2_receive_buffer[0];

            if(uart2_received_command = 'A') bsp_key_flag = !bsp_key_flag;

            //ROM_UARTCharPutNonBlocking(UART2_BASE, (uint8_t) uart2_received_value);
            uart2_receive_count = 0;
        }
    }
}




void UARTInit(){
//    //
//    // Enable lazy stacking for interrupt handlers.  This allows floating-point
//    // instructions to be used within interrupt handlers, but at the expense of
//    // extra stack usage.
//    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();


    // UART1 Init
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    ROM_IntMasterEnable();

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    ROM_IntEnable(INT_UART1);

    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    //UART1Send((uint8_t *)"UART initialization finished.\n", 30);


    // UART2 Init
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);

    // unlock gpio
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xFF;

    ROM_GPIOPinConfigure(GPIO_PD6_U2RX);
    ROM_GPIOPinConfigure(GPIO_PD7_U2TX);

    ROM_IntMasterEnable();

    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    UART_CONFIG_PAR_NONE));

    ROM_IntEnable(INT_UART2);
    ROM_UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);

    //UART2Send((uint8_t *)"Enter text: ", 12);

}

