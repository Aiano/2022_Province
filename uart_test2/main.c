#include "bsp_uart.h"


// this project test uart1 and uart3 working at the same time.
void main(void)
{
    // System Clock
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
    SYSCTL_XTAL_16MHZ);

    bsp_uart_init();

    while (1)
    {

    }
}
