#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "bool.h"
#include "status.h"
#include "status.h"


char SERVER_STATUS[20];


// Global constants
const char SERVER_STATUS_WAITING[] = "SERVER_STATUS_WAITING";
const char SERVER_STATUS_BUSY[]    = "SERVER_STATUS_BUSY";
const char SERVER_STATUS_READY[]   = "SERVER_STATUS_READY";
const char SERVER_STATUS_FAILURE[] = "SERVER_STATUS_FAILURE";

BOOL server_isReady()
{
  if ( strcmp(SERVER_STATUS, SERVER_STATUS_READY) == 0)
    return TRUE;
  else
    return FALSE;
}

char* server_GetStatus()
{
  printf("%s \n", SERVER_STATUS);
  return SERVER_STATUS;
}

void server_SetStatus(const char status[])
{
  // Check for status is equal to a valid status
  const int amount_of_statuses = 4;
  int counter = 0;
  BOOL valid_status = FALSE;

  for (counter = 0; counter < amount_of_statuses; counter++)
    {
      switch(counter)
	{
	case 0:
	  if (strcmp(status, SERVER_STATUS_WAITING) == 0) valid_status = TRUE;
	  break;
	case 1:
	  if (strcmp(status, SERVER_STATUS_BUSY) == 0) valid_status = TRUE;
	  break;
	case 2:
	  if (strcmp(status, SERVER_STATUS_READY) == 0) valid_status = TRUE;
	  break;
	case 3:
	  if (strcmp(status, SERVER_STATUS_FAILURE) == 0) valid_status = TRUE;
	  break;
	}

      if (valid_status)
	break;
    }

  // Set status
  if (valid_status)
    {
      strcpy(SERVER_STATUS, status);
    }
  else
    {
      printf("WARNING!! Unkown status given!!\n");
    }
}





