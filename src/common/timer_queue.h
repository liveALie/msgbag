#pragma once
#include "event_loop.h"
#include "timer.h"
#include "timerid.h"
#include "timestamp.h"
#include <set>
#include <sys/timerfd.h>
#include <vector>

namespace nullmax {
namespace msgbag {

class TimerQueue {
  friend class EventLoop;
  typedef std::pair<Timestamp, std::shared_ptr<Timer>> Entry;
  typedef std::set<Entry> TimerList;
  typedef std::pair<std::shared_ptr<Timer>, int64_t> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;

public:
  TimerQueue();
  TimerQueue(const TimerQueue &) = delete;
  TimerQueue &operator=(const TimerQueue &right) = delete;
  ~TimerQueue();

  void AddTimer(const TimerCallback &cb, Timestamp when, double interval = 0);
  void CancelTimer(const TimerId &timer_id);
  void Run();
  void Stop();
  void Reset(const std::vector<Entry> &expired, Timestamp now);

private:
  void HandleEvent();
  bool Insert(std::shared_ptr<Timer> timer);
  std::vector<Entry> GetExpired(Timestamp now);

private:
  std::shared_ptr<EventLoop> event_loop_;
  const int timerfd_;
  TimerList timers_;
  // ActiveTimer active_timers_;
  bool calling_expeired_timers_;
  ActiveTimerSet canceling_timers_;
};
} // namespace msgbag
} // namespace nullmax