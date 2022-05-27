/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-28 21:23:27
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-05 19:11:28
 */
#include "bsp_led.h"
#include "stm32f4xx.h"

//私有变量：用于记录当前LED的状态
static T_LED_Control LEDGreen_Status;
static T_LED_Control LEDRed_Status;

/**
 * @function Name: LED_Init
 * @description: LED 灯初始化函数
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
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

    LEDGreen_Status = LED_OFF;
    LEDRed_Status = LED_OFF;
}

/**
 * @function Name: LED_Control
 * @description: 控制某颗 LED 的亮灭
 * @param {T_LED} led : 具体哪颗 LED
 * @param {T_LED_Control} LedControl : 亮灭状态
 * @return {*}
 * @author: rXQiang
 */
void LED_Control(T_LED led, T_LED_Control LedControl)
{
    if (led == LED_RED)
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, (LedControl) ? (GPIO_PIN_RESET) : (GPIO_PIN_SET));
        LEDRed_Status = LedControl;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, (LedControl) ? (GPIO_PIN_RESET) : (GPIO_PIN_SET));
        LEDGreen_Status = LedControl;
    }
}

/**
 * @function Name: LED_Status
 * @description: 获取指定 LED 的亮灭状态
 * @param {T_LED} led : 具体哪颗 LED
 * @return {*}
 * @author: rXQiang
 */
T_LED_Control LED_GetStatus(T_LED led)
{
    return ((led == LED_RED) ? (LEDRed_Status) : (LEDGreen_Status));
}

/**
 * @function Name: LED_Toggle
 * @description: 翻转 LED 的亮灭
 * @param {T_LED} led : 具体哪颗 LED
 * @return {*}
 * @author: rXQiang
 */
void LED_Toggle(T_LED led)
{
    if (LED_RED == led)
        LED_Control(LED_RED, (T_LED_Control)!LEDRed_Status);
    else
        LED_Control(LED_GREEN, (T_LED_Control)!LEDGreen_Status);
}
