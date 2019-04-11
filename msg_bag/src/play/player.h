#pragma once
#include "common/msgbag_constants.h"
#include <chrono>
#include <string>
#include <vector>

namespace nullmax {
namespace msgbag {
struct PlayerOptions {
  PlayerOptions(){};

  void check(){};

  std::string prefix;
  bool quiet;
  bool start_paused;
  bool at_once;
  bool bag_time;
  double bag_time_frequency;
  double time_scale;
  int queue_size;
  TimeDuration advertise_sleep;
  bool try_future;
  bool has_time;
  bool loop;
  float time;
  bool has_duration;
  float duration;
  bool keep_alive;
  bool wait_for_subscribers;
  std::string rate_control_topic;
  float rate_control_max_delay;
  TimeDuration skip_empty;

  std::vector<std::string> bags;
  std::vector<std::string> topics;
  std::vector<std::string> pause_topics;
};

class Player {
public:
  void Stop();
};
} // namespace msgbag
} // namespace nullmax
