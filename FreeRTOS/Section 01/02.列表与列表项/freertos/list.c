/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-21 20:51:10
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-21 22:05:04
 */

#include "list.h"

//��ʼ������ڵ�
void vListInitialiseItem(ListItem_t *const pxItem)
{
    pxItem->pvContainer = NULL;
}

//��ʼ��������ڵ�
void vListInitialise(List_t *const pxList)
{
    pxList->pxIndex = (ListItem_t *)&(pxList->xListEnd);
    pxList->xListEnd.xItemValue = portMAX_DELAY;

	pxList->xListEnd.pxNext = ( ListItem_t * ) &( pxList->xListEnd );
	pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );

    pxList->uxNumberOfItems = (UBaseType_t)0U;
}

//β�巨����ڵ�
void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *const pxIndex = pxList->pxIndex;

    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;
    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;

    pxNewListItem->pvContainer = (void *)pxList;

    (pxList->uxNumberOfItems)++;
}

//�������ڵ�
void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *pxIterator;
    const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;

    if (portMAX_DELAY == xValueOfInsertion)
    {
        pxIterator = pxList->xListEnd.pxPrevious;
    }
    else
    {
        for (pxIterator = (ListItem_t *)&(pxList->xListEnd);
             pxIterator->pxNext->xItemValue <= xValueOfInsertion;
             pxIterator = pxIterator->pxNext)
        {
            //Nothing to do
        }
    }

    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListItem;

    pxNewListItem->pvContainer = (void *)pxList;

    (pxList->uxNumberOfItems)++;
}

//ɾ���ڵ�
UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove)
{
    List_t *const pxList = (List_t *)pxItemToRemove->pvContainer;

    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;

    if (pxItemToRemove == pxList->pxIndex)
    {
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }
    pxItemToRemove->pvContainer = NULL;

    (pxList->uxNumberOfItems)--;

    return pxList->uxNumberOfItems;
}
