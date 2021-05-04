#ifndef TIMER_DELAY_HPP
#define TIMER_DELAY_HPP

#include <chrono>

/**
 * Class delaying its destruction for the remainder of the given time duration,
 * starting from its construction.
 */
class TimerDelay
{
private:
  using Duration = std::chrono::duration<double>;
  using Clock = std::chrono::steady_clock;

  Duration m_duration;
  std::chrono::time_point<Clock, Duration> m_end_time_point;

public:
  /**
   * Factories for delays in Hz or seconds
   */
  static TimerDelay hz(double hz);
  static TimerDelay sec(double sec);

  /**
   * Reset the starting time to "now"
   */
  void reset();

  /**
   * Wait for the delay to expire
   */
  void wait();

  /**
   * Calls `wait()`
   */
  ~TimerDelay();

private:
  TimerDelay(const Duration& duration);
};

#endif /* ifndef TIMER_DELAY_HPP */
