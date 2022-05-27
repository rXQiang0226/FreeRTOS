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

//任务句柄
TaskHandle_t AppTaskCreate_Handler = NULL;
TaskHandle_t LEDGreenTask_Handler = NULL;
TaskHandle_t LEDRedTask_Handler = NULL;

//任务函数
void AppTaskCreate(void);
void LED_Green_Task(void);
void LED_Red_Task(void);

/**
 * @function Name: main
 * @description: 主函数
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
int main(void)
{
    BaseType_t xReturn = pdPASS;

    //硬件初始化
    HAL_Init();
    DEBUG_USART_Config();
    LED_Init();

    //创建启动任务
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char *)"AppTaskCreate",
                          (uint16_t)512,
                          (void *)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t *)&AppTaskCreate_Handler);

    //开启任务调度器
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
 * @description: 启动任务。用于创建红、绿灯任务。
 * @param {*}
 * @return {*}
 * @author: rXQiang
 */
void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    //进入临界区
    taskENTER_CRITICAL();

    //创建两个LED灯任务后删除自身
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
        printf("任务创建成功...");
    vTaskDelete(AppTaskCreate_Handler);

    //推出临界区
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
 * @description: 红灯任务
 * @param {void} *param
 *          无参数
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
