/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-30 23:11:48
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-03 11:51:13
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_debug_usart.h"
#include "bsp_key.h"
#include "bsp_led.h"

QueueHandle_t Queue_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Send_TaskHandler = NULL;
TaskHandle_t Receive_TaskHandler = NULL;

void AppTaskCreate(void);
void Send_Task(void);
void Receive_Task(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;

    HAL_Init();
    SystemClock_Init();
    Key_Init();
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

    Queue_Handler = xQueueCreate((UBaseType_t)4, (UBaseType_t)4);
    xTaskCreate((TaskFunction_t)Send_Task,
                (const char *)"Send_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)4,
                (TaskHandle_t *)&Send_TaskHandler);
    xTaskCreate((TaskFunction_t)Receive_Task,
                (const char *)"Receive_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)3,
                (TaskHandle_t *)&Receive_TaskHandler);

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Send_Task(void)
{
    UBaseType_t xReturn = pdPASS;
    uint32_t send_data0 = 0;
    uint32_t send_data1 = 1;

    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            printf("发送消息send_data0...\n");
            xReturn = xQueueSend(Queue_Handler, &send_data0, 0);

            if (pdPASS == xReturn)
                printf("消息send_data0发送成功...\n");
        }

        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
        {
            printf("发送消息send_data1...\n");
            xReturn = xQueueSend(Queue_Handler, &send_data1, 0);

            if (pdPASS == xReturn)
                printf("消息send_data1发送成功...\n");
        }

        vTaskDelay(20);
    }
}

void Receive_Task(void)
{
    BaseType_t xReturn = pdPASS;
    uint32_t r_queue;

    while (1)
    {
        xReturn = xQueueReceive(Queue_Handler, &r_queue, portMAX_DELAY);
        if (pdPASS == xReturn)
            printf("收到数据：%d...\n", r_queue);
        else
            printf("数据接收出错，错误代码:0x%1x\n", xReturn);
            
        if (r_queue)
            LED_Control(LED_RED, LED_ON);
        else
            LED_Control(LED_RED, LED_OFF);
    }
}
