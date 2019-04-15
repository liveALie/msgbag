#pragma once
#include "bag.h"
#include "common/msgbag_types.h"
#include "common/timestamp.h"
#include "common/timer_queue.h"
#include "publish_subscribe.h"
#include <chrono>
#include <string>
#include <termios.h>
#include <vector>

namespace nullmax {
namespace msgbag {
struct PlayerOptions {
  PlayerOptions();

  void Check();

  std::string prefix_;
  bool quiet_;
  bool start_paused_;
  bool at_once_;
  bool bag_time_;
  double bag_time_frequency_;
  double time_scale_;
  int queue_size_;
  TimeDuration advertise_sleep_;
  bool try_future_;
  bool has_time_;
  bool loop_;
  float time_;
  bool has_duration_;
  float duration_;
  bool keep_alive_;
  bool wait_for_subscribers_;
  std::string rate_control_topic_;
  float rate_control_max_delay_;
  TimeDuration skip_empty_;

  std::vector<std::string> bags_;
  std::vector<std::string> topics_;
  std::vector<std::string> pause_topics_;
};

class MsgbagConf;

class Player {

public:
  Player(const PlayerOptions &opts, MsgbagConf *conf);
  ~Player();
  void Stop();
  template <typename T>
  void MsgHandler(const std::string &topic, const std::string &msg);
  void Publish();
  void DoPublish(MessageRecordPtr msg_record);

private:
  void Init();
  void PauseCallback(const std::string &msg_str);
  void SetupTerminal();

private:
  typedef std::shared_ptr<PublishSubscribe> PubSubPtr;
  typedef std::map<std::string, PubSubPtr> PublisherMap;

  PlayerOptions options_;

  // ros::NodeHandle node_handle_;

  // ros::ServiceServer pause_service_;

  bool paused_;
  bool delayed_;

  bool pause_for_topics_;

  bool pause_change_requested_;

  bool requested_pause_state_;

  // ros::Subscriber rate_control_sub_;
  PubSubPtr subscriber_;
  std::string err_msg_;

  Timestamp last_rate_control_;

  Timestamp paused_time_;

  std::vector<std::shared_ptr<Bag>> bags_;
  // PublisherMap publishers_;
  PubSubPtr publisher_;

  // Terminal
  bool terminal_modified_;
  termios orig_flags_;
  fd_set stdin_fdset_;
  int maxfd_;

  // TimeTranslator time_translator_;
  // TimePublisher time_publisher_;

  Timestamp start_time_;
  TimeDuration bag_length_;
  MsgbagConf *conf_;
  TimerQueue timer_queue_;
};

template <typename T>
void Player::MsgHandler(const std::string &topic, const std::string &msg) {}

} // namespace msgbag
} // namespace nullmax
