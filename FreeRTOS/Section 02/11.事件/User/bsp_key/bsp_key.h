

#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "stm32f4xx.h"

#define KEY_UP_PORT GPIOA
#define KEY_DOWN_PORT GPIOE
#define KEY_LEFT_PORT GPIOE
#define KEY_RIGHT_PORT GPIOE

#define KEY_UP_PIN GPIO_PIN_0
#define KEY_DOWN_PIN GPIO_PIN_3
#define KEY_LEFT_PIN GPIO_PIN_2
#define KEY_RIGHT_PIN GPIO_PIN_4

enum E_KEYSTATUS
{
    KEY_OFF = 0,
    KEY_ON
};
typedef enum E_KEYSTATUS Key_Status;

enum E_KEY
{
    KEY_UP = 0,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};
typedef enum E_KEY T_Key;

void Key_Init(void);
Key_Status Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
#endif



