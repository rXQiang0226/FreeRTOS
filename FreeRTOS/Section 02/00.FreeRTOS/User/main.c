/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-30 23:11:48
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-03 13:30:58
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_debug_usart.h"
#include "bsp_key.h"
#include "bsp_led.h"

TaskHandle_t AppTaskCreate_Handler = NULL;

void AppTaskCreate(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    HAL_Init();
    LED_Init();
    DEBUG_USART_Config();

    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)5,
                          (TaskHandle_t *)&AppTaskCreate_Handler);
    if (pdPASS == xReturn)
        vTaskStartScheduler();
    else
        return 0;

    while (1)
    {
    }
}

void AppTaskCreate(void)
{
    taskENTER_CRITICAL();

	
	
    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}
