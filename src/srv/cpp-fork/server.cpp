/* 
   Author      : Jan Emil Bacher
   Name        : server.c
   Description : Server application used to recieve data from client and pass it further
                 on to system. See different list of commands in documentation/commands.lst
		 Furthermore documentation can be found in report in section regarding server app.
*/
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
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
#include "./include/serial.hpp"
#include "./include/sonicsensor.hpp"

void rob_sleep(int x)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

int main()
{
  // Initiaize gpio pins
  if (!bcm2835_init())
    {
      print_error("BCM2835 failed to initialize.\n");
      exit(-1);
    }

  // Create motor class
  DC_Motor motorA(PIN11, PIN13, PIN15);  
  DC_Motor motorB(PIN19, PIN21, PIN23);
  SerialComm serial("/dev/ttyACM0");
  SonicSensor soundSensor(PIN16, PIN18);

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
      else if (buffer.compare("CLIENT_READ_SENSOR")==0)
	{
	  print_warning("Reading from sensor!!!");
	  soundSensor.Pulse();
	  auto dist = soundSensor.ReadDistance();
	  std::string dist_str = std::to_string(dist);
	  std::cout << "TEST : " << dist_str << std::endl;
	  net.WriteText(dist_str);

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
	      motorA.Start();
	      motorB.Start();
	      rob_sleep(100);
	      motorA.Stop();
	      motorB.Stop();
	      rob_sleep(10);

	      soundSensor.Pulse();
	      dist = soundSensor.ReadDistance();
	    }

	  
	}

      // ============================= MOTOR A ====================================
      else if(buffer.compare("CLIENT_MOTORA_START")==0)
	{
	  // Start motor
	  print_msg("STARTING MOTOR A");
	  motorA.Start();
	}
      else if(buffer.compare("CLIENT_MOTORA_STOP")==0)
	{
	  // Stop motor
	  motorA.Stop();
	}
      else if (buffer.compare("CLIENT_READ_SENSOR")==0)
	{
	  // Ask arduino to print out reading
	  serial.Write("READ_SENSOR");
	  std::string test = serial.Read("");
	  print_warning(test);
	}
      else if(buffer.compare("CLIENT_MOTORA_FORWARD")==0)
	{
	  motorA.setDirection(0);
	}
      else if (buffer.compare("CLIENT_MOTORA_BACKWARD")==0)
	{
	  motorA.setDirection(1);
	}
      // ============================ MOTOR B ===============================================
      else if (buffer.compare("CLIENT_MOTORB_START")==0)
	{
	  print_msg("Motor B starting");
	  motorB.Start();
	}
      else if (buffer.compare("CLIENT_MOTORB_STOP")==0)
	{
	  print_msg("Motor B stopping");
	  motorB.Stop();
	}
      // ============================== Both motors ======================================
      else if (buffer.compare("CLIENT_MOTORS_START")==0)
	{
	  print_msg("Starting both motors..");
	  motorA.Start();
	  motorB.Start();
	}
      else if (buffer.compare("CLIENT_MOTORS_STOP")==0)
	{
	  print_msg("Stopping both motors..");
	  motorA.Stop();
	  motorB.Stop();
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
	  const auto sleep_time = 95;
	  auto repeats = 3;

	  print_msg("Turning left");
	  // Save state of motors
	  auto mota_dir = motorA.getDirection();
	  auto motb_dir = motorB.getDirection();
	  // Change direcions of motors
	  motorA.setDirection(1);
	  motorB.setDirection(0);
	  
	  while (repeats > 0)
	    {
	      // Fire them up!
	      motorA.Start();
	      motorB.Start();
	      // Wait x time, then halt them!
	      rob_sleep(sleep_time);
	      motorA.Stop();
	      motorB.Stop();
	      rob_sleep(sleep_time);
	      repeats--;
	    }

	  // Set motor state back to origin
	  motorA.setDirection(mota_dir);
	  motorB.setDirection(motb_dir);
	}
      else if (buffer.compare("CLIENT_MOTORS_TURNRIGHT")==0)
	{
	  const auto sleep_time = 95;
	  auto repeats = 3;
	  print_msg("Turning right");
	  // Save state of motors
	  auto mota_dir = motorA.getDirection();
	  auto motb_dir = motorB.getDirection();
	  // Change directions
	  motorA.setDirection(0);
	  motorB.setDirection(1);
	  // Fire them up
	  while ( repeats > 0)
	    {
	      motorA.Start();
	      motorB.Start();
	      // Wait x ms and halt
	      rob_sleep(sleep_time);
	      motorA.Stop();
	      motorB.Stop();
	      rob_sleep(sleep_time);
	      repeats--;
	    }

	  // Set motor stae back to origin
	  motorA.setDirection(mota_dir);
	  motorB.setDirection(motb_dir);
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
