/*
 * bsp_uart.c
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */


#include "bsp_uart.h"


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
UART2IntHandler(void)
{
    uint32_t ui32Status;

//
// Get the interrrupt status.
//
//ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    ui32Status = ROM_UARTIntStatus(UART2_BASE, true);

//
// Clear the asserted interrupts.
//
//ROM_UARTIntClear(UART0_BASE, ui32Status);
    ROM_UARTIntClear(UART2_BASE, ui32Status);

//
// Loop while there are characters in the receive FIFO.
//
//while(ROM_UARTCharsAvail(UART0_BASE))
    while(ROM_UARTCharsAvail(UART2_BASE))
    {
//
// Read the next character from the UART and write it back to the UART.
//
// ROM_UARTCharPutNonBlocking(UART0_BASE,
// ROM_UARTCharGetNonBlocking(UART0_BASE));
        ROM_UARTCharPutNonBlocking(UART2_BASE, ROM_UARTCharGetNonBlocking(UART2_BASE));
    }
}

void bsp_uart_init(){
    // UART2
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

    UART2Send((uint8_t *)"Enter text: ", 12);

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
