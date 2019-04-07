#pragma once

#include "common/msgbag_types.h"
#include "common/time.h"
#include <chrono>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <vector>
// #include "msgbag_msgs.pb.cc"

class OutgoingMessage {
public:
  // OutgoingMessage(std::string const& _topic,
  // topic_tools::ShapeShifter::ConstPtr _msg, boost::shared_ptr<ros::M_string>
  // _connection_header, ros::Time _time);

  std::string topic;
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
  bool record_all;
  bool regex;
  bool do_exclude;
  bool quiet;
  bool append_date;
  bool snapshot;
  bool verbose;
  bool publish;
  // CompressionType compression;
  std::string prefix;
  std::string name;
  // boost::regex    exclude_regex;
  uint32_t buffer_size;
  uint32_t chunk_size;
  uint32_t limit;
  bool split;
  uint64_t max_size;
  uint32_t max_splits;
  // ros::Duration   max_duration; //sec and nsec
  std::chrono::duration<double> max_duration; // sec-double
  std::string node;
  unsigned long long min_space;
  std::string min_space_str;
  // ros::TransportHints transport_hints;

  std::vector<std::string> topics;
};

class Recorder {
public:
  Recorder(const RecorderOptions &options);

  void DoTrigger();

  //   bool IsSubscribed(std::string const &topic) const;

  // std::shared_ptr<ros::Subscriber> subscribe(std::string const& topic);

  int Run();

private:
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

  // Bag                           bag_;

  std::string target_filename_;
  std::string write_filename_;
  std::list<std::string> current_files_;

  std::set<std::string>
      currently_recording_; //!< set of currenly recording topics
  int num_subscribers_; //!< used for book-keeping of our number of subscribers

  int exit_code_; //!< eventual exit code

  std::condition_variable_any
      queue_condition_;                //!< conditional variable for queue
  std::mutex queue_mutex_;             //!< mutex for queue
  std::queue<OutgoingMessage> *queue_; //!< queue for storing
  uint64_t queue_size_;                //!< queue size
  uint64_t max_queue_size_;            //!< max queue size

  uint64_t split_count_; //!< split count

  std::queue<OutgoingQueue>
      queue_queue_; //!< queue of queues to be used by the snapshot recorders

  nullmax::msgbag::Time last_buffer_warn_;

  nullmax::msgbag::Time start_time_;

  bool writing_enabled_;
  std::mutex check_disk_mutex_;
  nullmax::msgbag::Time check_disk_next_;
  nullmax::msgbag::Time warn_next_;

  // ros::Publisher pub_begin_write;
};