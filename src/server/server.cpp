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
#include <thread>
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



static std::string current_distance_reading_string = "";
int32_t current_distance = -1;

extern bool debugFlag;

// Atomic booleans for keeping the threads running, or closing them.
std::atomic_bool sensorThreadRun;
std::atomic_bool mapmodeThreadRun;

// change please!!!!!!!!!!!!!
void rob_sleep(int x)
{
  if (debugFlag)
    print_msg("Sleep called");

  std::this_thread::sleep_for(std::chrono::milliseconds(x));
}


void signal_callback_handle(int _signalnum)
{  

  if ( _signalnum == 2)
    {
      print_error("Interrupt signal recieved, CTRL-C pressed, closing down..");
    }
}

Direction UpdateDirectionByTurn(Direction _currentDirection, Direction _turn)
{
  switch (_turn)
    {
    case Direction::Right:
      switch(_currentDirection)
	{
	case Direction::Up:
	  _currentDirection = Direction::Right;
	  break;
	case Direction::Right:
	  _currentDirection = Direction::Down;
	  break;
	case Direction::Down:
	  _currentDirection = Direction::Left;
	  break;
	case Direction::Left:
	  _currentDirection = Direction::Up;
	  break;
	}
      break;
    case Direction::Left:
      switch(_currentDirection)
	{
	case Direction::Up:
	  _currentDirection = Direction::Left;
	  break;
	case Direction::Left:
	  _currentDirection = Direction::Down;
	  break;
	case Direction::Down:
	  _currentDirection = Direction::Right;
	  break;
	case Direction::Right:
	  _currentDirection = Direction::Up;
	    break;
	}
      break;
    }

  return _currentDirection;
}

Point IncreasePointByDirection(Point _pt, Direction _dir)
{
  switch(_dir)
    {
    case Direction::Up:
      _pt.y--;
      break;
    case Direction::Down:
      _pt.y++;
      break;
    case Direction::Right:
      _pt.x++;
      break;
    case Direction::Left:
      _pt.x--;
      break;
    }

  return _pt;
}

void mapmodeFunc(MapHandler* maphandler)
{
  // setup, initialize
  mapmodeThreadRun = true;
  const uint16_t freq = 5000;
  MachineState state = MachineState::Standby;
  Direction direction = Direction::Up;

  print_msg("Map mode thread, started and running");


  while (mapmodeThreadRun)
    {
      print_msg("Map mode..");
      
      switch (direction)
	{
	case Direction::Up:
	  print_msg("Direction facing up");
	  break;
	case Direction::Down:
	  print_msg("Direction facing down");
	  break;
	case Direction::Right:
	  print_msg("Direction facing right");
	  break;
	case Direction::Left:
	  print_msg("Direction facing left");
	  break;
	}

      Point p = maphandler->getPosition();
      std::cout << "Position : [ " << p.x << " , " << p.y << " ]"<< std::endl;
      
      // Check distance:
      // if distance greater than 20
      //     Drive forward
      //     goto Check distance
      // else
      //     update map
      //     turn left OR turn right
      //     goto Check distance

      // Do the logic for setting the current state
      auto distance = current_distance;
      state = MachineState::Standby;

      if (distance > 20)
	{
	  // update position
	  Point pos = maphandler->getPosition();
	  std::cout << pos.x << "\t" << pos.y << std::endl;
	  pos = IncreasePointByDirection(pos, direction);
	  std::cout << pos.x << "\t" << pos.y << std::endl;
	  maphandler->setPosition(pos);
	  
	  // set mapp as free
	  maphandler->setTile(maphandler->getPosition(), TileType::Free);

	  // Set state
	  state = MachineState::Forward;
	  
	}
      else if(distance < 20 && distance > 3)
	{
	  // Set map+1 as block
	  Point pos = maphandler->getPosition();
	  std::cout << pos.x << "\t" << pos.y << std::endl;
	  pos = IncreasePointByDirection(pos, direction);
	  std::cout << pos.x << "\t" << pos.y << std::endl;

	  // dont update position to handler
	  maphandler->setTile(pos, TileType::Block);
	  
	  // Set state
	  state = MachineState::TurnRight;
	}
      else
	{
	  // Abort, failure has happened halt everything
	}


      // Handle the current state
      switch (state)
	{
	case MachineState::Standby:
	  // Do nothing
	  print_msg("Standing by..");
	  break;
	case MachineState::Forward:
	  // Drive forward
	  print_msg("Driving forward");
	  break;
	case MachineState::TurnRight:
	  // Turn right
	  direction = UpdateDirectionByTurn(direction, Direction::Right);
	  print_msg("Turning right");
	  break;

	}


      rob_sleep(freq);
   }
}

void senFunc(SonicSensor& _sensor)
{
  print_msg("sensor thread started");
  const uint16_t freq = 500; //  ms update frequency
  sensorThreadRun = true;
  while (sensorThreadRun)
    {
      auto dist = _sensor.ReadDistance();
      if (dist == -1)
	{
	  // Error reading
	}
      else
	{
	  // Sucess reading
	}

      // Set to tring
      current_distance = dist;
      current_distance_reading_string = std::to_string(dist);

      // sleep
      rob_sleep(freq);
    }

  print_warning("Sensor thread stopped");
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

  // We take care of the signals
  signal(SIGINT, signal_callback_handle);



  // Initialize hardware
  DC_Motor motorA(PIN11, PIN13, PIN15);
  DC_Motor motorB(PIN19, PIN21, PIN23);
  SonicSensor soundSensor(PIN16, PIN18);

  MotorController controller(&motorA, &motorB);



  // Initialize networking
  Network net("localhost", 1000);

  // Initialize atomic variables
  sensorThreadRun = false;
  mapmodeThreadRun = false;


  // Scheduler
  Scheduler sched;
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
  std::string dist_reading = "-1";
  auto dist_lastRead = 0 ;

  // The main threads
  std::thread senThread (senFunc, std::ref(soundSensor));


  while (prog_running)
    {
      sched.Update();

      if (sched.getNetworkFlag())
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
		      net.WriteText(comm_REPLY_DISCONNECT);
		      prog_running = false;
		      break;
		    }	  
		  else if (buffer.compare("CLIENT_READ_SENSORS")==0)
		    {
		      std::cout << "Last distance measurement : " << current_distance_reading_string << std::endl;	      
		      uint8_t* rawBytes = new uint8_t[sizeof(current_distance)];
		      std::memset(&rawBytes[0], 0, sizeof(current_distance));
		      std::memcpy(&rawBytes[0], &current_distance, sizeof(current_distance));
		      net.WriteData(rawBytes, sizeof(dist_lastRead), SENSOR_TYPE);
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
		       //		       motorA.StopPWM();
		       //                      motorB.StopPWM();
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
		       // 
		     }
		   else if (buffer.compare("CLIENT_MOTORS_TURNRIGHT")==0)
		     {
		       // 
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
		       if (mapmodeThreadRun.load() == false)
			 {
			   print_warning("Starting map mode");
			   std::thread mapmodeThread (mapmodeFunc, &mapHandler);
			   mapmodeThread.detach();
			 }
		     }
		   else if(buffer.compare("CLIENT_MAP_MODE_STOP")==0)
		     {
		       mapmodeThreadRun.store(false);
		     }
		   else
		     {
		       print_warning("Unknown message recived :");
		       print_warning(buffer);    
		     }
		}
	      

	      if (debugFlag)
		print_msg("Clearing text packet queue");
	      
	      // We processed all element, now clear queue
	      net.clearTextQueue();
	      
	    }  
	  
	  // Reset flag
	  sched.resetNetworkFlag();
	}     
    }
  


  // Exit threads
  sensorThreadRun = false;
  senThread.join();

  mapmodeThreadRun = false;


  
  // just exit
  return EXIT_SUCCESS;
}
