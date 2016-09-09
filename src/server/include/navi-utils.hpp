#include <chrono>


namespace Naviberry
{

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


}
