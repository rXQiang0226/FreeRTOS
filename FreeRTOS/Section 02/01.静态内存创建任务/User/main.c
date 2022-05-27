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

//空闲任务与定时器任务的栈、TCB
 StackType_t IdleTask_Stack[configMINIMAL_STACK_SIZE];
 StackType_t TimerTask_Stack[configMINIMAL_STACK_SIZE];
StaticTask_t Timer_Task_TCB;
StaticTask_t IdleTask_TCB;

//任务句柄
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t LED1Task_Handler = NULL;
TaskHandle_t LED2Task_Handler = NULL;

//任务栈
StackType_t AppTaskCreate_Stack[128];
StackType_t LED1Task_Stack[128];
StackType_t LED2Task_Stack[128];

//任务控制块
StaticTask_t AppTaskCreate_TCB;
StaticTask_t LED1Task_TCB;
StaticTask_t LED2Task_TCB;

//任务函数
void AppTaskCreate(void);
void LED1_Task(void *param);
void LED2_Task(void *param);

//静态内存分配函数
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

/**
 * @function Name: main
 * @description: 主函数
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
int main(void)
{
    //硬件初始化
    HAL_Init();
    DEBUG_USART_Config();
    printf("硬件初始化...");
    LED_Init();

    //创建启动任务
    AppTaskCreate_Handler = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
                                              (const char *)"AppTaskCreate",
                                              (uint32_t)128,
                                              (void *)NULL,
                                              (UBaseType_t)4,
                                              (StackType_t *)&AppTaskCreate_Stack,
                                              (StaticTask_t *)&AppTaskCreate_TCB);
    if (NULL != AppTaskCreate_Handler)
    {
        //开启任务调度器
        vTaskStartScheduler();
    }

    while (1);
}

/**
 * @function Name: AppTaskCreate
 * @description: 启动任务。用于创建红、绿灯任务。
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
void AppTaskCreate(void)
{
    //进入临界区
    taskENTER_CRITICAL();

    //创建LED1(GREEN)任务
    LED1Task_Handler = xTaskCreateStatic((TaskFunction_t)LED1_Task,
                                         (const char *)"LED1_Task",
                                         (uint32_t)128,
                                         (void *)NULL,
                                         (UBaseType_t)3,
                                         (StackType_t *)&LED1Task_Stack,
                                         (StaticTask_t *)&LED1Task_TCB);

    //创建LED2(RED)任务
    LED2Task_Handler = xTaskCreateStatic((TaskFunction_t)LED2_Task,
                                         (const char *)"LED2_Task",
                                         (uint32_t)128,
                                         (void *)NULL,
                                         (UBaseType_t)2,
                                         (StackType_t *)&LED2Task_Stack,
                                         (StaticTask_t *)&LED2Task_TCB);

    //打印串口消息
    if (NULL != LED1Task_Handler)
        printf("LED1_Task 创建成功...");
    else
        printf("LED1_Task创建失败...");
    if (NULL != LED2Task_Handler)
        printf("LED2_Task 创建成功...");
    else
        printf("LED2_Task创建失败...");

    //删除启动任务
    vTaskDelete(AppTaskCreate_Handler);

    //退出临界区
    taskEXIT_CRITICAL();
}

/**
 * @function Name: LED1_Task
 * @description: 绿灯任务
 * @param {void} *param
 *          无参数
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
 * @description: 红灯任务
 * @param {void} *param
 *          无参数
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
 * @description: 定时器任务静态分配内存函数。使用静态内存创建任务时必须实现该函数。
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
 * @description: 空闲任务静态分配内存函数。使用静态内存创建任务时必须实现该函数。
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
