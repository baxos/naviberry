#include <cstdint>
#include <iostream>
#include "motordriver.h"

// GPIO access, uses C library
extern "C"
{
#include "bcm2835.h"
}


void DC_Motor::Motor_Start()
{
  std::cout << "Motor_Start() called";
}

void DC_Motor::Motor_Stop()
{
  std::cout << "Motor_Stop() called";
}

void DC_Motor::Motor_setDirection(uint8_t _direct)
{
  direction = _direct;
}

DC_Motor::DC_Motor(uint8_t _pin_e, uint8_t _pin_l, uint8_t _pin_r)
{
  pin_r = _pin_r;
  pin_l = _pin_l;
  pin_e = _pin_e;
  std::cout << "Class initialized";
}
