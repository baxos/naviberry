#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>

class SonicSensor
{
private:
  uint8_t PIN_TRIG;
  uint8_t PIN_ECHO;
  bool ready;
  bool threadRunning;
  bool bad_read;
  int lastReading;
  void microS_delay(int x);
  void threadFuncLoop();
public:
  SonicSensor(uint8_t _trig, uint8_t _echo);
  ~SonicSensor();
  void Pulse();
  void AutoLoop();
  void AutoLoopStop();
  int ReadDistance();
  int getReading() { return lastReading; } ;
};
