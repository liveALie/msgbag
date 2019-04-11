#include "recorder.h"
#include "common/msgbag_constants.h"
#include "common/msgbag_exception.h"
#include "drive_timer.h"
#include "log/logger.h"
#include "msgbag_conf.h"
#include <exception>
#include <sys/statvfs.h>

namespace nullmax {
namespace msgbag {

// // OutgoingQueue

// OutgoingQueue::OutgoingQueue(string const& _filename,
// std::queue<OutgoingMessage>* _queue, Time _time) :
//     filename(_filename), queue(_queue), time(_time)
// {
// }

RecorderOptions::RecorderOptions()
    : trigger(false), record_all(false), regex(false), do_exclude(false),
      quiet(false), append_date(true), snapshot(false), verbose(false),
      publish(false), compression(Uncompressed), prefix(""), name(""),
      // exclude_regex(),
      buffer_size(1048576 * 256), chunk_size(1024 * 768), limit(0),
      split(false), max_size(0), max_splits(0), max_duration(-1.0), node(""),
      min_space(1024 * 1024 * 1024), min_space_str("1G") {}

Recorder::Recorder(const RecorderOptions &options, MsgbagConf *conf)
    : options_(options), num_subscribers_(0), exit_code_(0), split_count_(0),
      writing_enabled_(true), conf_(conf), err_msg_(""), running_(false),
      thread_pool_(4), outmsg_queueu_(MAX_TASK_COUNT) {}

void Recorder::DoTrigger() {
  LOG_INFO("Recorder::DoTrigger,reorder will exit.");
}

void Recorder::DoSubscribe() {
  int index = 0;
  for (const auto &topic : options_.topics) {
    auto handler = msg_handler_map.find(topic);
    if (handler == msg_handler_map.end()) {
      LOG_ERROR("msg of topic:{} has no handler.", topic);
      continue;
    }
    if (index >= subscribers_.size()) {
      index = 0;
    }
    subscribers_[index++]->Subscribe(topic, handler->second, err_msg_);
    currently_recording_.insert(topic);
  }
}

void Recorder::DoWrite(const OutgoingMessage &out_msg) {
  LOG_DEBUG("Recorder::DoWrite");
  if (CheckSize())
    running_ = false;

  if (CheckDuration(out_msg.time_))
    running_ = false;
  std::string msg;
  msg.append(std::to_string(out_msg.msg_.size()))
      .append("\t")
      .append(out_msg.time_.ToTimeStr())
      .append("\t")
      .append(out_msg.topic_)
      .append("\t\n")
      .append(out_msg.msg_);
  LOG_DEBUG("to write msg:{}", msg);
  // LOG_DEBUG("Recorder::DoWrite,buffer readablebytes:",
  //           out_msg.buffer_->readableBytes());
  if (ScheduledCheckDisk() && CheckLogging()) {
    try {
      // bag_.Write(out_msg.buffer_->peek(), out_msg.buffer_->readableBytes());
      bag_.Write(msg);
    } catch (const MsgbagException &e) {
      std::cout << "terminated exception:" << e.what() << std::endl;
      running_ = false;
      return;
    }
  }
}

void Recorder::StartWriting() {
  LOG_INFO("Recorder::StartWriting");
  bag_.SetCompression(options_.compression);
  bag_.SetChunkThreshold(options_.chunk_size);
  UpdateFilename();
  try {
    bag_.Open(write_filename_, BagMode::Write);
  } catch (const MsgbagException &e) {
    std::cout << "terminated exception:" << e.what() << std::endl;
    running_ = false;
    return;
  }

  LOG_INFO("recording to {}", target_filename_);
  if (options_.publish) {
    // TODO:not important.
  }
}

void Recorder::StopWriting() {
  LOG_INFO("Closing {}.", target_filename_);
  bag_.Close();
  ::rename(write_filename_.c_str(), target_filename_.c_str());
}

void Recorder::UpdateFilename() {
  LOG_INFO("Recorder::UpdateFilename");
  std::vector<std::string> parts;
  std::string prefix = options_.prefix;
  size_t ind = prefix.rfind(".bag");
  if (ind != std::string::npos && ind == prefix.size() - 4) {
    prefix.erase(ind);
  }
  if (prefix.length() > 0)
    parts.push_back(prefix);
  if (options_.append_date)
    parts.push_back(Time::Now().ToStr());
  if (options_.split)
    parts.push_back(std::to_string(split_count_));

  if (parts.size() == 0) {
    std::cerr << "Bag filename is empty (neither of these was specified: "
                 "prefix, append_date, split)"
              << std::endl;
    running_ = false;
  }

  target_filename_ = parts[0];
  for (int i = 1; i < parts.size(); ++i)
    target_filename_.append("_").append(parts[i]);

  target_filename_.append(".bag");
  write_filename_ = target_filename_ + ".active";
  LOG_DEBUG("update file name,target_filename:{},write_filename:{}",
            target_filename_, write_filename_);
}

void Recorder::DoRecordMultiThread(const OutgoingMessage &msg) {
  // LOG_INFO("Recorder::DoRecordMultiThread, out_msg topic:{}", msg.topic_);
}

bool Recorder::CheckDisk() {
  LOG_INFO("Recorder::CheckDisk");
  struct statvfs fiData;
  if ((statvfs(bag_.GetFileName().c_str(), &fiData)) < 0) {
    LOG_WARN("Failed to check filesystem stats.");
    return true;
  }
  unsigned long long free_space = 0;
  free_space = (unsigned long long)(fiData.f_bsize) *
               (unsigned long long)(fiData.f_bavail);
  if (free_space < options_.min_space) {
    LOG_ERROR(
        "Less than {} of space free on disk with {}.  Disabling recording.",
        options_.min_space_str.c_str(), bag_.GetFileName().c_str());
    writing_enabled_ = false;
    return false;
  } else if (free_space < 5 * options_.min_space) {
    LOG_WARN("Less than 5 x {} of space free on disk with {}.",
             options_.min_space_str.c_str(), bag_.GetFileName().c_str());
  } else {
    writing_enabled_ = true;
  }
  return true;
}

bool Recorder::CheckSize() {
  LOG_DEBUG("Recorder::CheckSize");
  if (options_.max_size > 0) {
    if (bag_.GetSize() > options_.max_size) {
      if (options_.split) {
        StopWriting();
        split_count_++;
        CheckNumSplits();
        StartWriting();
      } else {
        running_ = false;
        return true;
      }
    }
  }
  return false;
}

void Recorder::CheckNumSplits() {
  LOG_DEBUG("Recorder::CheckNumSplits");
  if (options_.max_splits > 0) {
    current_files_.push_back(target_filename_);
    if (current_files_.size() > options_.max_splits) {
      int err = unlink(current_files_.front().c_str());
      if (err != 0) {
        LOG_ERROR("Unable to remove {}: {}", current_files_.front().c_str(),
                  strerror(errno));
      }
      current_files_.pop_front();
    }
  }
}

bool Recorder::CheckDuration(const Time &time) {
  LOG_DEBUG("Recorder::CheckDuration");
  if (options_.max_duration > TimeDuration(0)) {
    if ((time - start_time_) > options_.max_duration) {
      if (options_.split) {
        while (start_time_ + options_.max_duration < time) {
          StopWriting();
          split_count_++;
          CheckNumSplits();
          start_time_.AddDuration(options_.max_duration);
          StartWriting();
        }
      } else {
        running_ = false;
        return true;
      }
    }
  }
  return false;
}

bool Recorder::ScheduledCheckDisk() {
  std::lock_guard<std::mutex> lck(check_disk_mutex_);
  if (Time::Now() < check_disk_next_)
    return true;
  check_disk_next_.AddDuration(TimeDuration(2.0));
  return CheckDisk();
}

bool Recorder::CheckLogging() {
  if (writing_enabled_)
    return true;

  Time now = Time::Now();
  if (!(now < warn_next_)) {
    warn_next_ = now.AddDuration(TimeDuration(5.0));
    LOG_WARN("Not logging message because logging disabled.  Most likely cause "
             "is a full disk.");
  }
  return false;
}

void Recorder::DoRecordSingleThread() {
  LOG_INFO("Recorder::DoRecordSingleThread");
  StartWriting();
  warn_next_ = Time::Now();
  CheckDisk();
  // TODO:
  // try
  //   {
  //       checkDisk();
  //   }
  //   catch (rosbag::BagException &ex)
  //   {
  //       ROS_ERROR_STREAM(ex.what());
  //       exit_code_ = 1;
  //       stopWriting();
  //       return;
  //   }
  check_disk_next_ = Time::Now().AddDuration(TimeDuration(20));
  // while (running_) {
  //   std::list<Task> list;
  //   queue_.Take(list);
  //   for (auto &task : list) {
  //     if (!running_) {
  //       return;
  //     }
  //     task();
  //   }
  // }
  while (running_) {
    std::list<OutgoingMessage> list;
    outmsg_queueu_.Take(list);
    LOG_DEBUG("take msgs from queue,size:{}", list.size());
    for (auto &msg : list) {
      if (!running_) {
        return;
      }
      DoWrite(msg);
    }
  }
}

void Recorder::InitRecordedTopics() {

  for (const auto &handler : msg_handler_map) {
    options_.topics.push_back(handler.first);
  }
}

void Recorder::InitSubscribers() {
  if (options_.topics.size() >= 4) {
    num_subscribers_ = 4;
  } else {
    num_subscribers_ = options_.topics.size();
  }
  subscribers_.reserve(num_subscribers_);
  for (int i = 0; i < num_subscribers_; ++i) {
    subscribers_.push_back(std::make_shared<PublishSubscribe>(
        conf_->GetIp(), conf_->GetPort(), conf_->GetBufferSize()));
  }
  LOG_DEBUG("number of subscriber is {}", subscribers_.size());
}

void Recorder::DoCheckMaster() { LOG_INFO("Recorder::DoCheckMaster"); }

void Recorder::EventLoop(const TimeDuration &duaration) {
  const TimeDuration loop_duration(0.01);
  TimeDuration real_duration(0.0);
  while (running_) {
    std::this_thread::sleep_for(loop_duration);
    if (duaration <= real_duration) {
      DoCheckMaster();
      real_duration = TimeDuration(0.0);
      continue;
    }
    real_duration += loop_duration;
  }
}

int Recorder::Run() {
  InitHandlerMap(this);
  LOG_DEBUG("msg_handler_map size:{}", msg_handler_map.size());
  if (options_.record_all) {
    InitRecordedTopics();
  }

  if (options_.trigger) {
    // TODO:发送一个空消息，然后退出
    DoTrigger();
    exit(0);
  }

  if (options_.topics.size() == 0) {
    LOG_ERROR("recorder run,topics is empty.");
    // Make sure limit is not specified with automatic topic subscription
    if (options_.limit > 0) {
      std::cerr << "Specifing a count is not valid with automatic topic "
                   "subscription."
                << std::endl;
      exit(1);
    }

    // Make sure topics are specified
    if (!options_.record_all && (options_.node == std::string(""))) {
      std::cerr << "No topics specified." << std::endl;
      exit(1);
    }
  }
  InitSubscribers();
  if (options_.publish) {
    // pub_begin_write = nh.advertise<std_msgs::String>("begin_write", 1,
    // true);
    LOG_INFO("recorder run,options_.publish is:{}", options_.publish);
    // TODO: create a ipc pubsub to publish string msg.
  }

  last_buffer_warn_ = nullmax::msgbag::Time::Now();
  running_ = true;
  start_time_ = nullmax::msgbag::Time::Now();
  record_thread_ = std::make_shared<std::thread>(
      std::thread(std::bind(&Recorder::DoRecordSingleThread, this)));
  DoSubscribe();
  // Subscribe to each topic,now regex is not useless.
  // if (!options_.regex) {
  //   DoSubscribe();
  // }
  // TODO:use regex,node.
  // if (options_.regex) {
  //   // check for master first
  //   DoCheckMaster();
  //   EventLoop(TimeDuration(1.0));
  // }
  while (running_) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
  return exit_code_;
}

void Recorder::Stop() {
  LOG_INFO("Recorder stop.");
  running_ = false;
  outmsg_queueu_.Stop();
  StopWriting();
  record_thread_->join();
  thread_pool_.Stop();
}
} // namespace msgbag
} // namespace nullmax
