#ifndef NAVIUTILS__HPP
#define NAVIUTILS__HPP

#include <cstdint>
#include <cstddef>
#include <string>
#include <chrono>
#include <cstring>
#include <thread>

 
class StopWatch
{
private:
  std::chrono::time_point<std::chrono::system_clock>  start_time;
  std::chrono::time_point<std::chrono::system_clock>  end_time;
public:
  void Start();
  void Stop();
    int getTimeElapsed();
};


class CountdownTimer
{
private:
  
public:
  CountdownTimer();
};


  
template <class T> 
class AdvCountdown
{
private:
  T *trigger_var;
  T result_var;
  bool ready;
  bool running;
  int32_t time_left;
  void internal_thread_function();
public:
  AdvCountdown();
  void Init(int32_t time, T *_trigger, T _triggerVal);
  void Start();
  void Stop();
};

#endif
