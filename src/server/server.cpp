/**
  * @name server.cpp
  * @author Jan Emil Bacher
  * @date 2015-2016
  * @brief The main file.
  * 
  * @mainpage Naviberry Documentation
  *
  */

/* 
   Author      : Jan Emil Bacher
   Name        : server.cpp
   Description : Server application used to recieve data from client and pass it further
                 on to system. See different list of commands in documentation/commands.lst
		 Furthermore documentation can be found in report in section regarding server app.
*/

#include <signal.h>
#include <cstring>
#include "./include/sonicsensor.hpp"
#include "./include/naviberryio.hpp"
#include "./include/communications.hpp"
#include "./include/bcmwrapper.hpp"
#include "./include/comm.hpp"


bool debugFlag = false;

// Atomic booleans for keeping the threads running, or closing them.
// std::atomic_bool mapmodeThreadRun;


void signal_callback_handle(int _signalnum)
{  
  if ( _signalnum == 2)
    {
      print_error("Interrupt signal recieved, CTRL-C pressed, closing down..");
    }
}


int main()
{
  // Initiaize gpio pins
  // and this????
  // how about cpp class wrapper instead of the c style wrap?
  if (!bcm2835_init())
    {
      print_error("BCM2835 failed to initialize.\n");
      exit(-1);
    }

  // We take care of the system signals
  signal(SIGINT, signal_callback_handle);

  // Sonic sensor for distance measurement
  // Mounted at PIN16, PIN18
  SonicSensor soundSensor(PIN16, PIN18);

  // Communications class for network communcation with mothership ^>^
  Naviberry::Communications communications;


  // Create server, start listen for clients.
  if ( (communications.CreateServer()) == false)
    {
      print_error("Error creating server.");
    }

  print_msg("All ready, waiting for connection.");

  // Block until connection is happeneing..
  communications.AwaitConnection();

  print_msg("Connected with client..");

  communications.StartRead();


  while (true)
    {
      if (communications.getPacketCount() > 0 )
	{
	  auto packets = communications.PopPackets();
	  print_msg("Popping packets .. ");
	  for ( auto p : packets )
	    {
	      /*
	      // Viewable print to stdout
	      std::cout << "=== Packet ===" << std::endl
			<< "time       = " << p.time_str << std::endl
			<< "size       = " << p.core.size << std::endl
			<< "data.size  = " << p.data.size() << std::endl
			<< "data.value = [ "; 

	      for ( auto d : p.data )
		{
		  std::cout << (char) d ;
		}
	      std::cout << " ]" << std::endl <<  std::endl;
	      */


	      // Lets parse data, split it up in different sections
	      if ( p.channel == communications.channels.fromName("system") )
		{
		  print_msg("System message. ");
		  auto data_msg = DeconstructDataToStr(p.data);

		  if ( data_msg == "PING" )
		    {
		      std::cout << "\t Recieved at " << p.time_str << "." << std::endl;
		      std::cout << "\t Responding with a pong" << std::endl;

		      communications.WriteText("PONG");
		    }
		}
	    }
	}
    }
  
  
  // just exit
  return EXIT_SUCCESS;
}
