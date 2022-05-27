/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-03 21:36:30
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-03 22:05:54
 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_debug_usart.h"

SemaphoreHandle_t BinarySem_Handler = NULL;
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t Receive_TaskHandler = NULL;

void AppTaskCreate(void);
void Receive_Task(void);
void EXTI_Init(void);

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

    BinarySem_Handler = xSemaphoreCreateBinary();
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
    while (1)
    {
        xSemaphoreTake(BinarySem_Handler, portMAX_DELAY);
        xSemaphoreGive(BinarySem_Handler);

        printf("Receive_Task 正在运行中...\n");
        LED_Control(LED_RED, LED_ON);
        vTaskDelay(200);
        LED_Control(LED_RED, LED_OFF);
        vTaskDelay(200);
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
    uint32_t xCriticalReturn;
    uint32_t xSemaphoreReturn = pdPASS;
    BaseType_t pxHigherPriorityTaskWoken;
    xCriticalReturn = taskENTER_CRITICAL_FROM_ISR();

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
    {
        xSemaphoreReturn = xSemaphoreGiveFromISR(BinarySem_Handler, &pxHigherPriorityTaskWoken);
        if (pdPASS == xSemaphoreReturn)
        {
            printf("二值信号量释放成功...\n");
        }
        else
        {
            xSemaphoreReturn = xSemaphoreTakeFromISR(BinarySem_Handler, &pxHigherPriorityTaskWoken);
            if (pdTRUE == xSemaphoreReturn)
                printf("二值信号量剥夺成功...\n");
            else
                printf("二值信号量剥夺失败,Receive_Task 仍在运行...\n");
        }
        portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    }

    taskEXIT_CRITICAL_FROM_ISR(xCriticalReturn);
}
