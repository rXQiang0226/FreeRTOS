/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-28 21:23:27
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-28 21:45:09
 */
#include "bsp_led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);

    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
}

void LED_Control(T_LED led, T_LED_Control LedControl)
{
    if (led == LED_RED)
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, (LedControl) ? (GPIO_PIN_RESET) : (GPIO_PIN_SET));
    }
    else
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, (LedControl) ? (GPIO_PIN_RESET) : (GPIO_PIN_SET));
    }
}
