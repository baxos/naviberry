#include "./navi-utils.hpp"

namespace Naviberry
{

  // stopwatch member functions

  void StopWatch::Start()
  {
    this->start_time = std::chrono::system_clock::now();
  }

  void StopWatch::Stop()
  {
    this->end_time = std::chrono::system_clock::now();
  }

  int StopWatch::getTimeElapsed()
  {
    auto time_elapsed = end_time - start_time;
    
    auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_elapsed).count();
    return time_ms;
  }



  // count down timer member functions



}
