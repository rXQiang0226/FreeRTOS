/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-30 23:42:03
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-02 18:33:01
 */

#include "FreeRTOS.h"
#include "task.h"
#include "bsp_key.h"
#include "bsp_led.h"

TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Key_TaskHandler = NULL;
TaskHandle_t LED_TaskHandler = NULL;

void AppTaskCreate(void);
void Key_Task(void);
void LED_Task(void);

int main(void)
{
    UBaseType_t xReturn = pdPASS;

    HAL_Init();
    Key_Init();
    LED_Init();

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
    UBaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    xReturn = xTaskCreate((TaskFunction_t)Key_Task,
                          (const char *)"Key_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t *)&Key_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)LED_Task,
                          (const char *)"LED_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t *)&LED_TaskHandler);
    vTaskSuspend(LED_TaskHandler);

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Key_Task(void)
{
    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
            vTaskSuspend(LED_TaskHandler);
            
        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
            vTaskResume(LED_TaskHandler);

        //必须添加延时，让按键任务让出CPU使用权
        //如果不添加延时，会因为按键任务的优先级比LED任务高导致CPU无法成功调度LED任务
        vTaskDelay(50);
    }
}

void LED_Task(void)
{
    while (1)
    {
        LED_Control(LED_RED, LED_ON);
        vTaskDelay(200);

        LED_Control(LED_RED, LED_OFF);
        vTaskDelay(200);
    }
}
