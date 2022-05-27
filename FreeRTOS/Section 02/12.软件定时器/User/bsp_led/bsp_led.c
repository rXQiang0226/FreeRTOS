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

//˽�б��������ڼ�¼��ǰLED��״̬
static T_LED_Control LEDGreen_Status;
static T_LED_Control LEDRed_Status;

/**
 * @function Name: LED_Init
 * @description: LED �Ƴ�ʼ������
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
 * @description: ����ĳ�� LED ������
 * @param {T_LED} led : �����Ŀ� LED
 * @param {T_LED_Control} LedControl : ����״̬
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
 * @description: ��ȡָ�� LED ������״̬
 * @param {T_LED} led : �����Ŀ� LED
 * @return {*}
 * @author: rXQiang
 */
T_LED_Control LED_GetStatus(T_LED led)
{
    return ((led == LED_RED) ? (LEDRed_Status) : (LEDGreen_Status));
}

/**
 * @function Name: LED_Toggle
 * @description: ��ת LED ������
 * @param {T_LED} led : �����Ŀ� LED
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
