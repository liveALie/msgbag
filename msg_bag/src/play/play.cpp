#include "common/msgbag_types.h"
#include "geometry_msgs.pb.h"
#include "log/logger.h"
#include "msgbag_conf.h"
#include "player.h"
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <sstream>

using namespace nullmax::msgbag;
Player *player_ptr = nullptr;

static int lopt = -1;
static struct option long_opts[] = {
    {"help", no_argument, NULL, 'h'},
    {"quiet", no_argument, NULL, 'q'},
    {"immediate", no_argument, NULL, 'i'},
    {"loop", no_argument, NULL, 'l'},
    {"pause", no_argument, &lopt, 0},
    {"keep-alive", no_argument, NULL, 'k'},
    {"clock", no_argument, &lopt, 1},
    {"try-future-version", no_argument, &lopt, 2},
    {"topics", no_argument, &lopt, 3},
    {"pause-topics", no_argument, &lopt, 8},
    {"wait-for-subscribers", no_argument, &lopt, 9},
    {"prefix", required_argument, NULL, 'p'},
    {"delay", required_argument, NULL, 'd'},
    {"rate", required_argument, NULL, 'r'},
    {"queue", required_argument, &lopt, 4},
    {"hz", required_argument, &lopt, 5},
    {"skip-empty", required_argument, &lopt, 6},
    {"start", required_argument, NULL, 's'},
    {"bags", required_argument, &lopt, 7},
    {"duration", required_argument, NULL, 'u'},
    {0, 0, 0, 0}};

void ShowUsage() {
  std::cout << "Usage: rosbag play BAGFILE1 [BAGFILE2 BAGFILE3 ...]            "
               "                     "
            << std::endl;
  std::cout << std::endl;
  std::cout << "Play back the contents of one or more bag files in a "
               "time-synchronized"
            << std::endl;
  std::cout << "fashion." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help            show this help message and exit"
            << std::endl;
  std::cout << "  -p PREFIX, --prefix=PREFIX" << std::endl;
  std::cout << "                        prefix all output topics" << std::endl;
  std::cout << "  -q, --quiet           suppress console output" << std::endl;
  std::cout << "  -i, --immediate       play back all messages without waiting"
            << std::endl;
  std::cout << "  --pause               start in paused mode" << std::endl;
  std::cout << "  --queue=SIZE          use an outgoing queue of size SIZE "
               "(defaults to 100)"
            << std::endl;
  std::cout << "  --clock               publish the clock time" << std::endl;
  std::cout << "  --hz=HZ               use a frequency of HZ when publishing "
               "clock time"
            << std::endl;
  std::cout << "                        (default: 100)" << std::endl;
  std::cout << "  -d SEC, --delay=SEC   sleep SEC seconds after every "
               "advertise call (to allow"
            << std::endl;
  std::cout << "                        subscribers to connect)" << std::endl;
  std::cout << "  -r FACTOR, --rate=FACTOR" << std::endl;
  std::cout << "                        multiply the publish rate by FACTOR"
            << std::endl;
  std::cout << "  -s SEC, --start=SEC   start SEC seconds into the bag files"
            << std::endl;
  std::cout << "  -u SEC, --duration=SEC" << std::endl;
  std::cout
      << "                        play only SEC seconds from the bag files"
      << std::endl;
  std::cout << "  --skip-empty=SEC      skip regions in the bag with no "
               "messages for more than"
            << std::endl;
  std::cout << "                        SEC seconds" << std::endl;
  std::cout << "  -l, --loop            loop playback" << std::endl;
  std::cout << "  -k, --keep-alive      keep alive past end of bag (useful for "
               "publishing"
            << std::endl;
  std::cout << "                        latched topics)" << std::endl;
  std::cout << "  --try-future-version  still try to open a bag file, even if "
               "the version"
            << std::endl;
  std::cout << "                        number is not known to the player"
            << std::endl;
  std::cout << "  --topics              topics to play back" << std::endl;
  std::cout << "  --pause-topics        topics to pause on during playback"
            << std::endl;
  std::cout << "  --bags=BAGS           bags files to play back from"
            << std::endl;
  std::cout << "  --wait-for-subscribers" << std::endl;
  std::cout << "                        wait for at least one subscriber on "
               "each topic before"
            << std::endl;
  std::cout << "                        publishing" << std::endl;
}

void ParseOptions(int argc, char **argv, struct PlayerOptions &opts) {
  char c = '\0';
  int opt_index = 0;
  while ((c = getopt_long(argc, argv, "hp:qid:r:s:u:lk", long_opts,
                          &opt_index)) != -1) {
    switch (c) {
    //--help
    case 'h': {
      ShowUsage();
      exit(0);
    } break;
    //--prefix
    case 'p': {
    } break;
    //--immediate
    case 'i': {
    } break;
    //--quiet
    case 'q': {
    } break;
    //--delay
    case 'd': {
    } break;
    //--rate
    case 'r': {
    } break;
    //--start
    case 's': {
    } break;
    //--duration
    case 'u': {
    } break;
    //--loop
    case 'l': {
    } break;
    //--keep-alive
    case 'k': {
    } break;
    //无参数的option
    case 0: {
      switch (lopt) {
      //--pause
      case 0: {
      } break;
      //--clock
      case 1: {
      } break;
      //--try-future-version
      case 2: {
      } break;
      //--topics
      case 3: {
      } break;
      //--queue
      case 4: {
      } break;
      //--hz
      case 5: {
      } break;
      //--skip-empty
      case 6: {
      } break;
      //--bags
      case 7: {
      } break;
      // pause-topics
      case 8: {
      } break;
      //--wait-for-subscribers
      case 9: {
      } break;
      default: { std::cout << "error option." << std::endl; } break;
      }
    }
    default: { std::cout << "error option." << std::endl; } break;
    }
  }
  while (::optind < argc) {
    opts.bags_.push_back(argv[optind++]);
  }
}

void ProgExit(int signo) {
  LOG_CRITICAL("program recv signal [{}],to exit.", signo);
  if (player_ptr) {
    player_ptr->Stop();
  }
}

int main(int argc, char *argv[]) {
  signal(SIGCHLD, SIG_DFL);
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, ProgExit);
  signal(SIGTERM, ProgExit);

  MsgbagConf conf;
  conf.LoadConf("msgbag.toml");

  // INIT_LOGGER("msgbag_recorder", "recorder.log", 1024 * 512, 3);
  Singleton<MyLogger>::GetInstance(
      conf.GetPlayerLoggerName(), conf.GetPlayerLogFile(),
      conf.GetLogfileMaxSize(), conf.GetMaxLogfileNum());
  LOG_DEBUG("conf ip:{}", conf.GetIp());
  LOG_DEBUG("conf port:{}", conf.GetPort());
  LOG_DEBUG("conf buffer_size:{}", conf.GetBufferSize());
  LOG_DEBUG("conf logger_name:{}", conf.GetPlayerLoggerName());
  LOG_DEBUG("conf log_file:{}", conf.GetPlayerLogFile());
  LOG_DEBUG("conf max_logfile_size:{}", conf.GetLogfileMaxSize());
  LOG_DEBUG("conf max_logfile_num:{}", conf.GetMaxLogfileNum());
  LOG_INFO("player start.");
  struct PlayerOptions opts;
  ParseOptions(argc, argv, opts);
  Player player(opts, &conf);

  player_ptr = &player;
  player.Publish();
  LOG_INFO("player stopped.");
  return 0;
}
// int main(int argc, char **argv) {

//   std::fstream fi(argv[1], std::ios::in);
//   if (!fi) {
//     std::cout << "file:{} open failed,because file not exist." << std::endl;
//     exit(1);
//   }
//   std::string msg;
//   std::string version_info;
//   getline(fi, version_info);
//   std::cout << "version:" << version_info << std::endl;
//   auto index_data = fi.tellg();
//   fi.seekg(0, std::ios::end);
//   auto end = fi.tellg();
//   fi.seekg(index_data);

//   while (fi.tellg() < end) {
//     size_t msg_size = 0;
//     long long time_stamp = 0;
//     fi.read((char *)&msg_size, sizeof(size_t));
//     fi.read((char *)&time_stamp, sizeof(long long));
//     std::string topic;
//     getline(fi, topic);
//     char *buf = new char[msg_size];
//     fi.read(buf, msg_size);
//     pb::geometry_msgs::PoseStamped pb_msg;
//     pb_msg.ParsePartialFromArray(buf, msg_size);
//     std::cout << "msg size:" << msg_size << ",timestamp:" << time_stamp
//               << ",topic:" << topic << ",msg body:" << pb_msg.DebugString();
//   }
// }
