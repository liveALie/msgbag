#pragma once
#include <stdint.h>
#include <stdio.h>

namespace nullmax {
namespace msgbag {
class Timer;

///
/// An opaque identifier, for canceling Timer.
///
class TimerId {
public:
  TimerId() : timer_(NULL), sequence_(0) {}

  TimerId(Timer *timer, uint64_t seq) : timer_(timer), sequence_(seq) {}

  // default copy-ctor, dtor and assignment are okay

  friend class TimerQueue;

private:
  std::shared_ptr<Timer> timer_;
  uint64_t sequence_;
};
}
}
