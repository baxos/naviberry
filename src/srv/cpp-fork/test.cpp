#include <chrono>
#include <thread>
#include <iostream>
#include "motordriver.hpp"
#include "bcmwrapper.hpp"
#include "naviberryio.hpp"


#define PIN__E RPI_V2_GPIO_P1_11
#define PIN__R RPI_V2_GPIO_P1_13
#define PIN__L RPI_V2_GPIO_P1_15

void navi_sleepMs(int val);

int main()
{
  if (!bcm2835_init())
    return 1;

  DC_Motor motorA(PIN__E, PIN__R, PIN__L);

  motorA.Motor_Start();
  navi_sleepMs(5000);
  motorA.Motor_setDirection(1);
  navi_sleepMs(3000);
  motorA.Motor_Stop();
  
  print_warning("Closing program");

  return 0;
}


void navi_sleepMs(int val)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(val));
}
