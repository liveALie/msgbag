#pragma once

#include <algorithm>
#include <stdint.h>
#include <string>

namespace nullmax {
namespace msgbag {
///
/// Time stamp in UTC, in microseconds resolution.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class Timestamp {
public:
  ///
  /// Constucts an invalid Timestamp.
  ///
  Timestamp() : nano_seconds_(0) {}

  ///
  /// Constucts a Timestamp at specific time
  ///
  /// @param nano_seconds
  explicit Timestamp(int64_t nano_seconds);

  void Swap(Timestamp &that) { std::swap(nano_seconds_, that.nano_seconds_); }

  // default copy/assignment/dtor are Okay

  std::string ToString() const;
  std::string ToFormattedString(bool show_nanosec = true) const;

  bool Valid() const { return nano_seconds_ > 0; }

  // for internal usage.
  int64_t NanoSecondsSinceEpoch() const { return nano_seconds_; }
  time_t SecondsSinceEpoch() const {
    return static_cast<time_t>(nano_seconds_ / NANO_SECONDS_PER_SECOND);
  }

  ///
  /// Get time of now.
  ///
  static Timestamp Now();
  static Timestamp Invalid();

  static const int NANO_SECONDS_PER_SECOND = 1000 * 1000 * 1000;

private:
  int64_t nano_seconds_;
};
namespace detail {
static const Timestamp TIME_MAX = Timestamp(
    std::numeric_limits<time_t>::max() * Timestamp::NANO_SECONDS_PER_SECOND +
    999999999);
static const Timestamp TIME_MIN = Timestamp(
    std::numeric_limits<time_t>::min() * Timestamp::NANO_SECONDS_PER_SECOND +
    0);
} // namespace detail

inline bool operator<(Timestamp lhs, Timestamp rhs) {
  return lhs.NanoSecondsSinceEpoch() < rhs.NanoSecondsSinceEpoch();
}

inline bool operator>(Timestamp lhs, Timestamp rhs) { return rhs < lhs; }

inline bool operator<=(Timestamp lhs, Timestamp rhs) { return !(lhs > rhs); }

inline bool operator>=(Timestamp lhs, Timestamp rhs) { return !(lhs < rhs); }

inline bool operator==(Timestamp lhs, Timestamp rhs) {
  return lhs.NanoSecondsSinceEpoch() == rhs.NanoSecondsSinceEpoch();
}

inline bool operator!=(Timestamp lhs, Timestamp rhs) { return !(lhs == rhs); }

///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microsecond
/// resolution for next 100 years.
inline double TimeDifference(Timestamp high, Timestamp low) {
  int64_t diff = high.NanoSecondsSinceEpoch() - low.NanoSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::NANO_SECONDS_PER_SECOND;
}

///
/// Add @c seconds to given timestamp.
///
/// @return timestamp+seconds as Timestamp
///
inline Timestamp AddTime(Timestamp timestamp, double seconds) {
  int64_t delta =
      static_cast<int64_t>(seconds * Timestamp::NANO_SECONDS_PER_SECOND);
  return Timestamp(timestamp.NanoSecondsSinceEpoch() + delta);
}
} // namespace msgbag
} // namespace nullmax
