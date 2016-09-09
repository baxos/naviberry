#ifndef SONICSENSOR__HPP
#define SONICSENSOR__HPP

/**
  * @file sonicsensor.hpp
  * @author Jan Emil Bacher
  * @date 2015-2016
  * @brief Class for sonic sensor module
  **/
#include <atomic>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include "./navi-utils.hpp"

/**
  * @class SonicSensor
  * @brief Used for controlling a sonic sensor module.
  *
  *
  * This class is used to control a sonic sensor module. The class can set to loop the readings or 
  * can be set to do manual readings.
  **/

class SonicSensor
{
private:
  uint8_t PIN_TRIG;
  uint8_t PIN_ECHO;
  bool ready;
  bool threadRunning;
  bool bad_read;
  Naviberry::StopWatch internal_sw;
  std::atomic<bool> failure_flag;
  int lastReading;
  void microS_delay(int x);
  void threadFuncLoop();
public:
  SonicSensor(uint8_t _trig, uint8_t _echo);
  ~SonicSensor();
  void Pulse();
  void AutoLoop();
  void AutoLoopStop();
  void Inject(int32_t _val);
  int ReadDistance();
  int32_t getReading();
};


#endif
