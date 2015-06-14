#include <chrono>
#include "./scheduler.hpp"
#include "./naviberryio.hpp"





// =================================== Scheduler Class ================================== //



// Scheduler Constructor function
// Initialize class variables
Scheduler::Scheduler()
{
  print_msg("Scheduler() constructor called");

  // Set counters
  hardwareCounter = hardwareCounterMax;
  sensorCounter   = sensorCounterMax;
  networkCounter  = networkCounterMax;

  // Set flags
  hardwareFlag = false;
  sensorFlag   = false;
  networkFlag  = false;
  firstRun     = true;

  // For time keeping
  total_msec = 0;
  total_sec  = 0;
  count_msec = 0;
}

Scheduler::~Scheduler()
{
  print_msg("~Scheduler() deconstructor called");
}

// Scheduler Update void function
// Update time, counters and flags
void Scheduler::Update()
{
  long ms_now = getmsecNow();

  if (firstRun)
    {
      ms_last_run = ms_now;
      firstRun = false;
    }

  if ( (ms_now - ms_last_run) != 0)
    {
      // We have a minimum tick of 1 
      auto deltaT = ms_now - ms_last_run;

      // Vaildate it's correct measurement(?)
      if (deltaT > 0)
	{
	  
	  // Decrease counters
	  hardwareCounter -= deltaT;
	  sensorCounter   -= deltaT;
	  networkCounter  -= deltaT;

	  // Set flags if needed
	  if (hardwareCounter <= 0)
	    {
	      hardwareFlag = true;
	    }
	  if (sensorCounter <= 0)
	    {
	      sensorFlag = true;
	    }
	  if (networkCounter <= 0)
	    {
	      networkFlag = true;
	    }
	}
    }
  else
    {
      // Do nothing
    }
}

// Scheduler getmsecNow long function
// Returns msec since epoch
long Scheduler::getmsecNow()
{
  std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds> ( std::chrono::high_resolution_clock::now().time_since_epoch());

  return (long) ms.count();
}
