#ifndef __ARGPARSE__H__
#define __ARGPARSE__H__

#include "llist.h"

struct funcinfo
{
  char Name[64];
  List* Args;
} typedef FunctionInfo;



FunctionInfo Arg_Parse(char* data);


#endif
