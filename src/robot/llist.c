#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./llist.h"
//#include "bool.h"


int List_deQueue(List** lst)
{
  if (List_isEmpty(&(*lst)) == TRUE)
    return NULL;
  if (List_isFull(&(*lst)) == TRUE)
    return NULL;


  List* curr = (*lst);
  List* next = curr->next;
  int value = 0;

  while (next->next != NULL)
    {
      curr = next;
      next = curr->next;
    }

  // curr shoud now be second last
  // set the data to value, so we can return later
  // and free next, and cut link
  value = next->data;
  free(curr->next);
  curr->next = NULL;

  return value;
}

int List_Peek(List** lst)
{
  List* curr = List_GetLast(&(*lst));
  return curr->data;
}

// Checks for list if full(out of memory)
BOOL List_isFull(List** lst)
{
  List * memTest = malloc(sizeof(List));
  if (memTest == NULL)
    {
      free(memTest);
      return TRUE;
    }
  else
    {
      free(memTest);
      return FALSE;
    }
}

// Checks for list is empty or not.
char List_isEmpty(List** lst)
{
  if ((*lst) == NULL)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

// Prints out the list
void List_Print(List** lst)
{
  int i = 0;
  List* current = (*lst);
  
  while (current != NULL)
    {
      printf("List[%d] : %d \n", i, current->data);
      i++;
      current = current->next;
    }
}

// Get last item in the list
List* List_GetLast(List** lst)
{
  List * current = (*lst);
  while (current->next != NULL)
    {
      current = current->next;
    }

  return current;
}

// Free memory allocation for the list
void List_Free(List** lst)
{
  List *curr, *nxt;

  curr = (*lst);
  nxt = curr->next;

  while (nxt != NULL)
    {
      List * tmp = curr;
      tmp->next = NULL;
      free(tmp);
      curr = nxt;
      nxt = nxt->next;

      printf("Freeing list entry..\n");
    }

  
  free((*lst));
  (*lst) = NULL;
  free(curr);
}


// Adds an element to the list
void List_enQueue(List** lst, char* newData)
{
  if ( List_isFull(&(*lst)) == FALSE)
    {
      if ( List_isEmpty(&(*lst)) == TRUE)
	{
	  // If list is empty, create it
	  // Allocate memory 
	  (*lst) = malloc(sizeof(List));
	  // Set values
	  (*lst)->next = NULL;
	  (*lst)->data = newData;
	}
      else
	{
	  List * current = List_GetLast(&(*lst));
	  // Now we should be at the end of the list
	  List * tmp = NULL;
	  tmp = malloc(sizeof(List));
	  tmp->next = NULL;
	  tmp->data = newData;
	  // Point to new element
	  current->next = tmp;
	}
    }
}


