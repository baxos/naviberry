#ifndef __LLIST__H__
#define __LLIST__H__
#include "./bool.h"

struct list
{
  int data;
  struct list* next;
};

typedef struct list List;


void List_enQueue(List** lst, int newData);
int  List_deQueue(List** lst);
int  List_Peek(List** lst);

BOOL List_isEmpty(List** lst);
BOOL List_isFull(List **lst);
void List_Print(List** lst);
void List_Add(List** lst, int  newData);
List* List_GetLast(List** Lst);
void List_Free(List** lst);

#endif
