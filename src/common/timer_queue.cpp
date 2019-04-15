#include "timer_queue.h"
#include "common/timer.h"
#include "common/util.h"
#include "log/logger.h"

namespace nullmax {
namespace msgbag {

TimerQueue::TimerQueue()
    : event_loop_(new EventLoop(this)), timerfd_(CreateTimerfd()),
      calling_expeired_timers_(false) {
  event_loop_->SetTimerCallback(std::bind(&TimerQueue::HandleEvent, this));
}

TimerQueue::~TimerQueue() { ::close(timerfd_); }

void TimerQueue::AddTimer(const TimerCallback &cb, Timestamp when,
                          double interval) {
  std::shared_ptr<Timer> timer = std::make_shared<Timer>(cb, when, interval);
  Insert(timer);
}
void TimerQueue::CancelTimer(const TimerId &timer_id) {}

void TimerQueue::Run() { event_loop_->Loop(); }

void TimerQueue::Stop() { close(timerfd_); }

void TimerQueue::HandleEvent() {
  Timestamp now(Timestamp::Now());
  std::vector<Entry> expired = GetExpired(now);
  if (expired.empty()) {
    std::cout << "expired timers is zero." << std::endl;
    Stop();
  }
  calling_expeired_timers_ = true;
  canceling_timers_.clear();
  // std::cout << "handle event expired timers number:" << expired.size();
  for (auto &entry : expired) {
    entry.second->Run();
  }
  calling_expeired_timers_ = false;
  Reset(expired, now);
}

bool TimerQueue::Insert(std::shared_ptr<Timer> timer) {
  Timestamp when = timer->Expiration();
  std::pair<TimerList::iterator, bool> result =
      timers_.insert(Entry(when, timer));
  assert(result.second);
  (void)result;
  return true;
}

std::vector<TimerQueue::Entry> TimerQueue::GetExpired(Timestamp now) {
  std::vector<Entry> expired;
  if (timers_.empty())
    return expired;
  Entry sentry(now, NULL);
  TimerList::iterator end = timers_.lower_bound(sentry);
  assert(end == timers_.end() || now < end->first);
  std::copy(timers_.begin(), end, std::back_inserter(expired));
  timers_.erase(timers_.begin(), end);

  return expired;
}

void TimerQueue::Reset(const std::vector<Entry> &expired, Timestamp now) {
  for (auto &entry : expired) {
    ActiveTimer timer(entry.second, entry.second->Sequence());
    if (entry.second->Repeat() &&
        canceling_timers_.find(timer) == canceling_timers_.end()) {
      entry.second->Restart(now);
      Insert(entry.second);
    }
  }
  Timestamp next_expired;

  if (!timers_.empty()) {
    next_expired = timers_.begin()->second->Expiration();
  } else {
    Stop();
  }

  if (next_expired.Valid()) {
    ResetTimerfd(timerfd_, next_expired);
  }
}
} // namespace msgbag
} // namespace nullmax
