/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-30 23:42:03
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-02 09:55:11
 */

#include "bsp_key.h"
#include "stm32f4xx.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_PIN_SET);

	GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
	GPIO_Initure.Mode = GPIO_MODE_INPUT;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_0;
	GPIO_Initure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}

Key_Status Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	if (GPIOE == GPIOx)
	{
		if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
		{
			while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);
			return KEY_ON;
		}
		else
		{
			return KEY_OFF;
		}
	}
	else
	{
		if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
		{
			while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET);
			return KEY_ON;
		}
		else
		{
			return KEY_OFF;
		}
	}
}

