#ifndef SCHEDULER__HPP
#define SCHEDULER__HPP

#include <iostream>
#include <cstdint>

class Scheduler
{
private:
  // For time keeping
  bool firstRun;
  long ms_last_run;
  long getmsecNow();
  // Time remembering
  long total_msec;
  long total_sec;
  // Time counting
  int16_t count_msec;

  // For scheduling
  // Counters 
  static const int16_t hardwareCounterMax = 100;
  static const int16_t sensorCounterMax = 200;
  static const int16_t networkCounterMax = 300;
  int16_t hardwareCounter;
  int16_t sensorCounter;
  int16_t networkCounter;
  // Flags
  bool hardwareFlag;
  bool sensorFlag;
  bool networkFlag;

public:
  // Constructor and Destructor
  Scheduler();
  ~Scheduler();

  // Get flag status
  bool getHardwareFlag() { return hardwareFlag; };
  bool getSensorFlag() { return sensorFlag; };
  bool getNetworkFlag() { return networkFlag; };

  // Reset flag to false and reset counters to counter max
  void resetHardwareFlag() { hardwareCounter=hardwareCounterMax; hardwareFlag = false; };
  void resetSensorFlag() { sensorCounter=sensorCounterMax; sensorFlag = false; };
  void resetNetworkFlag() { networkCounter=networkCounterMax; networkFlag = false; };

  // Update reference and update scheduler
  void Update();
};


#endif
