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
#include <thread>
#include "motordriver.hpp"
#include "naviberryio.hpp"
#include "bcmwrapper.hpp"

// GPIO access, uses C library
extern "C"
{
#include "bcm2835.h"
}


// This piece of code shold only be run as a tread
// hence the constant looping and sleep calls
// The total time T should be near 20 milli seconds. ~ 50 Hz
// 2ms / 20 ms  ~~   10%
// 4ms / 20 ms  ~~   20%
// 6ms / 20 ms  ~~   30%
// 86ms / 20 ms  ~~  40%
// 10ms / 20 ms  ~~  50%
// 12ms / 20 ms  ~~  60%
// 14ms / 20 ms  ~~  70%
// 16ms / 20 ms  ~~  80%
// 18ms / 20 ms  ~~  90%
// 20ms / 20 ms  ~~ 100%
void DC_Motor::PWM()
{
  auto pwm_low_duration = 20 - pwm_high_duration;
  
  
  while (pwm_on)
    {
      GPIO_out(pin_e, HIGH);
      std::this_thread::sleep_for(std::chrono::milliseconds(pwm_high_duration));
      GPIO_out(pin_e, LOW);
      std::this_thread::sleep_for(std::chrono::milliseconds(pwm_low_duration));
    }
}
0

// Starts the PMW function on the motor
// The _val argument should be a number between 1 and 100.
void DC_Motor::StartPWM(int _val)
{
  if (_val < 0)
  {
    _val = 0;  
  }
  else if (_val > 100)
  {
    _val = 100;
  }
  
  auto high_sleep_duration_ms = 0;
  
  if ((_val >= 0) && (_val < 10))
  {
    high_sleep_duration_ms = 2;
  }
  else if ((_val >= 10) && (_val < 20))
  {
    high_sleep_duration_ms = 4;
  }
  else if ((_val >= 20) && (_val < 30))
  {
    high_sleep_duration_ms = 6;
  }
  else if ((_val >= 30) && (_val < 40))
  {
    high_sleep_duration_ms = 8;
  }
  else if ((_val >= 40) && (_val < 50))
  {
    high_sleep_duration_ms = 10;
  }
  else if ((_val >= 50) && (_val < 60))
  {
    high_sleep_duration_ms = 12;
  }
  else if ((_val >= 60) && (_val < 70))
  {
    high_sleep_duration_ms = 14;
  }
  else if ((_val >= 70) && (_val < 80))
  {
    high_sleep_duration_ms = 16;
  }
  else if ((_val >= 80) && (_val < 90))
  {
    high_sleep_duration_ms = 18;
  }
  else if ((_val >= 90) && (_val <= 100))
  {
    high_sleep_duration_ms = 20;
  }
  else if ((_val >= 10) && (_val < 20))
  {
    high_sleep_duration_ms = 20;
  }
  
  
  // set to thread variable
  pwm_high_duration = high_slee_duration;
  
  // start pwm loop in seperated thread
  std::thread pwmThread (&DC_Motor::PWM, this);
  // set the thread variable
  pwm_on = true;
}

void DC_Motor::StopPWM()
{
  pwm_on = false;
}

// DC_Motor Start void function
// Starts the motor, by setting PIN_E to high
void DC_Motor::Start()
{
  running = 1;
  print_msg("Motor starting");
  GPIO_out(pin_e, HIGH);
}

// DC_Motor Stop void function
// Stops the motor, by setting PIN_E to low
void DC_Motor::Stop()
{
  running = 0;
  print_msg("Motor stopping");
  GPIO_out(pin_e, LOW);
}
// DC_Motor getDirection integer function
// Takes no inputs
// Return 0 for clockwise, 1 for counter-clockwise
uint8_t DC_Motor::getDirection()
{
  return direction;
}


// DC_Motor setDirection void function
// Takes input as direction
// 0 is clockwise
// 1 is counter-clockwise
void DC_Motor::setDirection(uint8_t _direct)
{
  // Save current motor state
  auto motor_on = running;

  // If motor is on, halt it
  
  if (motor_on != 0)
    {
      print_msg("Motor running, halt it");
      Stop();
    }

  // Select direction
  switch(_direct)
    {
    case 0:
      // Right
      // Set logic
      print_msg("Setting motor direction to right");
      GPIO_out(pin_l, LOW);
      GPIO_out(pin_r, HIGH);
      // Set class variable
      direction = _direct;
      break;
    case 1:
      // Left
      // Set logic
      print_msg("Setting motor direction to left");
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
      print_msg("Switching done, starting motor..");
      Start();
    }
}

// DC_Motor DC_Motor De-constructor function
// When class is about to be cleaned, this function is called
// So we clean, set all pins used to low
DC_Motor::~DC_Motor(void)
{
  print_msg("De-constructor called on DC_Motor");
  print_msg("Setting all motor inputs to low..");
  GPIO_out(pin_r, LOW);
  GPIO_out(pin_l, LOW);
  GPIO_out(pin_e, LOW);
}

// DC_Motor DC_Motor constructor function
// Takes 3 integer inputs
// pin_e = Enable PIN
// pin_r = Logic pin1
// pin_l = Logic pin2
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
