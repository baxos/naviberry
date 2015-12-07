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

void SonicSensor::microS_delay(int x)
{
  std::this_thread::sleep_for(std::chrono::microseconds(x));
}

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
	  bad_read = false;

	  average += (int) distance;

	}
    }

  if ((bad_reads > (reads/2)))
    {
      return -1;
    }
  else
    {
      return average / (reads-bad_reads);
    }

}


#endif
