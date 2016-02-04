#ifndef SonicSensor__HPP
#define SonicSensor__HPP
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include "./naviberryio.hpp"
#include "./sonicsensor.hpp"
#include "./bcmwrapper.hpp"

extern bool debugFlag;

/**
  * @name SonicSensor
  * @brief Constructs the SonicSensor class
  * @param _trig The trigger pin
  * @param _echo The echo pin
  * @retval None
  *
  *
  * Constructs and initializes the SonicSensor class. 
  * When done, it sets the ready flag.
  *
  **/
SonicSensor::SonicSensor(uint8_t _trig, uint8_t _echo)
{
  // Set class pins
  PIN_TRIG = _trig;
  PIN_ECHO = _echo;

  // Set gpio pins 
  GPIO_setup(PIN_TRIG, GPIO_OUT);
  GPIO_setup(PIN_ECHO, GPIO_IN);

  // No bad reads yet
  bad_read = false;

  // Set we are ready
  ready = true;
}


/**
  * @name ~sonicSensor
  * @brief Deconstructs the class
  * @retval None
  *
  *
  * Cleans up the class. Set all outputs to low. 
  * Stop thread if it is running.
  **/
SonicSensor::~SonicSensor()
{
  // set all outputs to low
  GPIO_out(PIN_TRIG, LOW);

  // if thread is running, ask it to stop
  if (threadRunning)
    {
      threadRunning = false;
    }
}

/**
  * @name AutoLoop
  * @brief Loop the sensor readings
  * @retval None
  *
  *
  * Spawns a thread which will continue to loop the sensor readings.
  * The loop can be stopped by call AutoLoopStop .
  * Reading is stored inside class and can be fetched from the outside by call
  * getReading() .
  **/
void SonicSensor::AutoLoop()
{
  std::thread sensorThread (&SonicSensor::threadFuncLoop, this);
  
  sensorThread.detach();
}


/**
  * @name AutoLoopStop
  * @brief Stops the loop of the sensor readings.
  * @retval None
  *
  *
  * Stops the thread which runs the reading loop of the sensor.
  **/
void SonicSensor::AutoLoopStop()
{
  threadRunning = false;
}

/**
  * @name threadFuncLoop
  * @brief Internal function used by thread
  * @retval None
  *
  *
  * This function is called by a thread spawned in AutoLoop . The function will continue to 
  * fetch the sensor readings.
  *
  **/
void SonicSensor::threadFuncLoop()
{
  const auto freq = 500;
  
  // Make sure threadRunning is setted
  threadRunning = true;

  while (threadRunning)
    {
      auto dist = this->ReadDistance();
      if (dist == -1)
	{
	  // Error reading
	}
      else
	{
	  // Success
	}

      lastReading = dist;

      std::this_thread::sleep_for(std::chrono::milliseconds(freq));
    }
}


/**
 * @name microS_delay
 * @brief Sleep for X micro second(s).
 * @param x Value in micro second
 * @retval None
 *
 *
 * Just small function for sleep X micro second.
 **/
void SonicSensor::microS_delay(int x)
{
  std::this_thread::sleep_for(std::chrono::microseconds(x));
}

/**
 * @name Pulse
 * @brief Sends a short pulse on the trigger pin.
 * @retval None
 *
 *
 * Sends out a short pulse ~ 20 micro second. 
 **/
void SonicSensor::Pulse()
{
  // Make sure trigger is low
  GPIO_out(PIN_TRIG, LOW);
  // Wait
  microS_delay(15);
  // Pull it high wait
  GPIO_out(PIN_TRIG, HIGH);
  microS_delay(20);
  // Pull it low after 10~20 micro seconds
  GPIO_out(PIN_TRIG, LOW);
}

/**
 * @name ReadDistance
 * @brief Returns [cm] reading of the distance
 * @retval int Returns the value in [cm].
 * @retval Returns -1 on error
 *
 * This function will automaticially call several triggers and reads.
 * Sample nd filter the result and return it.
 *
 * It will 
 * Pulse, Read, Check for error, Store, Repeat
 *
 * const auto reads times
 **/
int SonicSensor::ReadDistance()
{
  auto bad_reads = 0;
  auto average = 0;
  const auto reads = 5;


  for (auto n = 0; n < reads; n++)
    {
      Pulse();
      
      auto start_total = std::chrono::high_resolution_clock::now();
      // Start
      auto echo_in = LOW;
      
      // Wait for pin to change state
      while (echo_in == LOW)
	{
	  // Read
	  echo_in = GPIO_read(PIN_ECHO);
	}
      
      // Now we can start timing
      auto start_echo = std::chrono::high_resolution_clock::now();
      
      // Wait for pin to change state to low again
      while (echo_in == HIGH)
	{
	  // Read
	  echo_in = GPIO_read(PIN_ECHO);
	}
      // Get time
      auto elapsed_echo = std::chrono::high_resolution_clock::now() - start_echo;
      // auto tdiff = elapsed_echo - start_echo;
      
      // Find difference on timings
      long long time_used = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_echo).count();
      
      // convert time to distance
      // and return distance in cm
      long long distance = time_used/29/2;
      
      
      
      // If bad reading
      // Set bad read flag and return number BELOW zero
      if (distance < 0 || distance > 300)
	{
	  print_warning("Sonic sensor BAD read!");
	  bad_read = true;
	  bad_reads++;
	}
      else
	{
	  if (debugFlag)
	    {
	      // std::cout << "TIME READ : " << time_used << " microSeconds \t distance : " << distance << " cm" << std::endl;
	    
	    }
	  
	  average += (int) distance;
	  
	}
      // First iteration is done,
      // make small pause
      microS_delay(100);
      
    }

  // If more than half of the reads are bad_reads, consider the whole reading as bad
  // return -1;
  if ((bad_reads > (reads/2)))
    {
      return -1;
    }
  else
    {
      return (average / (reads-bad_reads));
    }

}


#endif
