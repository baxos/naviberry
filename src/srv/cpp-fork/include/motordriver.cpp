/*##################################################################
  # motordriver.cpp                                                #
  #                                                                #
  # Motor driver class for controlling h-bridge for a DC motor
  #
  #                    
  # Written by J.E.B @  2015                                      #
  #################################################################
*/

#include <cstdint>
#include <iostream>
#include "motordriver.hpp"
#include "naviberryio.hpp"
#include "bcmwrapper.hpp"

// GPIO access, uses C library
extern "C"
{
#include "bcm2835.h"
}


void DC_Motor::Start()
{
  print_msg("Motor starting");
  GPIO_out(pin_e, HIGH);
}

void DC_Motor::Stop()
{
  print_msg("Motor stopping");
  GPIO_out(pin_e, LOW);
}

void DC_Motor::setDirection(uint8_t _direct)
{
  // Save current motor state
  auto motor_on = running;

  // If motor is on, halt it
  if (motor_on != 0)
    {
      Stop();
    }

  // Select direction
  switch(_direct)
    {
    case 0:
      // Right
      // Set logic
      GPIO_out(pin_l, LOW);
      GPIO_out(pin_r, HIGH);
      // Set class variable
      direction = _direct;
      break;
    case 1:
      // Left
      // Set logic
      GPIO_out(pin_r, LOW);
      GPIO_out(pin_l, HIGH);
      // Set class variable
      direction = _direct;
      break;
    default:
      // Do nothing
      break;
    }

  // If motor was running, start it again
  if (motor_on)
    {
      Start();
    }
}

DC_Motor::~DC_Motor(void)
{
  print_msg("De-constructor called on DC_Motor");
  print_msg("Setting all motor inputs to low..");
  GPIO_out(pin_r, LOW);
  GPIO_out(pin_l, LOW);
  GPIO_out(pin_e, LOW);
}

DC_Motor::DC_Motor(uint8_t _pin_e, uint8_t _pin_l, uint8_t _pin_r)
{
  print_msg("Motor initialization");
  print_msg("Setting class variables..");
  pin_r = _pin_r;
  pin_l = _pin_l;
  pin_e = _pin_e;

  print_msg("Setting pins as outputs");
  GPIO_setup(pin_r, GPIO_OUT);
  GPIO_setup(pin_l, GPIO_OUT);
  GPIO_setup(pin_e, GPIO_OUT);

  print_msg("Setting all pins low");
  GPIO_out(pin_r, LOW);
  GPIO_out(pin_l, LOW);
  GPIO_out(pin_e, LOW);

  print_msg("Setting motor state to off");
  running = 0;

  print_msg("Setting direction");
  setDirection(0);

  print_msg("Motor initialized");
}
