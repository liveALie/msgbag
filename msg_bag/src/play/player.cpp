#include "player.h"
#include "log/logger.h"
#include "msgbag_conf.h"
#include "std_msgs.pb.h"
#include "view.h"
#include <iostream>

namespace nullmax {
namespace msgbag {

PlayerOptions::PlayerOptions()
    : prefix_(""), quiet_(false), start_paused_(false), at_once_(false),
      bag_time_(false), bag_time_frequency_(0.0), time_scale_(1.0),
      queue_size_(0), advertise_sleep_(0.2), try_future_(false),
      has_time_(false), loop_(false), time_(0.0f), has_duration_(false),
      duration_(0.0f), keep_alive_(false), wait_for_subscribers_(false),
      rate_control_topic_(""), rate_control_max_delay_(1.0f),
      skip_empty_(9999999999.999999999) {}

void PlayerOptions::Check() {
  if (bags_.size() == 0) {
    std::cout << "You must specify at least one bag file to play from"
              << std::endl;
    exit(1);
  }

  if (has_duration_ && duration_ <= 0.0) {
    std::cout << "Invalid duration, must be > 0.0" << std::endl;
    exit(1);
  }
}

Player::Player(const PlayerOptions &opts, MsgbagConf *conf)
    : options_(opts), paused_(false),
      // If we were given a list of topics to pause on, then go into that mode
      // by default (it can be toggled later via 't' from the keyboard).
      pause_for_topics_(options_.pause_topics_.size() > 0),
      pause_change_requested_(false), requested_pause_state_(false),
      terminal_modified_(false), conf_(conf) {
  Init();
}

Player::~Player() {}

void Player::Stop() { timer_queue_.Stop(); }

void Player::Publish() {
  options_.Check();
  // open all bag files
  for (const auto &bagname : options_.bags_) {
    LOG_INFO("opening {}", bagname);
    try {
      std::shared_ptr<Bag> bag = std::make_shared<Bag>();
      bag->Open(bagname, BagMode::Read);
      bags_.push_back(bag);
    } catch (const std::exception &e) {
      std::cout << "terminated by excption:" << e.what() << std::endl;
      exit(1);
    }
  }
  // SetupTerminal();

  if (!options_.prefix_.empty()) {
    LOG_INFO("using prefix {} for topics.", options_.prefix_);
  }

  if (!options_.quiet_)
    puts(" ");

  View full_view;
  for (const auto &bag : bags_) {
    full_view.AddQuery(*bag);
  }

  Timestamp initial_time = full_view.GetBeginTime();
  initial_time = AddTime(initial_time, options_.time_);

  Timestamp finish_time = detail::TIME_MAX;
  if (options_.has_duration_)
    finish_time = AddTime(initial_time, options_.duration_);

  View view;

  TopicQuery topics(options_.topics_);
  if (options_.topics_.empty()) {
    for (const auto &bag : bags_) {
      view.AddQuery(*bag, initial_time, finish_time);
    }
  } else {
    for (const auto &bag : bags_) {
      view.AddQuery(*bag, topics, initial_time, finish_time);
    }
  }

  if (view.Size() == 0) {
    std::cout << "No messages to play on specified topics.  Exiting."
              << std::endl;
    return;
  }
  std::cout << "start create timers." << std::endl;
  while (true) {
    Timestamp now = Timestamp::Now();
    std::cout << "now:" << now.NanoSecondsSinceEpoch() << std::endl;
    Timestamp initial_time = view.GetBeginTime();
    std::cout << "initial time:" << initial_time.NanoSecondsSinceEpoch()
              << std::endl;
    double diff = TimeDifference(now, initial_time);
    std::cout << "diff :" << diff << " nano seconds." << std::endl;
    for (const auto &record : view.GetRecords()) {
      Timestamp when = AddTime(record.first, diff);
      std::cout << "topic:" << record.second->topic_
                << ",when:" << when.NanoSecondsSinceEpoch() << std::endl;
      timer_queue_.AddTimer(std::bind(&Player::DoPublish, this, record.second),
                            when);
    }
    std::cout << "create timers over." << std::endl;
    timer_queue_.Run();
    if (!options_.loop_) {
      std::cout << std::endl << "Done." << std::endl;
      break;
    }
  }
}

void Player::PauseCallback(const std::string &msg_str) {
  pb::std_msgs::Bool msg;
  msg.ParseFromString(msg_str);
  pause_change_requested_ = (msg.data() != paused_);
  requested_pause_state_ = msg.data();
}

void Player::SetupTerminal() {
  if (terminal_modified_)
    return;
  const int fd = fileno(stdin);
  termios flags;
  tcgetattr(fd, &orig_flags_);
  flags = orig_flags_;
  flags.c_lflag &= ~ICANON; // set raw (unset canonical modes)
  flags.c_cc[VMIN] =
      0; // i.e. min 1 char for blocking, 0 chars for non-blocking
  flags.c_cc[VTIME] = 0; // block if waiting for char
  tcsetattr(fd, TCSANOW, &flags);

  FD_ZERO(&stdin_fdset_);
  FD_SET(fd, &stdin_fdset_);
  maxfd_ = fd + 1;
  terminal_modified_ = true;
}

void Player::Init() {
  publisher_ = std::make_shared<PublishSubscribe>(
      conf_->GetIp(), conf_->GetPort(), conf_->GetBufferSize());
}

void Player::DoPublish(MessageRecordPtr msg_record) {
  // static std::string err_msg;
  // static int msg_id;
  // publisher_.Publish(msg_record->topic_,
  //                    (char *)msg_record->serialized_msg_.c_str(),
  //                    msg_record->serialized_msg_.size(), msg_id, err_msg);
  Timestamp now = Timestamp::Now();
  std::cout << "Player::DoPublish " << now.NanoSecondsSinceEpoch()
            << " publish " << msg_record->topic_ << std::endl;
}

} // namespace msgbag
} // namespace nullmax