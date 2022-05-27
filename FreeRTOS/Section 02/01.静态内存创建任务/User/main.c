/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-29 22:03:23
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-30 22:26:21
 */
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_debug_usart.h"
#include "bsp_led.h"

//���������붨ʱ�������ջ��TCB
 StackType_t IdleTask_Stack[configMINIMAL_STACK_SIZE];
 StackType_t TimerTask_Stack[configMINIMAL_STACK_SIZE];
StaticTask_t Timer_Task_TCB;
StaticTask_t IdleTask_TCB;

//������
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t LED1Task_Handler = NULL;
TaskHandle_t LED2Task_Handler = NULL;

//����ջ
StackType_t AppTaskCreate_Stack[128];
StackType_t LED1Task_Stack[128];
StackType_t LED2Task_Stack[128];

//������ƿ�
StaticTask_t AppTaskCreate_TCB;
StaticTask_t LED1Task_TCB;
StaticTask_t LED2Task_TCB;

//������
void AppTaskCreate(void);
void LED1_Task(void *param);
void LED2_Task(void *param);

//��̬�ڴ���亯��
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

/**
 * @function Name: main
 * @description: ������
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
int main(void)
{
    //Ӳ����ʼ��
    HAL_Init();
    DEBUG_USART_Config();
    printf("Ӳ����ʼ��...");
    LED_Init();

    //������������
    AppTaskCreate_Handler = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
                                              (const char *)"AppTaskCreate",
                                              (uint32_t)128,
                                              (void *)NULL,
                                              (UBaseType_t)4,
                                              (StackType_t *)&AppTaskCreate_Stack,
                                              (StaticTask_t *)&AppTaskCreate_TCB);
    if (NULL != AppTaskCreate_Handler)
    {
        //�������������
        vTaskStartScheduler();
    }

    while (1);
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
    //�����ٽ���
    taskENTER_CRITICAL();

    //����LED1(GREEN)����
    LED1Task_Handler = xTaskCreateStatic((TaskFunction_t)LED1_Task,
                                         (const char *)"LED1_Task",
                                         (uint32_t)128,
                                         (void *)NULL,
                                         (UBaseType_t)3,
                                         (StackType_t *)&LED1Task_Stack,
                                         (StaticTask_t *)&LED1Task_TCB);

    //����LED2(RED)����
    LED2Task_Handler = xTaskCreateStatic((TaskFunction_t)LED2_Task,
                                         (const char *)"LED2_Task",
                                         (uint32_t)128,
                                         (void *)NULL,
                                         (UBaseType_t)2,
                                         (StackType_t *)&LED2Task_Stack,
                                         (StaticTask_t *)&LED2Task_TCB);

    //��ӡ������Ϣ
    if (NULL != LED1Task_Handler)
        printf("LED1_Task �����ɹ�...");
    else
        printf("LED1_Task����ʧ��...");
    if (NULL != LED2Task_Handler)
        printf("LED2_Task �����ɹ�...");
    else
        printf("LED2_Task����ʧ��...");

    //ɾ����������
    vTaskDelete(AppTaskCreate_Handler);

    //�˳��ٽ���
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
void LED1_Task(void *param)
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
void LED2_Task(void *param)
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

/**
 * @function Name: vApplicationGetTimerTaskMemory
 * @description: ��ʱ������̬�����ڴ溯����ʹ�þ�̬�ڴ洴������ʱ����ʵ�ָú�����
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB;
    *ppxTimerTaskStackBuffer = TimerTask_Stack;
    *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @function Name: vApplicationGetIdleTaskMemory
 * @description: ��������̬�����ڴ溯����ʹ�þ�̬�ڴ洴������ʱ����ʵ�ָú�����
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &IdleTask_TCB;
    *ppxIdleTaskStackBuffer = IdleTask_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
