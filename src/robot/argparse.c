#include "llist.h"
#include "argparse.h"
#include <string.h>
#include <stdlib.h>
#include "bool.h"

/*
  arg_Parse
  
  This function parses the given string and converts it
  to the structure type FunctionInfo containing function name
  and a list of arguments.
*/

FunctionInfo* arg_Parse(char* data)
{
  FunctionInfo* newFunction = NULL;
  List* args;
  BOOL isName = TRUE;

  int offset, position;
  offset = position = 0;
  
  newFunction = malloc(sizeof(FunctionInfo)); 

  while (data[position] != 0)
    {
      position++;

      if (data[position] == '_')
	{
	  char* buffer = malloc(sizeof(char) * (position-offset) + 1);
	  memcpy(buffer, data+offset, (position-offset));
	  offset = position;

	  if (isName)
	    {
	      // Name
	      memset(newFunction->Name, 0, 63);
	      int nameSize = position - offset;
	      if (nameSize > 64)
		nameSize = 64;

	      memcpy(newFunction->Name, data+offset, (position-offset));
	      isName = FALSE;
	    }
	  else
	    {
	      // Args
	    }
	}
    }

  return newFunction;
}
