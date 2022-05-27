/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-25 21:28:24
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-25 21:53:22
 */
#include "FreeRTOS.h"
#include "task.h"

portCHAR flag1;
portCHAR flag2;

//��������б�
//�����С�������������ȼ� configMAX_PRIORITIES ����
//�����б���±��Ӧ�����������ȼ�
extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

//���� 1
TaskHandle_t Task1_Handler;
#define TASK1_STACK_SIZE 128
StackType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t Task1_TCB;

//���� 2
TaskHandle_t Task2_Handler;
#define TASK2_STACK_SIZE 128
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t Task2_TCB;

void delay(uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

int main(void)
{
    //��ʼ����������б���������б�
    prvInitialiseTaskLists();

    //��̬�������񲢽������������б�
    Task1_Handler = xTaskCreateStatic(
        (TaskFunction_t)Task1_Entry,
        (char *)"Task1_Entry",
        (uint32_t)TASK1_STACK_SIZE,
        (void *)NULL,
        (StackType_t *)Task1Stack,
        (TCB_t *)&Task1_TCB);
    vListInsertEnd(&(pxReadyTasksLists[1]), &(((TCB_t *)(&Task1_TCB))->xStateListItem));

    //��̬�������񲢽������������б�
    Task2_Handler = xTaskCreateStatic(
        (TaskFunction_t)Task2_Entry,
        (char *)"Task2_Entry",
        (uint32_t)TASK2_STACK_SIZE,
        (void *)NULL,
        (StackType_t *)Task2Stack,
        (TCB_t *)&Task2_TCB);
    vListInsertEnd(&(pxReadyTasksLists[2]), &(((TCB_t *)(&Task2_TCB))->xStateListItem));

    //�������������
    vTaskStartScheduler();
}

void delay(uint32_t count)
{
    for (; count > 0; count--)
        ;
}
void Task1_Entry(void *p_arg)
{
    while (1)
    {
        flag1 = 1;
        delay(100);
        flag1 = 0;
        delay(100);

        taskYIELD();
    }
}
void Task2_Entry(void *p_arg)
{
    while (1)
    {
        flag2 = 1;
        delay(100);
        flag2 = 0;
        delay(100);

        taskYIELD();
    }
}
