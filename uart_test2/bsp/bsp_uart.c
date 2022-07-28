/*
 * bsp_uart.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "bsp_uart.h"


void
UART3Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
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
        ROM_UARTCharPutNonBlocking(UART3_BASE, *pui8Buffer++);
    }
}

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
//ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
        ROM_UARTCharPutNonBlocking(UART1_BASE, *pui8Buffer++);
    }
}

void
UART1IntHandler(void)
{
    uint32_t ui32Status;

//
// Get the interrrupt status.
//
//ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

//
// Clear the asserted interrupts.
//
//ROM_UARTIntClear(UART0_BASE, ui32Status);
    ROM_UARTIntClear(UART1_BASE, ui32Status);

//
// Loop while there are characters in the receive FIFO.
//
//while(ROM_UARTCharsAvail(UART0_BASE))
    while(ROM_UARTCharsAvail(UART1_BASE))
    {
//
// Read the next character from the UART and write it back to the UART.
//
// ROM_UARTCharPutNonBlocking(UART0_BASE,
// ROM_UARTCharGetNonBlocking(UART0_BASE));
        ROM_UARTCharPutNonBlocking(UART1_BASE, ROM_UARTCharGetNonBlocking(UART1_BASE));
    }
}

void
UART3IntHandler(void)
{
    uint32_t ui32Status;

//
// Get the interrrupt status.
//
//ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    ui32Status = ROM_UARTIntStatus(UART3_BASE, true);

//
// Clear the asserted interrupts.
//
//ROM_UARTIntClear(UART0_BASE, ui32Status);
    ROM_UARTIntClear(UART3_BASE, ui32Status);

//
// Loop while there are characters in the receive FIFO.
//
//while(ROM_UARTCharsAvail(UART0_BASE))
    while(ROM_UARTCharsAvail(UART3_BASE))
    {
//
// Read the next character from the UART and write it back to the UART.
//
// ROM_UARTCharPutNonBlocking(UART0_BASE,
// ROM_UARTCharGetNonBlocking(UART0_BASE));
        ROM_UARTCharPutNonBlocking(UART3_BASE, ROM_UARTCharGetNonBlocking(UART3_BASE));
    }
}

void bsp_uart_init(){
    // UART3
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

    ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);

    ROM_IntMasterEnable();

    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    ROM_UARTConfigSetExpClk(UART3_BASE, ROM_SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    UART_CONFIG_PAR_NONE));

    ROM_IntEnable(INT_UART3);
    ROM_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);

    UART3Send((uint8_t *)"Enter text: ", 12);

    // UART1
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);

    ROM_IntMasterEnable();

    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    UART_CONFIG_PAR_NONE));

    ROM_IntEnable(INT_UART1);
    ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);

    UART1Send((uint8_t *)"Enter text: ", 12);
}
