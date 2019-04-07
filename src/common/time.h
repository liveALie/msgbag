#pragma once
#include <time.h>
namespace nullmax {
namespace msgbag {
struct Time {
  Time() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    sec = ts.tv_sec;
    nsec = ts.tv_nsec;
  }
  time_t sec;
  long nsec;
};
} // namespace msgbag
} // namespace nullmax
