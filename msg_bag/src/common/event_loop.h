#pragma once

#include "common/msgbag_constants.h"
#include "common/msgbag_types.h"
#include <atomic>
#include <vector>

namespace nullmax {
namespace msgbag {

class TimerQueue;

class EventLoop {
public:
  explicit EventLoop(TimerQueue *timer_queue);
  ~EventLoop();
  EventLoop(const EventLoop &) = delete;
  EventLoop &operator=(const EventLoop &) = delete;
  void Loop();
  void SetTimerCallback(const TimerCallback &callback);

private:
  TimerQueue *timer_queue_;
  TimerCallback callback_;
  std::atomic<bool> running_;
};
}
}