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
  bool bad_read;
  void microS_delay(int x);
public:
  SonicSensor(uint8_t _trig, uint8_t _echo);
  void Pulse();
  int ReadDistance();
};
