#ifndef __STACK__H__
#define __STACK__H__

#include <stdint.h>
#include <string.h>
#include "bool.h"


typedef struct
{
  size_t stack_size;
  int stack_pointer;
  int8_t** the_stack;
} Stack;

int Stack_Create(Stack** _stk, size_t _size);
int Stack_Push(Stack** _stk,int8_t _val);
int Stack_Pop(Stack** _stk, int8_t _dst);
int Stack_Peek(Stack** _stk, int8_t _dst);
int Stack_Free(Stack** _stk);
BOOL Stack_isEmpty(Stack** _stk);
BOOL Stack_isFull(Stack** _stk);

#endif
