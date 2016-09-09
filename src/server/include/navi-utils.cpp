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


  template <class T> AdvancedCountdownTriggerer<T>::AdvancedCountdownTriggerer()
  {
    // not ready before init has been run.
    this->ready = false;
    this->running = false;
  }

  template <class T> void AdvancedCountdownTriggerer<T>::Init(int32_t time, T *_trigger, T _triggerVal)
  {
    this->trigger_var = _trigger;
    this->result_var = _triggerVal;
    this->time_left = time;
    this->ready = true;
  }

  template <class T> void AdvancedCountdownTriggerer<T>::Start()
  {
    this->running = true;
  }

  template <class T> void AdvancedCountdownTriggerer<T>::Stop()
  {
    this->running = false;
  }

  template <class T> void AdvancedCountdownTriggerer<T>::internal_thread_function()
  {
    while (this->running)
      {
	if (this->time_left <= 0)
	  {
	    // Time exceeded and thread is still running
	    // set trigger variable
	    trigger_var = result_var;

	    // stop thread
	    this->running = false;
	    break;
	  }
      }
  }
  
}
