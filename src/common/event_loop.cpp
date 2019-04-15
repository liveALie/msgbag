#include "event_loop.h"
#include "common/util.h"
#include "log/logger.h"
#include "timer_queue.h"
#include <unistd.h>

namespace nullmax {
namespace msgbag {
EventLoop::EventLoop(TimerQueue *timer_queue)
    : timer_queue_(timer_queue), running_(false) {}

EventLoop::~EventLoop() {}

void EventLoop::Loop() {
  if (!running_) {
    Timestamp now = Timestamp::Now();
    ResetTimerfd(timer_queue_->timerfd_, now);
    running_ = true;
  }
  uint64_t howmany = 0;
  while (running_) {
    ssize_t n = ::read(timer_queue_->timerfd_, &howmany, sizeof howmany);
    Timestamp now = Timestamp::Now();
    LOG_DEBUG("TimerQueue::handleRead() {} at {}", howmany,
              now.NanoSecondsSinceEpoch());
    if (n != sizeof howmany) {
      LOG_ERROR("TimerQueue::handleRead() reads {} bytes instead of 8", n);
      running_ = false;
      break;
    }
    if (callback_)
      callback_();
  }
}

void EventLoop::SetTimerCallback(const TimerCallback &callback) {
  callback_ = callback;
}
} // namespace msgbag
} // namespace nullmax