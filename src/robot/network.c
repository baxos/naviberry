#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include "./netpacket.h"
#include "./bool.h"
#include "./llist.h"
#include "./argparse.h"

#define ROBOT_INTERNET_INTERFACE "wlan0"
#define PORT_NUMBER 1000
#define IP_ADDRESS "192.168.0.110"

struct robIPaddr
{	
	uint8_t high;
	uint8_t midHigh;
	uint8_t midLow;
	uint8_t low;
};

int networkConnect(int *socket);

int main()
{
	struct ifaddrs *addrs, *tmp;
	struct sockaddr_in dest_addr;
	BOOL isConnected = FALSE;
	int sock = 0;
	
	// Testing llink.h
	List * test = NULL;
	int abc;

	for (abc=0; abc < 10; abc++)
	  {
	    List_enQueue(&test, abc);
	  }

	
	List_Print(&test);
	List_Free(&test);
	List_Print(&test);

	struct netPacket* lPacket;
	lPacket = createNetPacket("test..", 0, 0);

	

	printf("Packet created\nsec : %d \t msec : %d \n", lPacket->secCreated, lPacket->msCreated);
	printf("Sender : %d \t Reciever : %d \nDataSize : %d \n", lPacket->packSender, lPacket->packReciever, lPacket->dataSize);
	printf("data : %s \n", lPacket->data);


	// Setup connection
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&dest_addr, 0, sizeof(struct sockaddr_in));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	dest_addr.sin_port = htons(PORT_NUMBER);

	// Try connect
	int n = connect(sock, (struct sockaddr *) &dest_addr,sizeof (struct sockaddr_in)); 

	// Check for error
	if (n != 0)
	  {
	    printf("Error connecting to server..\n");
	    exit(-1);
	  }
	else
	  {
	    
	    isConnected = TRUE;
	  }

	// If not, we have a connection
	while (isConnected == TRUE)
	  {
	    printf("[IN] :");
	    char *inBuffer = NULL;
	    size_t inLen = NULL;
	    ssize_t test;

	    // Get input from terminal
	    test = getline(&inBuffer, &inLen, stdin);
	    
	    // Trim for whitespaces
	    int i = 0;

	    for (i=0; i < strlen(inBuffer); i++)
	      {
		if ( *(inBuffer+i) == '\n')
		  *(inBuffer+i) = 0;
	      }

	    // Compare it
	    if (strcmp(inBuffer, "quit") == 0)
	      {
		isConnected = FALSE;
	      }

	    free(inBuffer);
	  }
       

	// Shutdown socket
	shutdown(sock, SHUT_RDWR);

	// Free allocated memory and exit
	//free(hostName);
	//free(resName);
	freeifaddrs(addrs);

	
  	return 0;
}


int networkConnect(int *socket)
{
  struct sockaddr_in dest_addr;

  // Zero set structure
  memset(&dest_addr, 0, sizeof(struct sockaddr_in));

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
}
