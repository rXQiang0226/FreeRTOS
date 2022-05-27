/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-04 22:24:48
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-05 20:29:28
 */

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_debug_usart.h"

EventGroupHandle_t Event_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Key_TaskHandler = NULL;
TaskHandle_t Receive_TaskHandler = NULL;

void AppTaskCreate(void);
void Key_Task(void);
void Receive_Task(void);

int main(void)
{
    BaseType_t xReturn = pdTRUE;

    HAL_Init();
    Key_Init();
    LED_Init();
    DEBUG_USART_Config();

    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate, "AppTaskCreate", 512, (void *)NULL, 9, &AppTaskCreate_Handler);
    if (pdTRUE == xReturn)
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

    Event_Handler = xEventGroupCreate();
    if (NULL != Event_Handler)
        printf("事件组创建成功...\n");

    xReturn = xTaskCreate((TaskFunction_t)Key_Task, "Key_Task", 512, (void *)NULL, 8, &Key_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)Receive_Task, "Receive_Task", 512, (void *)NULL, 7, &Receive_TaskHandler);
    if (pdTRUE == xReturn)
        printf("任务创建成功...\n");

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}
void Key_Task(void)
{
    const uint32_t KEY1_EVENT = (0x01 << 0);
    const uint32_t KEY2_EVENT = (0x01 << 1);

    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            printf("触发Key1..\n");
            xEventGroupSetBits(Event_Handler, KEY1_EVENT);
        }
        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
        {
            printf("触发Key2..\n");
            xEventGroupSetBits(Event_Handler, KEY2_EVENT);
        }
        vTaskDelay(50);
    }
}
void Receive_Task(void)
{
    const uint32_t KEY1_EVENT = (0x01 << 0);
    const uint32_t KEY2_EVENT = (0x01 << 1);
    EventBits_t r_event;

    while (1)
    {
        r_event = xEventGroupWaitBits(Event_Handler,
                                      KEY1_EVENT | KEY2_EVENT,
                                      pdTRUE,
                                      pdTRUE,
                                      portMAX_DELAY);
        if ((r_event & (KEY1_EVENT | KEY2_EVENT)) == (KEY1_EVENT | KEY2_EVENT))
        {
            printf("Receive_Task:检测到 Key1 & Key2 都被按下...\n");
        }

        vTaskDelay(50);
    }
}
