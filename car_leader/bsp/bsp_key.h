/*
 * bsp_key.h
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: Aiano
 */

#ifndef BSP_BSP_KEY_H_
#define BSP_BSP_KEY_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "main.h"

void io_interrupt(void);
void bsp_key_init();

#endif /* BSP_BSP_KEY_H_ */
