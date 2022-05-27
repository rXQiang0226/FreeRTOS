/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-03 23:29:58
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-03 23:50:38
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_debug_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"

QueueHandle_t CountSem_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Give_TaskHandler = NULL;
TaskHandle_t Take_TaskHandler = NULL;

void AppTaskCreate(void);
void Give_Task(void);
void Take_Task(void);

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
    BaseType_t xReturn = pdTRUE;
    taskENTER_CRITICAL();

    CountSem_Handler = xSemaphoreCreateCounting(5, 5);
    if (NULL != CountSem_Handler)
        printf("计数信号量创建成功...\n");

    xReturn = xTaskCreate((TaskFunction_t)Give_Task,
                          (const char *)"Give_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)6,
                          (TaskHandle_t *)&Give_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)Take_Task,
                          (const char *)"Take_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)7,
                          (TaskHandle_t *)&Take_TaskHandler);
    if (pdTRUE == xReturn)
        printf("任务创建成功...\n");

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Give_Task(void)
{
    BaseType_t xReturn = pdTRUE;

    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            xReturn = xSemaphoreGive(CountSem_Handler);
            if (pdTRUE == xReturn)
                printf("Up 按键被按下,释放一个计数信号量...\n");
        }

        vTaskDelay(20);
    }
}

void Take_Task(void)
{
    BaseType_t xReturn = pdTRUE;

    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
        {
            xReturn = xSemaphoreTake(CountSem_Handler, 0);
            if (pdTRUE == xReturn)
                printf("Down 按键被按下,获取到一个计数信号量...\n");
            else
                printf("Down 按键被按下,但计数信号量已空,没有获取到...\n");
        }

        vTaskDelay(20);
    }
}

