/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-10 18:07:27
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-10 19:20:35
 */

#include "FreeRTOS.h"
#include "task.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_debug_usart.h"

#define ULONG_MAX 0XFFFFFFFF

TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Key_TaskHandler = NULL;
TaskHandle_t Recv_TaskHandler = NULL;

void AppTaskCreate(void);
void Key_Task(void);
void Recv_Task(void);

int main(void)
{
    UBaseType_t xReturn = pdPASS;
    HAL_Init();
    DEBUG_USART_Config();
    LED_Init();
    Key_Init();

    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)9,
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
                          (UBaseType_t)9,
                          (TaskHandle_t *)&Key_TaskHandler);
    xReturn = xTaskCreate((TaskFunction_t)Recv_Task,
                          (const char *)"Recv_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)8,
                          (TaskHandle_t *)&Recv_TaskHandler);
    if (pdPASS == xReturn)
        printf("Task 创建成功...\n");

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Key_Task(void)
{
    /**
     * 形参 eSetValueWithoutOverwrite
     * 对象任务接收任务通知且对象任务没有通知值，那么通知值就会被设置为 ulValue。
     * 对象任务接收任务通知，但是上一次接收到的通知值并没有取走
     * 那么本次的通知值将不会更新，同时函数返回pdFALSE。
     * 在这种方式下，任务通知可以看成是函数 xQueueSend() 应用在队列深度为 1 的队列上的一种轻量型实现
     * 速度更快。
     */
    UBaseType_t xReturn = pdTRUE;
    uint32_t data0 = 0;
    uint32_t data1 = 1;

    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            xReturn = xTaskNotify(Recv_TaskHandler, data0, eSetValueWithoutOverwrite);
            if (pdPASS == xReturn)
                printf("data0 任务通知已释放...\n");
        }

        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
        {
            xReturn = xTaskNotify(Recv_TaskHandler, data1, eSetValueWithoutOverwrite);
            if (pdPASS == xReturn)
                printf("data1 任务通知已释放...\n");
        }
        vTaskDelay(50);
    }
}

void Recv_Task(void)
{
    /**
     * 形参 ulBitsToClearOnEntry
     * ulBitsToClearOnEntry 表示在使用通知之前将任务通知值的哪些位清0，
     * 实现过程就是将任务的通知值与参数ulBitsToClearOnEntry 的按位取反值按位与操作。
     * 设置为 0 表示对任务的通知值不进行清 0
     * 
     * 形参ulBitsToClearOnExit
     * 表示在函数 xTaskNotifyWait()退出前决定任务接收到的通知值的哪些位会被清0
     * 实现过程就是将任务的通知值与参数 ulBitsToClearOnExit 的按位取反值按位与操作。
     * 设置为 ULONG_MAX(0xFFFFFFFF) 表示对任务的通知值全部进行清 0
     */
    UBaseType_t xReturn = pdTRUE;
    uint32_t r_num;

    while (1)
    {
        xReturn = xTaskNotifyWait(0, ULONG_MAX, &r_num, portMAX_DELAY);
        if (pdTRUE == xReturn)
            printf("Recv1_Task 任务通知为 %d \n", r_num);

        if (r_num)
            LED_Control(LED_RED, LED_ON);
        else
            LED_Control(LED_RED, LED_OFF);
        vTaskDelay(100);
    }
}
