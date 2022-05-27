/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-07 21:04:30
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-07 21:43:37
 */

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_debug_usart.h"

TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Key_TaskHandler = NULL;
TimerHandle_t Timer_Handler = NULL;

uint16_t Timer_Counter = 0;

void AppTaskCreate(void);
void Timer_Callback(void);
void Key_Task(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    HAL_Init();
    Key_Init();
    LED_Init();
    DEBUG_USART_Config();

    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)8,
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

    Timer_Handler = xTimerCreate((const char *)"LED_Timer",
                                 pdMS_TO_TICKS(500),
                                 pdTRUE,
                                 (void *)1,
                                 (TimerCallbackFunction_t)Timer_Callback);
    if (NULL != Timer_Handler)
        printf("定时器创建成功，按下up启动...\n");

    xTaskCreate((TaskFunction_t)Key_Task,
                (const char *)"Key_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)7,
                (TaskHandle_t *)&Key_TaskHandler);

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Timer_Callback(void)
{
    static TickType_t tickNum = 0;

    Timer_Counter++;
    tickNum = xTaskGetTickCount();
    LED_Toggle(LED_RED);
    printf("Timer_Callback 被执行 %d 次\n", Timer_Counter);
    printf("Tick: %d \n", tickNum);
}
void Key_Task(void)
{
    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            printf("定时器启动成功，按下down停止...\n");
            xTimerStart(Timer_Handler, tmrCOMMAND_START);
        }

        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
        {
            printf("定时器停止成功，按下left删除...\n");
            xTimerStop(Timer_Handler, 0);
        }

        if (KEY_ON == Key_Scan(KEY_LEFT_PORT, KEY_LEFT_PIN))
        {
            printf("定时器删除成功，按下right重新创建...\n");
            xTimerDelete(Timer_Handler, 0);
            Timer_Counter = 0;
            Timer_Handler = NULL;
        }

        if (KEY_ON == Key_Scan(KEY_RIGHT_PORT, KEY_RIGHT_PIN))
        {
            if (NULL == Timer_Handler)
            {
                Timer_Handler = xTimerCreate((const char *)"LED_Timer",
                                             pdMS_TO_TICKS(500),
                                             pdTRUE,
                                             (void *)1,
                                             (TimerCallbackFunction_t)Timer_Callback);
                if (NULL != Timer_Handler)
                    printf("定时器创建成功，按下up启动...\n");
            }
        }

        vTaskDelay(50);
    }
}