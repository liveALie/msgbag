#include "timer.h"

namespace nullmax {
namespace msgbag {
std::atomic<int64_t> Timer::s_num_created_;

void Timer::Restart(Timestamp now) {
  if (repeat_) {
    expiration_ = AddTime(now, interval_);
  } else {
    expiration_ = Timestamp::Invalid();
  }
}
}
}
