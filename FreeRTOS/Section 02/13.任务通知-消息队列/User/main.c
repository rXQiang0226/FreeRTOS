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
        printf("Task �����ɹ�...\n");

    vTaskDelete(AppTaskCreate_Handler);
    taskEXIT_CRITICAL();
}

void Key_Task(void)
{
    /**
     * �β� eSetValueWithoutOverwrite
     * ���������������֪ͨ�Ҷ�������û��ֵ֪ͨ����ôֵ֪ͨ�ͻᱻ����Ϊ ulValue��
     * ���������������֪ͨ��������һ�ν��յ���ֵ֪ͨ��û��ȡ��
     * ��ô���ε�ֵ֪ͨ��������£�ͬʱ��������pdFALSE��
     * �����ַ�ʽ�£�����֪ͨ���Կ����Ǻ��� xQueueSend() Ӧ���ڶ������Ϊ 1 �Ķ����ϵ�һ��������ʵ��
     * �ٶȸ��졣
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
                printf("data0 ����֪ͨ���ͷ�...\n");
        }

        if (KEY_ON == Key_Scan(KEY_DOWN_PORT, KEY_DOWN_PIN))
        {
            xReturn = xTaskNotify(Recv_TaskHandler, data1, eSetValueWithoutOverwrite);
            if (pdPASS == xReturn)
                printf("data1 ����֪ͨ���ͷ�...\n");
        }
        vTaskDelay(50);
    }
}

void Recv_Task(void)
{
    /**
     * �β� ulBitsToClearOnEntry
     * ulBitsToClearOnEntry ��ʾ��ʹ��֪֮ͨǰ������ֵ֪ͨ����Щλ��0��
     * ʵ�ֹ��̾��ǽ������ֵ֪ͨ�����ulBitsToClearOnEntry �İ�λȡ��ֵ��λ�������
     * ����Ϊ 0 ��ʾ�������ֵ֪ͨ�������� 0
     * 
     * �β�ulBitsToClearOnExit
     * ��ʾ�ں��� xTaskNotifyWait()�˳�ǰ����������յ���ֵ֪ͨ����Щλ�ᱻ��0
     * ʵ�ֹ��̾��ǽ������ֵ֪ͨ����� ulBitsToClearOnExit �İ�λȡ��ֵ��λ�������
     * ����Ϊ ULONG_MAX(0xFFFFFFFF) ��ʾ�������ֵ֪ͨȫ�������� 0
     */
    UBaseType_t xReturn = pdTRUE;
    uint32_t r_num;

    while (1)
    {
        xReturn = xTaskNotifyWait(0, ULONG_MAX, &r_num, portMAX_DELAY);
        if (pdTRUE == xReturn)
            printf("Recv1_Task ����֪ͨΪ %d \n", r_num);

        if (r_num)
            LED_Control(LED_RED, LED_ON);
        else
            LED_Control(LED_RED, LED_OFF);
        vTaskDelay(100);
    }
}
