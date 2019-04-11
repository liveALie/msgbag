#include "common/msgbag_constants.h"
#include "log/logger.h"
#include "msgbag_conf.h"
#include "recorder.h"
#include <getopt.h>
#include <iostream>
#include <sstream>

using namespace nullmax::msgbag;

static Recorder *recorder_ptr = nullptr;
static int lopt = -1;
static struct option long_opts[] = {
    {"help", no_argument, NULL, 'h'},
    {"all", no_argument, NULL, 'a'},
    {"regex", no_argument, NULL, 'e'},
    {"quiet", no_argument, NULL, 'q'},
    {"split", no_argument, &lopt, 0},
    {"bz2", no_argument, NULL, 'j'},
    {"lz4", no_argument, &lopt, 1},
    {"tcpnodelay", no_argument, &lopt, 2},
    {"udp", no_argument, &lopt, 3},
    {"exclude", required_argument, NULL, 'x'},
    {"output-prefix", required_argument, NULL, 'o'},
    {"output-name", required_argument, NULL, 'O'},
    {"max-splits", required_argument, &lopt, 4},
    {"size", required_argument, &lopt, 5},
    {"duration", required_argument, &lopt, 6},
    {"buffsize", required_argument, NULL, 'b'},
    {"chunksize", required_argument, &lopt, 7},
    {"limit", required_argument, NULL, 'l'},
    {"node", required_argument, &lopt, 8},
    {"min-space", required_argument, NULL, 'L'},
    {"topic", no_argument, &lopt, 9},
    {"publish", no_argument, NULL, 'p'},
    {0, 0, 0, 0}};

void ShowUsage() {
  std::cout << "Usage: msgbag --record TOPIC1 [TOPIC2 TOPIC3 ...]."
            << std::endl;
  std::cout << std::endl;
  std::cout << "Record a bag file with the contents of specified topics."
            << std::endl;
  std::cout << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help            show this help message and exit"
            << std::endl;
  std::cout << "  -a, --all             record all topics" << std::endl;
  std::cout << "  -e, --regex           match topics using regular expressions"
            << std::endl;
  std::cout << "  -x EXCLUDE_REGEX, --exclude=EXCLUDE_REGEX" << std::endl;
  std::cout << "                        exclude topics matching the follow "
               "regular expression"
            << std::endl;
  std::cout << "                        (subtracts from -a or regex)"
            << std::endl;
  std::cout << "  -q, --quiet           suppress console output" << std::endl;
  std::cout << "  -o PREFIX, --output-prefix=PREFIX" << std::endl;
  std::cout << "                        prepend PREFIX to beginning of bag "
               "name (name will"
            << std::endl;
  std::cout << "                        always end with date stamp)"
            << std::endl;
  std::cout << "  -O NAME, --output-name=NAME" << std::endl;
  std::cout << "                        record to bag with name NAME.bag"
            << std::endl;
  std::cout << "  --split               split the bag when maximum size or "
               "duration is reached"
            << std::endl;
  std::cout << "  --max-splits=MAX_SPLITS" << std::endl;
  std::cout << "                        Keep a maximum of N bag files, when "
               "reaching the"
            << std::endl;
  std::cout << "                        maximum erase the oldest one to keep a "
               "constant number"
            << std::endl;
  std::cout << "                        of files." << std::endl;
  std::cout << "  --size=SIZE           record a bag of maximum size SIZE MB. "
               "(Default:"
            << std::endl;
  std::cout << "                        infinite)" << std::endl;
  std::cout << "  --duration=DURATION   record a bag of maximum duration "
               "DURATION in seconds,"
            << std::endl;
  std::cout << "                        unless 'm', or 'h' is appended."
            << std::endl;
  std::cout << "  -b SIZE, --buffsize=SIZE" << std::endl;
  std::cout << "                        use an internal buffer of SIZE MB "
               "(Default: 256, 0 ="
            << std::endl;
  std::cout << "                        infinite)" << std::endl;
  std::cout << "  --chunksize=SIZE      Advanced. Record to chunks of SIZE KB "
               "(Default: 768)"
            << std::endl;
  std::cout << "  -l NUM, --limit=NUM   only record NUM messages on each topic"
            << std::endl;
  std::cout << "  --node=NODE           record all topics subscribed to by a "
               "specific node"
            << std::endl;
  std::cout << "  -j, --bz2             use BZ2 compression" << std::endl;
  std::cout << "  --lz4                 use LZ4 compression" << std::endl;
  std::cout << "  --tcpnodelay          Use the TCP_NODELAY transport hint "
               "when subscribing to"
            << std::endl;
  std::cout << "                        topics." << std::endl;
  std::cout << "  --udp                 Use the UDP transport hint when "
               "subscribing to topics."
            << std::endl;
}

void ParseOption(int argc, char **argv, RecorderOptions &opts) {
  char c = '\0';
  int opt_index = 0;
  while ((c = getopt_long(argc, argv, "haeqjx:o:O:b:l:", long_opts,
                          &opt_index)) != -1) {
    switch (c) {
    //--help
    case 'h': {
      ShowUsage();
      exit(0);
    } break;
    //--all
    case 'a': {
      opts.record_all = true;
    } break;
    //--regex
    case 'e': {
      opts.regex = true;
    } break;
    //--quiet
    case 'q': {
      opts.quiet = true;
    } break;
    //--bz2
    case 'j': {
      if (opts.compression != Uncompressed) {
        std::cerr << "Can only use one type of compression" << std::endl;
        exit(1);
      }
      opts.compression = BZ2;
    } break;
    //--exclude
    case 'x': {
      opts.do_exclude = true;
      // opts.exclude_regex = ::optarg;
    } break;
    //--output-prefix
    case 'o': {
      opts.prefix = ::optarg;
      opts.append_date = true;
    } break;
    //--output-name
    case 'O': {
      opts.prefix = ::optarg;
      opts.append_date = false;
    } break;
    //--buffsize
    case 'b': {
      std::stringstream ss(::optarg);
      int m = -1;
      if ((ss >> m).fail()) {
        std::cout << "use --buffsize <size>,size is number." << std::endl;
        exit(1);
      };
      if (m < 0) {
        std::cerr << "Buffer size must be 0 or positive" << std::endl;
        exit(1);
      }
      opts.buffer_size = 1048576 * m;
    } break;
    //--limit
    case 'l': {
      std::stringstream ss(::optarg);
      if ((ss >> opts.limit).fail()) {
        std::cout << "use -l NUM --limit=NUM ,NUM should be number."
                  << std::endl;
        exit(1);
      }
    } break;
    //--publish
    case 'p': {
      opts.publish = true;
    } break;
    //--min-space
    case 'L': {
      std::string ms = ::optarg;
      long long int value = 1073741824ull;
      char mul = 0;
      // Sane default values, just in case
      opts.min_space_str = "1G";
      opts.min_space = value;
      if (sscanf(ms.c_str(), " %lld%c", &value, &mul) > 0) {
        opts.min_space_str = ms;
        switch (mul) {
        case 'G':
        case 'g':
          opts.min_space = value * 1073741824ull;
          break;
        case 'M':
        case 'm':
          opts.min_space = value * 1048576ull;
          break;
        case 'K':
        case 'k':
          opts.min_space = value * 1024ull;
          break;
        default:
          opts.min_space = value;
          break;
        }
      }
      std::cout
          << "Rosbag using minimum space of << opts.min_space << bytes, or "
          << opts.min_space_str << std::endl;
    } break;
    //无参数的option
    case 0: {
      switch (lopt) {
      //--split
      case 0: {
        opts.split = true;
        int S = 0;
        std::stringstream ss(argv[::optind]);
        if (!(ss >> S).fail()) {
          std::cout << "Use of \"--split <MAX_SIZE>\" has been deprecated. "
                       "Please use --split --size <MAX_SIZE> or --split "
                       "--duration <MAX_DURATION>"
                    << std::endl;
          if (S < 0) {
            std::cerr << "Split size must be 0 or positive" << std::endl;
          }
          exit(1);
          // opts.max_size = 1048576 * S;
        }
      } break;
      //--lz4
      case 1: {
        if (opts.compression != Uncompressed) {
          std::cerr << "Can only use one type of compression" << std::endl;
          exit(1);
        }
        opts.compression = LZ4;
      } break;
      //--tcpnodelay
      case 2: {
        // opts.transport_hints.tcpNoDelay();
      } break;
      //--udp
      case 3: {
        // opts.transport_hints.udp();
      } break;
      // max-splits
      case 4: {
        if (!opts.split) {
          std::cout << "--max-splits is ignored without --split" << std::endl;
        } else {
          std::stringstream ss(::optarg);
          uint32_t max_splits = 0;
          if ((ss >> max_splits).fail()) {
            std::cout
                << "use --max-splits <MAX_SPLITS>, MAX_SPLITS should be number."
                << std::endl;
            exit(1);
          }
          opts.max_splits = max_splits;
        }
      } break;
      //--size
      case 5: {
        std::stringstream ss(::optarg);
        int size = -1;
        if ((ss >> size).fail()) {
          std::cout << "use --size <SIZE>, SIZE should be number." << std::endl;
          exit(1);
        }
        opts.max_size = size * 1048576;
        if (opts.max_size <= 0) {
          std::cerr << "Split size must be 0 or positive" << std::endl;
          exit(1);
        }
      } break;
      //--duration
      case 6: {
        std::string duration_str = ::optarg;

        double duration;
        double multiplier = 1.0;
        std::string unit("");

        std::istringstream iss(duration_str);
        if ((iss >> duration).fail()) {
          std::cerr << "Duration must start with a floating point number."
                    << std::endl;
          exit(1);
        }

        if ((!iss.eof() && ((iss >> unit).fail()))) {
          std::cerr << "Duration unit must be s, m, or h" << std::endl;
          exit(1);
        }
        if (unit == std::string(""))
          multiplier = 1.0;
        else if (unit == std::string("s"))
          multiplier = 1.0;
        else if (unit == std::string("m"))
          multiplier = 60.0;
        else if (unit == std::string("h"))
          multiplier = 3600.0;
        else
          std::cerr << "Duration unit must be s, m, or h" << std::endl;

        opts.max_duration = TimeDuration(duration * multiplier);
        if (opts.max_duration <= TimeDuration(0)) {
          std::cerr << "Duration must be positive." << std::endl;
          exit(1);
        }
      } break;
      //--chunksize
      case 7: {
        std::stringstream ss(::optarg);
        int chnk_sz = -1;
        if ((ss >> chnk_sz).fail()) {
          std::cout << "--chunksize <SIZE>, SIZE should be number."
                    << std::endl;
          exit(1);
        }
        if (chnk_sz < 0) {
          std::cerr << "Chunk size must be 0 or positive" << std::endl;
          exit(1);
        }
        opts.chunk_size = 1024 * chnk_sz;
      } break;
      // node
      case 8: {
        opts.node = ::optarg;
        std::cout << "Recording from: " << opts.node << std::endl;
      } break;
      //--topic
      case 9: {
        std::cout << "record topic" << std::endl;
      } break;
      default: { std::cout << "error option." << std::endl; } break;
      }
    }
    default: { std::cout << "error option." << std::endl; } break;
    }
  }
  // check that argument combinations make sense
  // TODO:实现exclude_regex
  // if (opts.exclude_regex.size() > 0 && !(opts.record_all || opts.regex)) {
  //   fprintf(stderr, "Warning: Exclusion regex given, but no topics to
  //                   subscribe to.\n "
  //                                   "Adding implicit 'record all'.");
  //   opts.record_all = true;
  // }

  // Every non-option argument is assumed to be a topic
  if (!opts.record_all) {
    while (optind < argc) {
      opts.topics.push_back(argv[optind++]);
    }
  }
}

void ProgExit(int signo) {
  LOG_CRITICAL("program recv signal [{}],to exit.", signo);
  if (recorder_ptr) {
    recorder_ptr->Stop();
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
      conf.GetRecorderLoggerName(), conf.GetRecorderLogFile(),
      conf.GetLogfileMaxSize(), conf.GetMaxLogfileNum());
  LOG_DEBUG("conf ip:{}", conf.GetIp());
  LOG_DEBUG("conf port:{}", conf.GetPort());
  LOG_DEBUG("conf buffer_size:{}", conf.GetBufferSize());
  LOG_DEBUG("conf logger_name:{}", conf.GetRecorderLoggerName());
  LOG_DEBUG("conf log_file:{}", conf.GetRecorderLogFile());
  LOG_DEBUG("conf max_logfile_size:{}", conf.GetLogfileMaxSize());
  LOG_DEBUG("conf max_logfile_num:{}", conf.GetMaxLogfileNum());
  LOG_INFO("msgbag recorder start!");
  RecorderOptions opts;
  ParseOption(argc, argv, opts);

  LOG_DEBUG("RecorderOptions");
  LOG_DEBUG(
      "trigger:{},record_all:{},regex:{},do_exclude:{},quiet:{},append_date:{}",
      opts.trigger, opts.record_all, opts.regex, opts.do_exclude, opts.quiet,
      opts.append_date);
  LOG_DEBUG(
      "snapshot:{},verbose:{},publish:{},compression:{},prefix:{},filename:{}",
      opts.snapshot, opts.verbose, opts.publish, opts.compression, opts.prefix,
      opts.name);
  LOG_DEBUG("buffer_size:{},chunk_size:{},limit:{},split:{},max_size:{},max_"
            "splits:{}",
            opts.buffer_size, opts.chunk_size, opts.limit, opts.split,
            opts.max_size, opts.max_splits);
  LOG_DEBUG("max_duration:{},node:{},min-space:{}min-space-str:{}",
            opts.max_duration.count(), opts.node, opts.min_space,
            opts.min_space_str);
  LOG_DEBUG("record topics number:{}", opts.topics.size());
  for (auto topic : opts.topics) {
    LOG_DEBUG("topic:{}", topic);
  }

  Recorder recorder(opts, &conf);
  recorder_ptr = &recorder;
  int result = recorder.Run();
  LOG_INFO("Record process stopped,exit_code:{}", result);
  return result;
}
