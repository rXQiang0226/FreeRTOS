/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-04 12:52:39
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-04 18:57:24
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_debug_usart.h"
#include "bsp_key.h"
#include "bsp_led.h"

SemaphoreHandle_t CountSem_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Give_TaskHandler = NULL;
TaskHandle_t Take1_TaskHandler = NULL;
TaskHandle_t Take2_TaskHandler = NULL;
TaskHandle_t Take3_TaskHandler = NULL;

void AppTaskCreate(void);
void Give_Task(void);
void Take1_Task(void);
void Take2_Task(void);
void Take3_Task(void);

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
    BaseType_t xReturn = pdTRUE;
    taskENTER_CRITICAL();

    CountSem_Handler = xSemaphoreCreateCounting(3, 0);
    if (NULL != CountSem_Handler)
        printf("计数信号量创建成功，信号数量：3 初始信号量：0...\n");

    xReturn = xTaskCreate((TaskFunction_t)Give_Task,
                          (const char *)"Give_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)11,
                          (TaskHandle_t *)&Give_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)Take1_Task,
                          (const char *)"Take1_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)10,
                          (TaskHandle_t *)&Take1_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)Take2_Task,
                          (const char *)"Take2_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)9,
                          (TaskHandle_t *)&Take2_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)Take3_Task,
                          (const char *)"Take3_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)8,
                          (TaskHandle_t *)&Take3_TaskHandler);
    if (pdPASS == xReturn)
        printf("3个获取信号量的任务已创建但被阻塞,按下Up给予信号量...\n");

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Give_Task(void)
{
    UBaseType_t xReturn = pdTRUE;
    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            xReturn = xSemaphoreGive(CountSem_Handler);
            if (pdTRUE == xReturn)
                printf("成功释放一个信号量...\n");
        }

        vTaskDelay(50);
    }
}

void Take1_Task(void)
{
    while (1)
    {
        xSemaphoreTake(CountSem_Handler, portMAX_DELAY);
        printf("Take1_Task 正在运行...\n");
        vTaskDelay(200);
        xSemaphoreGive(CountSem_Handler);
    }
}

void Take2_Task(void)
{
    while (1)
    {
        xSemaphoreTake(CountSem_Handler, portMAX_DELAY);
        printf("Take2_Task 正在运行...\n");
        vTaskDelay(200);
        xSemaphoreGive(CountSem_Handler);
    }
}

void Take3_Task(void)
{
    while (1)
    {
        xSemaphoreTake(CountSem_Handler, portMAX_DELAY);
        printf("Take3_Task 正在运行...\n");
        vTaskDelay(200);
        xSemaphoreGive(CountSem_Handler);
    }
}
