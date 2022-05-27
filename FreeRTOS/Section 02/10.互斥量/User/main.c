/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-04 12:52:39
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-04 21:51:52
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_debug_usart.h"
#include "bsp_key.h"
#include "bsp_led.h"

SemaphoreHandle_t Mutex_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Give_TaskHandler = NULL;
TaskHandle_t Low_TaskHandler = NULL;
TaskHandle_t Mid_TaskHandler = NULL;
TaskHandle_t High_TaskHandler = NULL;

void AppTaskCreate(void);
void Low_Task(void);
void Mid_Task(void);
void High_Task(void);

int main(void)
{
    UBaseType_t xReturn = pdPASS;
    HAL_Init();
    Key_Init();
    LED_Init();
    DEBUG_USART_Config();

    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)12,
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

    Mutex_Handler = xSemaphoreCreateMutex();
    xTaskCreate((TaskFunction_t)Low_Task,
                (const char *)"Low_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)8,
                (TaskHandle_t *)&Low_TaskHandler);
    xTaskCreate((TaskFunction_t)Mid_Task,
                (const char *)"Mid_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)9,
                (TaskHandle_t *)&Mid_TaskHandler);
    xTaskCreate((TaskFunction_t)High_Task,
                (const char *)"High_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)10,
                (TaskHandle_t *)&High_TaskHandler);

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Low_Task(void)
{
    static uint32_t i = 0;
    BaseType_t xReturn = pdTRUE;

    while (1)
    {
        printf("Low_Task try to get mutex..\n");
        xReturn = xSemaphoreTake(Mutex_Handler, portMAX_DELAY);
        if (xReturn == pdTRUE)
            printf("Low_Task got mutex and it is Running..\n\n");

        for (i = 0; i < 20000; i++)
            taskYIELD();

        xReturn = xSemaphoreGive(Mutex_Handler);
        printf("Low_Task given mutex..\n");
        vTaskDelay(1000);
    }
}

void Mid_Task(void)
{
    while (1)
    {
        printf("Mid_Task Runing\n");
        vTaskDelay(1000);
    }
}

void High_Task(void)
{
    BaseType_t xReturn = pdTRUE;
    while (1)
    {
        printf("High_Task try to get mutex..\n");

        xReturn = xSemaphoreTake(Mutex_Handler, portMAX_DELAY);
        if (pdTRUE == xReturn)
            printf("High_Task Runing\n");

        printf("High_Task given mutex..\n");
        xReturn = xSemaphoreGive(Mutex_Handler);

        vTaskDelay(1000);
    }
}
