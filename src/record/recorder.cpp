#include "recorder.h"

// OutgoingMessage

// OutgoingMessage::OutgoingMessage(string const& _topic,
// topic_tools::ShapeShifter::ConstPtr _msg, boost::shared_ptr<ros::M_string>
// _connection_header, Time _time) :
//     topic(_topic), msg(_msg), connection_header(_connection_header),
//     time(_time)
// {
// }

// // OutgoingQueue

// OutgoingQueue::OutgoingQueue(string const& _filename,
// std::queue<OutgoingMessage>* _queue, Time _time) :
//     filename(_filename), queue(_queue), time(_time)
// {
// }

RecorderOptions::RecorderOptions()
    : trigger(false), record_all(false), regex(false), do_exclude(false),
      quiet(false), append_date(true), snapshot(false), verbose(false),
      publish(false),
      // compression(compression::Uncompressed),
      prefix(""), name(""),
      // exclude_regex(),
      buffer_size(1048576 * 256), chunk_size(1024 * 768), limit(0),
      split(false), max_size(0), max_splits(0), max_duration(-1.0), node(""),
      min_space(1024 * 1024 * 1024), min_space_str("1G") {}

Recorder::Recorder(const RecorderOptions &options)
    : options_(options), num_subscribers_(0), exit_code_(0), queue_size_(0),
      split_count_(0), writing_enabled_(true) {}

void Recorder::DoTrigger() {}

int Recorder::Run() {
  if (options_.trigger) {
    DoTrigger();
    return 0;
  }

  if (options_.topics.size() == 0) {
    // Make sure limit is not specified with automatic topic subscription
    if (options_.limit > 0) {
      fprintf(stderr, "Specifing a count is not valid with automatic topic "
                      "subscription.\n");
      return 1;
    }

    // Make sure topics are specified
    if (!options_.record_all && (options_.node == std::string(""))) {
      fprintf(stderr, "No topics specified.\n");
      return 1;
    }
  }

  //   ros::NodeHandle nh;
  //   if (!nh.ok())
  //     return 0;

  //   if (options_.publish) {
  //     pub_begin_write = nh.advertise<std_msgs::String>("begin_write", 1,
  //     true);
  //   }

  //   last_buffer_warn_ = Time();
  //   queue_ = new std::queue<OutgoingMessage>;

  //   // Subscribe to each topic
  //   if (!options_.regex) {
  //     for (string const &topic : options_.topics)
  //       subscribe(topic);
  //   }

  //   if (!ros::Time::waitForValid(ros::WallDuration(2.0)))
  //     ROS_WARN("/use_sim_time set to true and no clock published.  Still
  //     waiting "
  //              "for valid time...");

  //   ros::Time::waitForValid();

  //   start_time_ = ros::Time::now();

  //   // Don't bother doing anything if we never got a valid time
  //   if (!nh.ok())
  //     return 0;

  //   ros::Subscriber trigger_sub;

  //   // Spin up a thread for writing to the file
  //   boost::thread record_thread;
  //   if (options_.snapshot) {
  //     record_thread =
  //         boost::thread(boost::bind(&Recorder::doRecordSnapshotter, this));

  //     // Subscribe to the snapshot trigger
  //     trigger_sub = nh.subscribe<std_msgs::Empty>(
  //         "snapshot_trigger", 100,
  //         boost::bind(&Recorder::snapshotTrigger, this, _1));
  //   } else
  //     record_thread = boost::thread(boost::bind(&Recorder::doRecord, this));

  //   ros::Timer check_master_timer;
  //   if (options_.record_all || options_.regex ||
  //       (options_.node != std::string(""))) {
  //     // check for master first
  //     doCheckMaster(ros::TimerEvent(), nh);
  //     check_master_timer = nh.createTimer(
  //         ros::Duration(1.0),
  //         boost::bind(&Recorder::doCheckMaster, this, _1, boost::ref(nh)));
  //   }

  //   ros::AsyncSpinner s(10);
  //   s.start();

  //   record_thread.join();
  //   queue_condition_.notify_all();
  //   delete queue_;

  //   return exit_code_;
  return 0;
}