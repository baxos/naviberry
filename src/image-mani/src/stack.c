#include <stdlib.h>
#include <stdint.h>
#include "stack.h"
#include "bool.h"

BOOL Stack_isEmpty(Stack** _stk)
{
  if ((*_stk)->stack_pointer == -1)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

BOOL Stack_isFull(Stack** _stk)
{
  if ((*_stk)->stack_pointer == (*_stk)->stack_size)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

int Stack_Create(Stack** _stk, size_t _size)
{
  // If pointer is not null we do not continue
  if (_stk != NULL)
    {
      return 0;
    }

  // Malloc for struct
  (*_stk) = malloc(sizeof(Stack));

  // Set initial values
  (*_stk)->stack_size = _size;
  (*_stk)->stack_pointer = 0;

  // Malloc for stack size
  (*_stk)->the_stack = malloc(sizeof(void*) * (*_stk)->stack_size);

  // Return succes
  return 1;
}

int Stack_Push(Stack** _stk, int8_t _val)
{
  if (Stack_isFull(&(*_stk)) == FALSE)
    {
      // Increase pointer
      (*_stk)->stack_pointer++;
      // Set value
      (*_stk)->the_stack[(*_stk)->stack_pointer] = &_val;
      
      // Return success
      return 1;
    }
    else
      {
	// Return failure
	return 0;
      }
}

int Stack_Pop(Stack** _stk, int8_t _dst)
{
  if (Stack_isEmpty(&(*_stk)) == TRUE)
    {
      return 0;
    }
  else
    {
      _dst = *(*_stk)->the_stack[(*_stk)->stack_pointer];
      (*_stk)->stack_pointer--;
    }
}
