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
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <string>
#include <list>
#include <cstring>
#include <atomic>
extern "C"
{
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include "./include/bcm2835.h"
}
#include "./include/comm.hpp"
#include "./include/naviberryio.hpp"
#include "./include/bcmwrapper.hpp"
#include "./include/motordriver.hpp"
#include "./include/serial.hpp"
#include "./include/sonicsensor.hpp"
#include "./include/scheduler.hpp"
#include "./include/mapping.hpp"
#include "./include/motorcontroller.hpp"



bool debugFlag = true;

// Atomic booleans for keeping the threads running, or closing them.
std::atomic_bool mapmodeThreadRun;


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



  // Initialize hardware
  DC_Motor motorA(PIN11, PIN13, PIN15);
  DC_Motor motorB(PIN19, PIN21, PIN23);
  SonicSensor soundSensor(PIN16, PIN18);

  // Steering module
  MotorController controller(&motorA, &motorB);



  // Initialize networking
  Network net("localhost", 1000);



  MapHandler mapHandler(50);
  
  


  // New method to create server connection
  if (net.CreateServer() == true)
    {
      print_msg("Connection is setup");
    }

  // make sure we are connected
  // will hang until we are connected
  while (net.isConnected() == false);


  // Keep loop-on as long as client wants
  auto prog_running = true;
  // Make a string buffer, because all comm is as strings
  std::string buffer;


  // start up sensor
  soundSensor.AutoLoop();
  

  while (prog_running)
    {
      // Zero set buffer for every run..
      buffer = "";
      
      // Read from network, to network class buffer
      net.Read();
      
      // Check for packets
      net.CheckForPackets();
      
      // Check for combinations
      net.CheckForCombinations();
      
      if (debugFlag)
	{
	  print_msg("Checking textpacket queue");
	  std::cout << "Queue count : " << net.getTextQueueCount() << std::endl;
	}
      
      if (net.getTextQueueCount() > 0)
	{
	  std::list<TextPacket> queue = net.getTextQueue();
	  
	  for (auto it = queue.begin(); it!=queue.end(); it++)
	    {
	      
	      std::cout << "Packet value : " << it->value << std::endl;
	      buffer = it->value;
	      
	      auto a = buffer.compare("CLIENT_PING");
	      
	      if (buffer.compare("CLIENT_DISCONNECT")==0)
		{
		  print_warning("Recieved DISONNECT signal, closing down");
		  net.WriteText("Closing down");
		  prog_running = false;
		  break;
		}	  
	      else if (buffer.compare("CLIENT_READ_SENSORS")==0)
		{
		  auto current_distance = soundSensor.getReading();
		  uint8_t* rawBytes = new uint8_t[sizeof(current_distance)];
		  std::memset(&rawBytes[0], 0, sizeof(current_distance));
		  std::memcpy(&rawBytes[0], &current_distance, sizeof(current_distance));
		  net.WriteData(rawBytes, sizeof(current_distance), SENSOR_TYPE);
		  delete rawBytes;
		}
	      else if (buffer.compare("CLIENT_TEST_SENSOR")==0)
		{
		  auto dist = 200;
		  while (dist > 30)
		    {
		      // Drive forward
		      // Wait
		      // Stop
		      // Measure
		      // Repeat
		    }
		}
 // ============================== Both motors ======================================
	      else if (buffer.compare("CLIENT_MOTORS_START")==0)
		{
		  print_msg("Starting both motors..");
		  controller.Start();
		}
	      else if (buffer.compare("CLIENT_MOTORS_STOP!")==0)
		{
		  print_msg("Stopping both motors..");
		  controller.Stop();
		}
	      else if (buffer.compare("CLIENT_MOTORS_FORWARD")==0)
		{
		  motorA.setDirection(0);
		  motorB.setDirection(0);
		}
	      else if (buffer.compare("CLIENT_MOTORS_BACKWARD")==0)
		{
		  motorA.setDirection(1);
		  motorB.setDirection(1);
		}
	      else if (buffer.compare("CLIENT_MOTORS_TURNLEFT")==0)
		{
		  controller.TurnLeft();
		}
	      else if (buffer.compare("CLIENT_MOTORS_TURNRIGHT")==0)
		{
		  controller.TurnRight();
		}
	      else if (buffer.compare("CLIENT_DOWNLOAD_MAP")==0)
		{
		  // Map size is 50x50
		  uint8_t* data = mapHandler.getByteArray();
		  net.WriteData(data, mapHandler.getDataSize(), MAP_TYPE);
		}
	      else if (buffer.compare("CLIENT_PING_READ")==0)
		{
		  net.WriteText("PONG");
		}
	      else if(buffer.compare("CLIENT_MAP_MODE_START")==0)
		{
		  mapHandler.MapModeStart(&controller, &soundSensor);
		}
	      else if(buffer.compare("CLIENT_MAP_MODE_STOP")==0)
		{
		  mapHandler.MapModeStop();
		}
	      else
		{
		  print_warning("Unknown message recived :");
		  print_warning(buffer);    
		}
	    }
	  
	  // We processed all element, now clear queue
	  net.clearTextQueue();
	}	     
    }
  
  
  // just exit
  return EXIT_SUCCESS;
}
