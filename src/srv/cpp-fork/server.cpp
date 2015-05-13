/* 
   Author      : Jan Emil Bacher
   Name        : server.c
   Description : Server application used to recieve data from client and pass it further
                 on to system. See different list of commands in documentation/commands.lst
		 Furthermore documentation can be found in report in section regarding server app.
*/
#include <iostream>
#include <algorithm>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include "./include/bcm2835.h"
}
#include <string>
#include "./include/comm.hpp"
#include "./include/naviberryio.hpp"
#include "./include/bcmwrapper.hpp"
#include "./include/motordriver.hpp"

int main()
{
  // Initiaize gpio pins
  if (!bcm2835_init())
    {
      print_error("BCM2835 failed to initialize.\n");
      exit(-1);
    }

  // Create motor class
  DC_Motor motor(PIN11, PIN13, PIN15);  
  // Create network class
  Network net("localhost", 1000);


  // Setup network connection  
  // Craete socket
  if (!net.Create())
    {
      print_error("Error trying to create socket");
      exit(-1);
    }
  // Bind socket
  if (!net.Bind())
    {
      print_error("Error trying to create socket");
      exit(-1);
    }
  // Listen on socket
  if (!net.Listen())
    {
      print_error("Error trying to listen on socket");
      exit(-1);
    }
  // Accept connection
  if (!net.Accept())
    {
      print_error("Error accepting connection");
      exit(-1);
    }


  // Keep loop-on as long as client wants
  auto prog_running = true;
  // Make a string buffer, because all comm is as strings
  std::string buffer;


  while (prog_running)
    {
      // Zero set buffer for every run..
      // memset(buffer,0, sizeof(buffer));
      buffer = "";
      
      // Tell client we are waiting
      print_msg("Waiting for command..");
      net.WriteText(comm_REPLY_WAITING);
      
      // Read text from client
      buffer = net.ReadText();

      // Tell we recieved data
      net.WriteText(comm_REPLY_SUCCESS);
      
      // Check for recognized cmds
      if (buffer.compare("CLIENT_DISCONNECT")==0)
	    {
	      print_warning("Recieved DISONNECT signal, closing down");
	      net.WriteText(comm_REPLY_DISCONNECT);
	      prog_running = false;
	    }	  
      else if(buffer.compare("CLIENT_MOTOR_START")==0)
	{
	  // Start motor
	  print_msg("STARTING MOTOR");
	  motor.Start();
	}
      else if(buffer.compare("CLIENT_MOTOR_STOP")==0)
	{
	  // Stop motor
	  motor.Stop();
	}
      else
	{
	  print_warning("Unknown message recived :");
	  print_warning(buffer);
	 
	}
      

    }

  // just exit
  return EXIT_SUCCESS;
}
