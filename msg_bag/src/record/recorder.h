#pragma once

#include "bag.h"
#include "common/msgbag_constants.h"
#include "common/thread_pool.h"
#include "common/time.h"
#include "drive_timer.h"
#include "log/logger.h"
#include "publish_subscribe.h"
#include <atomic>
#include <chrono>
#include <common/buffer.h>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace nullmax {
namespace msgbag {

class OutgoingMessage {
public:
  OutgoingMessage(const std::string &topic, const std::string &msg)
      : time_(Time::Now()), topic_(topic), msg_(msg) {}
  // topic_tools::ShapeShifter::ConstPtr _msg, boost::shared_ptr<ros::M_string>
  // _connection_header, ros::Time _time);
  nullmax::msgbag::Time time_;
  std::string topic_;
  std::string msg_;
  // unsigned int size_;
  // Buffer_t buffer_;
  // topic_tools::ShapeShifter::ConstPtr msg;
  // std::shared_ptr<ros::M_string>    connection_header;
  // ros::Time                           time;
};

class OutgoingQueue {
public:
  // OutgoingQueue(std::string const& _filename, std::queue<OutgoingMessage>*
  // _queue, ros::Time _time);

  std::string filename;
  std::queue<OutgoingMessage> *queue;
  // ros::Time                    time;
};

struct RecorderOptions {
  RecorderOptions();

  bool trigger;
  bool record_all;             //--all,-a
  bool regex;                  //--regex,-e
  bool do_exclude;             //--exclude,-x
  bool quiet;                  //--quiet,-q
  bool append_date;            //--out-prefix,-o and --out-name,-O
  bool snapshot;               // default false,no option
  bool verbose;                // default false,no option
  bool publish;                //--publish,-p
  CompressionType compression; //--bz2,-j and --lz4
  std::string prefix;          //--out-prefix,-o
  std::string name;            //--out-name,-O
  // boost::regex    exclude_regex;//--exclude,-x
  uint32_t buffer_size;         //--buffsize,-b
  uint32_t chunk_size;          //--chunksize
  uint32_t limit;               //--limit,-l
  bool split;                   //--split
  uint64_t max_size;            //--size
  uint32_t max_splits;          //--max-splits
  TimeDuration max_duration;    //--duration sec-double
  std::string node;             //--node
  unsigned long long min_space; //--min-space, -L
  std::string min_space_str;    //--min-space, -L
  // ros::TransportHints transport_hints;//--tcpnodelay,--udp

  std::vector<std::string> topics; //--topic
};

class MsgbagConf;

class Recorder {
public:
  Recorder(const RecorderOptions &options, MsgbagConf *conf);

  void DoTrigger();

  //   bool IsSubscribed(std::string const &topic) const;

  // std::shared_ptr<ros::Subscriber> subscribe(std::string const& topic);

  int Run();
  void Stop();
  template <typename T>
  void MsgHandler(const std::string &topic, const std::string &str_msg);
  void DoCheckMaster();

private:
  void DoSubscribe();
  void DoRecordMultiThread(const OutgoingMessage &msg);
  void DoRecordSingleThread();
  void DoWrite(const OutgoingMessage &out_msg);
  void InitRecordedTopics();
  void InitSubscribers();
  void EventLoop(const TimeDuration &duration);
  void StartWriting();
  void StopWriting();
  void UpdateFilename();
  bool CheckDisk();
  bool CheckSize();
  bool CheckDuration(const Time &time);
  bool ScheduledCheckDisk();
  bool CheckLogging();
  void CheckNumSplits();

  //   void PrintUsage();

  //   void UpdateFilenames();
  //   void StartWriting();
  //   void StopWriting();

  //   bool CheckLogging();
  //   bool ScheduledCheckDisk();
  //   bool CheckDisk();

  //   void SnapshotTrigger(const Empty &trigger);
  //   //   void
  //   //   DoQueue(const ros::MessageEvent<topic_tools::ShapeShifter const>
  //   //   &msg_event,
  //   //           std::string const &topic,
  //   //           boost::shared_ptr<ros::Subscriber> subscriber,
  //   //           boost::shared_ptr<int> count);
  //   void DoRecord();
  //   void CheckNumSplits();
  //   bool CheckSize();
  //   bool CheckDuration(const nullmax::msgbag::Time &);
  //   void DoRecordSnapshotter();
  //   // void DoCheckMaster(ros::TimerEvent const &e, ros::NodeHandle
  //   &node_handle);

  //   bool ShouldSubscribeToTopic(std::string const &topic, bool from_node =
  //   false);

  //   template <class T> static std::string TimeToStr(T ros_t);

private:
  RecorderOptions options_;

  Bag bag_;

  std::string target_filename_;
  std::string write_filename_;
  std::list<std::string> current_files_;

  std::set<std::string>
      currently_recording_; //!< set of currenly recording topics
  int num_subscribers_; //!< used for book-keeping of our number of subscribers

  int exit_code_; //!< eventual exit code

  // std::condition_variable_any
  //     queue_condition_;                //!< conditional variable for queue
  // std::mutex queue_mutex_;             //!< mutex for queue
  // std::queue<OutgoingMessage> *queue_; //!< queue for storing
  // uint64_t queue_size_;     //!< queue size
  // uint64_t max_queue_size_; //!< max queue size

  uint64_t split_count_; //!< split count

  std::queue<OutgoingQueue>
      queue_queue_; //!< queue of queues to be used by the snapshot recorders

  nullmax::msgbag::Time last_buffer_warn_;

  nullmax::msgbag::Time start_time_;

  bool writing_enabled_;
  std::mutex check_disk_mutex_;
  nullmax::msgbag::Time check_disk_next_;
  nullmax::msgbag::Time warn_next_;
  MsgbagConf *conf_;

  // ros::Publisher pub_begin_write;
  std::vector<std::shared_ptr<PublishSubscribe>> subscribers_;
  std::string err_msg_;

  std::atomic<bool> running_;
  ThreadPool thread_pool_;
  SyncQueue<OutgoingMessage> outmsg_queueu_;
  std::shared_ptr<std::thread> record_thread_;
};

template <typename T>
void Recorder::MsgHandler(const std::string &topic,
                          const std::string &str_msg) {
  T msg;
  msg.ParseFromString(str_msg);
  OutgoingMessage om(topic, str_msg);
  // uint64_t msg_size = msg.ByteSizeLong();
  // LOG_DEBUG("msgsize:{},msg:{}", msg_size, msg.DebugString());
  // OutgoingMessage om;
  // std::string now_str = om.time_.ToTimeStr();
  // int64_t outmsg_size = sizeof(int64_t) + now_str.size() + topic.size() +
  //                       msg_size + sizeof("\t") * 3;
  // LOG_DEBUG("outmsg size:{},msg:", outmsg_size, msg.DebugString());
  // // LOG_DEBUG("receive msg,topic:{},msg:{}", topic, msg.DebugString());

  // std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(outmsg_size);
  // buffer->appendInt64(msg.ByteSizeLong());
  // buffer->append("\t");
  // buffer->append(now_str);
  // buffer->append("\t");
  // buffer->append(topic);
  // buffer->append("\t");

  // msg.SerializeToArray(buffer->beginWrite(), msg_size);
  // om.buffer_ = buffer;
  // // thread_pool_.AddTask(std::bind(&Recorder::DoRecord, this, out_msg));
  if (running_) {
    outmsg_queueu_.Put(om);
  }
}
} // namespace msgbag
} // namespace nullmax
