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
    TickType_t xItemValue;         //辅助值，用于帮助结点排序
    struct xLIST_ITEM *pxNext;     //指向链表下一节点
    struct xLIST_ITEM *pxPrevious; //指向链表前一节点
    void *pvOwner;                 //指向拥有该结点的内核对象
    void *pvContainer;             //指向该节点所在的链表
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
