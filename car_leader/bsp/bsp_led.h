/*
 * bsp_led.h
 *
 *  Created on: 2022��7��28��
 *      Author: Aiano
 */

#ifndef BSP_BSP_LED_H_
#define BSP_BSP_LED_H_

#include "main.h"

typedef enum{
    BSP_LED_1,
    BSP_LED_2
}BSP_LED_t;

void bsp_led_init();
void bsp_led_set(BSP_LED_t led, bool state);

#endif /* BSP_BSP_LED_H_ */
