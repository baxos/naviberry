/* 
   Author      : Jan Emil Bacher
   Name        : server.c
   Description : Server application used to recieve data from client and pass it further
                 on to system. See different list of commands in documentation/commands.lst
		 Furthermore documentation can be found in report in section regarding server app.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <bcm2835.h>
#include "./include/bool.h"
#include "./include/utils.h"
#include "./include/comm.h"
#include "./include/status.h"
#include "./include/bcmwrapper.h"
#include "./include/motordriver.h"

void print_err(const char* err_msg)
{
  printf("++++++++++++++++++++ ERROR +++++++++++++++++ \n");
  printf("%s \n", err_msg);
  printf("ERROR : %s \n", strerror(errno));
  exit(EXIT_FAILURE);
}


int main()
{
  struct sockaddr_in srv_adr, cli_adr;
  int sockfd, con_sockfd;
  const int   in_port = 1000;
  const char* in_host = "localhost";
  BOOL prog_running = TRUE;
  struct MOTOR* motA = malloc(sizeof(struct MOTOR*));
  

  // Create socket for server(listening socket)
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  
  // Zero set structures..
  memset(&srv_adr, 0, sizeof(struct sockaddr_in));
  memset(&cli_adr, 0, sizeof(struct sockaddr_in));
 
 
  // Set up server structure
  // Accept any IP addresses so far, this should be changed?? ******************** <------------- EDIT PLEASE
  // As well as port, all of this should be loaded by a config file..
  srv_adr.sin_family = AF_INET;
  srv_adr.sin_addr.s_addr = INADDR_ANY;
  srv_adr.sin_port   = htons(in_port);
  
  
  // Bind socket, and prepare for listening for incoming calls
  if (bind(sockfd, (struct sockaddr *) &srv_adr, sizeof(srv_adr)) < 0)
    {
      print_err("Binding socket failed");
    }
  else
    {
      printf("Socket is now bound..\n");
    }
  
  // And start listening, queue should be small
  if (listen(sockfd, 1) < 0)
    {
      print_err("Listening on socket failed");
    }
  else
    {
      printf("Socket successfully listening..\n");
    }
  

  // Wait for call, and accept it when present..
  int cli_len = sizeof(cli_adr);
  con_sockfd = accept(sockfd, (struct sockaddr*) &cli_adr, &cli_len);

  // Check if new socket, is ready or if it's error prone
  if (con_sockfd < 0)
    {
      print_err("Accepting incoming call failed..");
    }
  else
    {
      printf("Accepted incoming connection..\n");
      send(con_sockfd, "Hello\n\0", 6, 0);
    }


  // If we are here, we should now have a connected connection, voila~~~ 
  char buffer[256];
  size_t recv_size = 0;

  // Initiaize gpio pins
  if (!bcm2835_init())
    {
      print_err("BCM2835 failed to initialize.\n");
    }

  // Setup ports
  // Setup connected LED
  GPIO_setup(PIN5, GPIO_OUT);
  GPIO_out(PIN5, HIGH);

  printf("[+]Initializing motorA..\n");
  Motor_Init(&motA, PIN11, PIN13, PIN15);


  // Set server status to ready..
  server_SetStatus(SERVER_STATUS_READY);

  while (prog_running)
    {
      // Zero set buffer for every run..
      memset(buffer,0, sizeof(buffer));

      // Is server ready?
      if (server_isReady())
	{
	  // Tell client we are waiting
	  printf("Waiting for command..\n");
	  comm_sendtxt(con_sockfd, comm_REPLY_WAITING);
	  
	  // Wait for data
	  recv_size = recv(con_sockfd, buffer, sizeof(buffer), 0);
	  printf("Recieve size : %zu \n", recv_size);
	  
	  // Tell we recieved data
	  comm_sendtxt(con_sockfd, comm_REPLY_SUCCESS);
	  
	  // Trim string for unwished characters
	  util_TrimString(buffer);
	  	  
	  // Check for recognized cmds
	  if ( strcmp(buffer, "CLIENT_DISCONNECT") == 0)
	    {
	      comm_sendtxt(con_sockfd, comm_REPLY_DISCONNECT);
	      prog_running = FALSE;
	    }
	  else if ( strcmp(buffer, "CLIENT_PING") == 0)
	    {
	      comm_sendtxt(con_sockfd, comm_REPLY_PONG);
	    }
	  else if ( strcmp(buffer, "CLIENT_MOTORA_ON") == 0)
	    {
	      printf("Starting MotorA\n");
	      Motor_Start(&motA);
	    }
	  else if ( strcmp(buffer, "CLIENT_MOTORA_OFF") == 0)
	    {
	      printf("Stopping MotorA\n");
	      Motor_Stop(&motA);
	    }
	  else if ( strcmp(buffer, "CLIENT_MOTORA_FORWARD") == 0)
	    {
	      printf("MotorA Forward\n");
	      Motor_setDirection(&motA, 1);
	    }
	  else if ( strcmp(buffer, "CLIENT_MOTORA_BACKWARD") == 0)
	    {
	      printf("MotorB Backward\n");
	      Motor_setDirection(&motA, 0);
	    }
	  else
	    {
	      // If not a known command respond, we dont know it
	      comm_sendtxt(con_sockfd, comm_REPLY_UNKNOWN_CMD);
	    }
	  
	}
    }

  // Clear LED
  GPIO_out(PIN5, LOW);


  // Clean motor
  Motor_Clean(&motA);

  // Shut down sockets
  printf("Shutting down sockets..\n");
  close(sockfd);
  close(con_sockfd);
  return EXIT_SUCCESS;
}
