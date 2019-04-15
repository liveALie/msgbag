#pragma once

#include "common/msgbag_types.h"
#include "common/timestamp.h"
#include "timestamp.h"
#include <atomic>
#include <stdint.h>

namespace nullmax {
namespace msgbag {
///
/// Internal class for timer event.
///
class Timer {
public:
  Timer(const TimerCallback &cb, Timestamp when, double interval)
      : callback_(cb), expiration_(when), interval_(interval),
        repeat_(interval > 0.0), sequence_(++s_num_created_) {}

  Timer(TimerCallback &&cb, Timestamp when, double interval)
      : callback_(std::move(cb)), expiration_(when), interval_(interval),
        repeat_(interval > 0.0), sequence_(++s_num_created_) {}

  void Run() const { callback_(); }

  Timestamp Expiration() const { return expiration_; }
  bool Repeat() const { return repeat_; }
  int64_t Sequence() const { return sequence_; }

  void Restart(Timestamp now);

  static int64_t NumCreated() { return s_num_created_; }

private:
  // noncopyable
  Timer(const Timer &rhs) = delete;
  Timer &operator=(const Timer &rhs) = delete;

private:
  const TimerCallback callback_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
  const int64_t sequence_;

  static std::atomic<int64_t> s_num_created_;
};
}
}
