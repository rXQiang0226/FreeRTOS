/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-30 22:28:10
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-30 22:46:34
 */

#include "FreeRTOS.h"
#include "task.h"
#include "bsp_debug_usart.h"
#include "bsp_led.h"

//������
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t LEDGreenTask_Handler = NULL;
TaskHandle_t LEDRedTask_Handler = NULL;

//������
void AppTaskCreate(void);
void LED_Green_Task(void);
void LED_Red_Task(void);

/**
 * @function Name: main
 * @description: ������
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
int main(void)
{
    BaseType_t xReturn = pdPASS;

    //Ӳ����ʼ��
    HAL_Init();
    DEBUG_USART_Config();
    LED_Init();

    //������������
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t *)&AppTaskCreate_Handler);

    //�������������
    if (pdPASS == xReturn)
        vTaskStartScheduler();
    else
        return -1;

    while (1)
    {
    }
}

/**
 * @function Name: AppTaskCreate
 * @description: �����������ڴ����졢�̵�����
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    //�����ٽ���
    taskENTER_CRITICAL();

    //��������LED�������ɾ������
    xReturn = xTaskCreate((TaskFunction_t)LED_Green_Task,
                          (const char *)"LED_Green_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t *)&LEDGreenTask_Handler);
    xReturn = xTaskCreate((TaskFunction_t)LED_Red_Task,
                          (const char *)"LED_Red_Task",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t *)&LEDRedTask_Handler);
    if (pdPASS == xReturn)
        printf("���񴴽��ɹ�...");
    vTaskDelete(AppTaskCreate_Handler);

    //�Ƴ��ٽ���
    taskEXIT_CRITICAL();
}

/**
 * @function Name: LED1_Task
 * @description: �̵�����
 * @param {void} *param
 *          �޲���
 * @return {*}
 * @author: rXQiang
 */
void LED_Green_Task(void)
{
    while (1)
    {
        LED_Control(LED_GREEN, LED_ON);
        printf("LED GREEN ON...");
        vTaskDelay(300);

        LED_Control(LED_GREEN, LED_OFF);
        printf("LED GREEN OFF...");
        vTaskDelay(300);
    }
}

/**
 * @function Name: LED2_Task
 * @description: �������
 * @param {void} *param
 *          �޲���
 * @return {*}
 * @author: rXQiang
 */
void LED_Red_Task(void)
{
    while (1)
    {
        LED_Control(LED_RED, LED_ON);
        printf("LED RED ON");
        vTaskDelay(500);

        LED_Control(LED_RED, LED_OFF);
        printf("LED RED OFF");
        vTaskDelay(500);
    }
}
