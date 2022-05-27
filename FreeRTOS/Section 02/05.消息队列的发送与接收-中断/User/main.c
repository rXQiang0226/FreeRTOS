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

QueueHandle_t Queue_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Send_TaskHandler = NULL;
TaskHandle_t Receive_TaskHandler = NULL;

void EXTI_Init(void);
void AppTaskCreate(void);
void Receive_Task(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    HAL_Init();
    EXTI_Init();
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

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_0;
    GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_Initure.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    NVIC_SetPriority(EXTI0_IRQn, 7);
}
void EXTI0_IRQHandler(void)
{
    BaseType_t pxHigherPriorityTaskWoken;
    uint32_t xReturn;
    static uint8_t flag = 1;

    xReturn = taskENTER_CRITICAL_FROM_ISR();

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
    {
        flag = (flag + 1) & 0x01;
        printf("EXTI-flag:%d\n",flag);
        xQueueSendFromISR(Queue_Handler, &flag, &pxHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    }

    taskEXIT_CRITICAL_FROM_ISR(xReturn);
}

void AppTaskCreate(void)
{
    taskENTER_CRITICAL();

    Queue_Handler = xQueueCreate((UBaseType_t)4, (UBaseType_t)1);
    xTaskCreate((TaskFunction_t)Receive_Task,
                (const char *)"Receive_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)6,
                (TaskHandle_t *)&Receive_TaskHandler);

    NVIC_EnableIRQ(EXTI0_IRQn);
    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Receive_Task(void)
{
    BaseType_t xReturn = pdPASS;
    uint8_t flag = 0;
    while (1)
    {
        xReturn = xQueueReceive(Queue_Handler, &flag, portMAX_DELAY);
        if (pdPASS == xReturn)
            printf("数据接收成功\n");
        else
            printf("数据接收失败\n");

        if (flag)
            LED_Control(LED_RED, LED_ON);
        else
            LED_Control(LED_RED, LED_OFF);
    }
}