/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-10-03 19:32:20
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-10-03 20:18:00
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_debug_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"

SemaphoreHandle_t BinarySem_Handler = NULL;
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
    printf("������ֵ�ź�����������....\n");

    BinarySem_Handler = xSemaphoreCreateBinary();
    xTaskCreate((TaskFunction_t)Send_Task,
                (const char *)"Send_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)8,
                (TaskHandle_t *)&Send_TaskHandler);
    xTaskCreate((TaskFunction_t)Receive_Task,
                (const char *)"Receive_Task",
                (uint16_t)512,
                (void *)NULL,
                (UBaseType_t)7,
                (TaskHandle_t *)&Receive_TaskHandler);

    printf("�������....\n����UP���ͷŶ�ֵ�ź���...\n");
    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Send_Task(void)
{
    UBaseType_t xReturn = pdPASS;
    while (1)
    {
        if (KEY_ON == Key_Scan(KEY_UP_PORT, KEY_UP_PIN))
        {
            xReturn = xSemaphoreGive(BinarySem_Handler);
#if 0
            if (pdTRUE == xReturn)
            {
                printf("��ֵ�ź����ͷųɹ�...\n");
            }
            else
            {
                printf("��ֵ�ź����ͷ�ʧ��...\n");
            }
#else
            if (pdTRUE == xReturn)
            {
                printf("��ֵ�ź����ͷųɹ�...\n");
                printf("Receive_Task ����һֱ���У��ٴΰ�����ֹ����...\n");
            }
            else
            {
                xReturn = xSemaphoreTake(BinarySem_Handler, portMAX_DELAY);
                if (pdTRUE == xReturn)
                {
                    printf("�ɹ����� nReceive_Task ����Ķ�ֵ�ź���...\n");
                }
            }
#endif
        }
        vTaskDelay(30);
    }
}

void Receive_Task(void)
{
#if 0
    BaseType_t xReturn = pdPASS;
    static uint8_t counter = 1;
    while (1)
    {
        xReturn = xSemaphoreTake(BinarySem_Handler, portMAX_DELAY);
        if (pdTRUE == xReturn)
            printf("��ֵ�ź�����ȡ�ɹ�...\n");

        LED_Control(LED_RED, LED_ON);
        printf("Receive_Task �� %d ������...\n", counter++);
        vTaskDelay(500);
    }
#else
    BaseType_t xReturn = pdPASS;
    while (1)
    {
        xReturn = xSemaphoreTake(BinarySem_Handler, portMAX_DELAY);
        xReturn = xSemaphoreGive(BinarySem_Handler);

        LED_Control(LED_RED, LED_ON);
        vTaskDelay(200);
        LED_Control(LED_RED, LED_OFF);
        vTaskDelay(200);
        printf("Receive_Task ����������...\n");
    }
#endif
}