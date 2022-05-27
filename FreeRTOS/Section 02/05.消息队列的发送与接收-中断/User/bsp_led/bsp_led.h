/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-28 21:23:20
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-28 21:33:59
 */

#ifndef _BSP_LED_H
#define _BSP_LED_H
#include "system.h"

enum E_LED
{
    LED_RED = 0,
    LED_GREEN
};
typedef enum E_LED T_LED;

enum E_LEDCONTROL
{
    LED_OFF = 0,
    LED_ON
};
typedef enum E_LEDCONTROL T_LED_Control;

void LED_Init(void);
void LED_Control(T_LED led, T_LED_Control LedControl);

#endif

