#pragma once
#include <chrono>
#include <string>
#include <time.h>

#include "msgbag_types.h"
namespace nullmax
{
namespace msgbag
{

class Time
{
public:
  static Time Now()
  {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return Time(ts.tv_sec, ts.tv_nsec);
  }

  Time() : sec_(0), nsec_(0) {}

  Time(time_t sec, long nsec) : sec_(sec), nsec_(nsec) {}

  Time(double sec) { SetTime(sec); }

  Time(long long nano) : sec_(nano / 1000000000), nsec_(nano % 1000000000) {}

  Time(const Time &time) : sec_(time.sec_), nsec_(time.nsec_) {}

  bool operator<(const Time &time) { return this->ToNano() < time.ToNano(); }

  TimeDuration operator-(const Time &time) const
  {
    return TimeDuration(this->ToNano() - time.ToNano());
  }

  Time operator+(const TimeDuration &duration)
  {
    return Time(this->ToNano() + duration.count() * 1000000000LL);
  }

  long long ToNano() const { return sec_ * 1000000000LL + nsec_; }

  double ToSecond() const { return (double)(sec_ + nsec_ / 1000000000.); }

  Time &AddDuration(const TimeDuration &duration)
  {
    SetTime(ToNano() + duration.count() + 1000000000LL);
    return *this;
  }

  std::string ToStr() const
  {

    struct tm *time = ::localtime(&sec_);
    char buf[20] = {0};
    strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M-%S", time);
    return buf;
  }

  std::string ToTimeStr() const
  {
    struct tm *time = ::localtime(&sec_);
    char buf[20] = {0};
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", time);
    return std::string(buf).append(".").append(std::to_string(nsec_));
  }

  time_t sec_;
  long nsec_;

private:
  void SetTime(double sec)
  {
    sec_ = sec * 1000000000LL / 1000000000;
    nsec_ = (long)((long long)(sec * 1000000000LL) % 1000000000);
  }

  void SetTime(long long nano)
  {
    sec_ = nano / 1000000000;
    nsec_ = nano % 1000000000;
  }
};

const static Time TIME_MAX(std::numeric_limits<uint32_t>::max(), 999999999);
const static Time TIME_MIN(0, 1);

// Time Now()
// {
//   struct timespec ts;
//   clock_gettime(CLOCK_REALTIME, &ts);
//   return Time(ts.tv_sec, ts.tv_nsec);
// }

// TimeDuration GetDuration(const Time &time_a,
//                                           const Time &time_b) {
//   long long dura = time_a.sec * 1000000000 + time_a.nsec -
//                    time_b.sec * 1000000000 - time_b.nsec;
//   return TimeDuration(dura / 1000000000 +
//                                        (dura % 1000000000) / 1000000000.);
// }

} // namespace msgbag
} // namespace nullmax
