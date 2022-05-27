/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-21 20:51:28
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-21 21:49:56
 */

#ifndef _LIST_H
#define _LIST_H

#include "FreeRTOS.h"

struct xLIST_ITEM
{
    TickType_t xItemValue;         //����ֵ�����ڰ����������
    struct xLIST_ITEM *pxNext;     //ָ��������һ�ڵ�
    struct xLIST_ITEM *pxPrevious; //ָ������ǰһ�ڵ�
    void *pvOwner;                 //ָ��ӵ�иý����ں˶���
    void *pvContainer;             //ָ��ýڵ����ڵ�����
};
typedef struct xLIST_ITEM ListItem_t;

struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;
    struct xLIST_ITEM *pxNext;
    struct xLIST_ITEM *pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

struct xLIST
{
    UBaseType_t uxNumberOfItems;
    ListItem_t *pxIndex;
    MiniListItem_t xListEnd;
};
typedef struct xLIST List_t;

void vListInitialise(List_t * const pxList);
void vListInitialiseItem(ListItem_t * const pxItem);
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem);
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem);
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove);

#endif
