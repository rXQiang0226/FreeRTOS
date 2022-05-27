/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-21 20:45:12
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-21 22:00:28
 */
#include "list.h"

struct xLIST List_Test;

struct xLIST_ITEM List_Item1;
struct xLIST_ITEM List_Item2;
struct xLIST_ITEM List_Item3;

int main(void)
{
	vListInitialise(&List_Test);
	vListInitialiseItem(&List_Item1);
	vListInitialiseItem(&List_Item2);
	vListInitialiseItem(&List_Item3);

	vListInsert(&List_Test, &List_Item1);
	vListInsert(&List_Test, &List_Item2);
	vListInsert(&List_Test, &List_Item3);
	
	while(1);
}
