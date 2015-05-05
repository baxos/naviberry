#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>


void comm_sendtxt(int socket, const char txt[])
{
  send(socket, txt, strlen(txt) + 1, 0);
}





