#include "timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdio.h>

namespace nullmax {
namespace msgbag {
static_assert(sizeof(Timestamp) == sizeof(int64_t), "sizeof(Timestamp) error");

Timestamp::Timestamp(int64_t nano_seconds) : nano_seconds_(nano_seconds) {}

std::string Timestamp::ToString() const {
  char buf[32] = {0};
  int64_t seconds = nano_seconds_ / NANO_SECONDS_PER_SECOND;
  int64_t nanoseconds = nano_seconds_ % NANO_SECONDS_PER_SECOND;
  snprintf(buf, sizeof(buf) - 1, "%ld.%09ld", seconds, (long)nanoseconds);
  return buf;
}

std::string Timestamp::ToFormattedString(bool show_nanosec) const {
  time_t seconds = static_cast<time_t>(nano_seconds_ / NANO_SECONDS_PER_SECOND);
  struct tm tm_time;

  struct tm *ptm;
  ptm = localtime(&seconds);
  tm_time = *ptm;
  // ss << std::put_time(&ptm, "%F %T");
  //<<		((double)(nano_seconds_ % NANO_SECONDS_PER_SECOND)) /
  // NANO_SECONDS_PER_SECOND;

  char buf[32] = {0};

  if (show_nanosec) {
    int nanoseconds = static_cast<int>(nano_seconds_ % NANO_SECONDS_PER_SECOND);
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d-%02d-%02d-%02d.%09ld",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             (long)nanoseconds);
  } else {
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d-%02d-%02d-%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }

  return buf;
}

Timestamp Timestamp::Now() {
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
      now = std::chrono::time_point_cast<std::chrono::nanoseconds>(
          std::chrono::system_clock::now());

  int64_t nanoseconds = now.time_since_epoch().count();
  Timestamp time(nanoseconds);
  return time;
}

Timestamp Timestamp::Invalid() { return Timestamp(); }
} // namespace msgbag
} // namespace nullmax
