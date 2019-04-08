#include "logger.h"
#include "recorder.h"
#include <getopt.h>
#include <iostream>
#include <sstream>

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
    {"min-space",required_argument,NULL,'L'},
    {"topic",no_argument,&lopt,9},
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
                          &opt_index)) != -1)
  {
    switch (c) {
    case 'h': {
      ShowUsage();
      exit(0);
    } break;
    case 'a': {
      opts.record_all = true;
    } break;
    case 'e': {
      opts.regex = true;
    } break;
    case 'q': {
      opts.quiet = true;
    } break;
    case 'j': {

    } break;
    case 'x': {
      opts.do_exclude = true;
      // opts.exclude_regex = ::optarg;
    } break;
    case 'o': {
      opts.prefix = ::optarg;
      opts.append_date = true;
    } break;
    case 'O': {
      opts.prefix = ::optarg;
      opts.append_date = false;
    } break;
    case 'b': {
      std::stringstream ss(::optarg);
      int m = -1;
      ss >> m;
      if (m < 0) {
        std::cerr << "Buffer size must be 0 or positive" << std::endl;
        exit(1);
      }
      opts.buffer_size = 1048576 * m;
    } break;
    case 'l': {
      std::stringstream ss(::optarg);
      // ss >> opts.limit；
    } break;
    case 0: {
      switch (lopt) {
      case 0: {
        // opts.split = true;

        // int S = vm["split"].as<int>();
        // if (S != 0)
        // {
        //   ROS_WARN("Use of \"--split <MAX_SIZE>\" has been deprecated. Please
        //   use --split --size <MAX_SIZE> or --split --duration
        //   <MAX_DURATION>"); if (S < 0)
        //     throw ros::Exception("Split size must be 0 or positive");
        //   opts.max_size = 1048576 * S;
        // }
        std::cout << "--split" << std::endl;
      } break;
      case 1: {

      } break;
      case 2: {

      } break;
      case 3: {

      } break;
      case 4: {
        // if (!opts.split)
        // {
        //   ROS_WARN("--max-splits is ignored without --split");
        // }
        // else
        // {
        //   opts.max_splits = vm["max-splits"].as<int>();
        // }
      } break;
      case 5: {
        std::stringstream ss(::optarg);
        int size = -1;
        ss >> size;
        opts.max_size = size * 1048576;
        if (opts.max_size <= 0) {
          std::cerr << "Split size must be 0 or positive" << std::endl;
          exit(1);
        }
      } break;
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

        opts.max_duration =
            std::chrono::duration<double>(duration * multiplier);
        if (opts.max_duration <= std::chrono::duration<double>(0)) {
          std::cerr << "Duration must be positive." << std::endl;
          exit(1);
        }
      } break;
      case 7: {
        std::stringstream ss(::optarg);
        int chnk_sz = -1;
        ss >> chnk_sz;
        if (chnk_sz < 0) {
          std::cerr << "Chunk size must be 0 or positive" << std::endl;
          exit(1);
        }
        opts.chunk_size = 1024 * chnk_sz;
      } break;
      case 8: {
        opts.node = ::optarg;
        std::cout << "Recording from: " << opts.node << std::endl;
      } break;
      default: { } break; }
    }
    default: { } break; }

    // if (vm.count("publish"))
    //   opts.publish = true;
    // if (vm.count("min-space"))
    // {
    //     std::string ms = vm["min-space"].as<std::string>();
    //     long long int value = 1073741824ull;
    //     char mul = 0;
    //     // Sane default values, just in case
    //     opts.min_space_str = "1G";
    //     opts.min_space = value;
    //     if (sscanf(ms.c_str(), " %lld%c", &value, &mul) > 0) {
    //         opts.min_space_str = ms;
    //         switch (mul) {
    //             case 'G':
    //             case 'g':
    //                 opts.min_space = value * 1073741824ull;
    //                 break;
    //             case 'M':
    //             case 'm':
    //                 opts.min_space = value * 1048576ull;
    //                 break;
    //             case 'K':
    //             case 'k':
    //                 opts.min_space = value * 1024ull;
    //                 break;
    //             default:
    //                 opts.min_space = value;
    //                 break;
    //         }
    //     }
    //     ROS_DEBUG("Rosbag using minimum space of %lld bytes, or %s",
    //     opts.min_space, opts.min_space_str.c_str());
    // }
    // if (vm.count("bz2") && vm.count("lz4"))
    // {
    //   throw ros::Exception("Can only use one type of compression");
    // }
    // if (vm.count("bz2"))
    // {
    //   opts.compression = rosbag::compression::BZ2;
    // }
    // if (vm.count("lz4"))
    // {
    //   opts.compression = rosbag::compression::LZ4;
    // }
    // if (vm.count("duration"))
    // {

    // }
    // if (vm.count("size"))
    // {

    // }

    // if (vm.count("tcpnodelay"))
    // {
    //   opts.transport_hints.tcpNoDelay();
    // }
    // if (vm.count("udp"))
    // {
    //   opts.transport_hints.udp();
    // }
  }
  // Every non-option argument is assumed to be a topic
  while (optind < argc) {
    opts.topics.push_back(argv[optind++]);
  }
  // check that argument combinations make sense
  // if(opts.exclude_regex.size() > 0 &&
  //           !(opts.record_all || opts.regex)) {
  //       fprintf(stderr, "Warning: Exclusion regex given, but no topics to
  //       subscribe to.\n"
  //               "Adding implicit 'record all'.");
  //       opts.record_all = true;
  //   }
}

int main(int argc, char *argv[]) {
  INIT_LOGGER("msgbag_recorder", "recorder.log", 1024 * 512, 3);
  LOG_INFO("msgbag recorder start!");
  RecorderOptions opts;
  ParseOption(argc, argv, opts);
  Recorder recorder(opts);
  recorder.Run();
  return 0;
}
