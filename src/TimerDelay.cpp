#include "TimerDelay.hpp"
#include <chrono>
#include <thread>

TimerDelay::TimerDelay(const Duration& duration)
    : m_duration(duration), m_end_time_point()
{
  reset();
}

TimerDelay TimerDelay::hz(double hz)
{
  return TimerDelay{Duration{1 / hz}};
}

TimerDelay TimerDelay::sec(double sec)
{
  return TimerDelay{Duration{sec}};
}

void TimerDelay::reset()
{
  m_end_time_point = Clock::now() + m_duration;
}

void TimerDelay::wait()
{
  std::this_thread::sleep_until(m_end_time_point);
}

TimerDelay::~TimerDelay()
{
  wait();
}
