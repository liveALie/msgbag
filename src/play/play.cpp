#include <iostream>
#include <getopt.h>
static int lopt = -1;
static struct option long_opts[] = {
  { "help", no_argument, NULL, 'h' },
  { "all", no_argument, NULL, 'a' },
  { "regex", no_argument, NULL, 'e' },
  { "quiet", no_argument, NULL, 'q' },
  { "split", no_argument, &lopt, 0 },
  { "bz2", no_argument, NULL, 'j' },
  { "lz4", no_argument, &lopt, 1 },
  { "tcpnodelay", no_argument, &lopt, 2 },
  { "udp", no_argument, &lopt, 3 },
  { "exclude", required_argument, NULL, 'x' },
  { "output-prefix", required_argument, NULL, 'o' },
  { "output-name", required_argument, NULL, 'O' },
  { "max-splits", required_argument, &lopt, 4 },
  { "size", required_argument, &lopt, 5 },
  { "duration", required_argument, &lopt, 6 },
  { "buffsize", required_argument, NULL, 'b' },
  { "chunksize", required_argument, &lopt, 7 },
  { "limit", required_argument, NULL, 'l' },
  { "node", required_argument, &lopt, 8 },
  { 0, 0, 0, 0 }
};


void ShowUsage() {
  std::cout <<"Usage: msgbag --record TOPIC1 [TOPIC2 TOPIC3 ...]." << std::endl;
  std::cout << std::endl;
  std::cout <<"Record a bag file with the contents of specified topics." << std::endl;
  std::cout << std::endl;
  std::cout <<"Options:" << std::endl;
  std::cout <<"  -h, --help            show this help message and exit" << std::endl;
  std::cout <<"  -a, --all             record all topics" << std::endl;
  std::cout <<"  -e, --regex           match topics using regular expressions" << std::endl;
  std::cout <<"  -x EXCLUDE_REGEX, --exclude=EXCLUDE_REGEX" << std::endl;
  std::cout <<"                        exclude topics matching the follow regular expression" << std::endl;
  std::cout <<"                        (subtracts from -a or regex)" << std::endl;
  std::cout <<"  -q, --quiet           suppress console output" << std::endl;
  std::cout <<"  -o PREFIX, --output-prefix=PREFIX" << std::endl;
  std::cout <<"                        prepend PREFIX to beginning of bag name (name will" << std::endl;
  std::cout <<"                        always end with date stamp)" << std::endl;
  std::cout <<"  -O NAME, --output-name=NAME" << std::endl;
  std::cout <<"                        record to bag with name NAME.bag" << std::endl;
  std::cout <<"  --split               split the bag when maximum size or duration is reached" << std::endl;
  std::cout <<"  --max-splits=MAX_SPLITS" << std::endl;
  std::cout <<"                        Keep a maximum of N bag files, when reaching the" << std::endl;
  std::cout <<"                        maximum erase the oldest one to keep a constant number" << std::endl;
  std::cout <<"                        of files." << std::endl;
  std::cout <<"  --size=SIZE           record a bag of maximum size SIZE MB. (Default:" << std::endl;
  std::cout <<"                        infinite)" << std::endl;
  std::cout <<"  --duration=DURATION   record a bag of maximum duration DURATION in seconds," << std::endl;
  std::cout <<"                        unless 'm', or 'h' is appended." << std::endl;
  std::cout <<"  -b SIZE, --buffsize=SIZE" << std::endl;
  std::cout <<"                        use an internal buffer of SIZE MB (Default: 256, 0 =" << std::endl;
  std::cout <<"                        infinite)" << std::endl;
  std::cout <<"  --chunksize=SIZE      Advanced. Record to chunks of SIZE KB (Default: 768)" << std::endl;
  std::cout <<"  -l NUM, --limit=NUM   only record NUM messages on each topic" << std::endl;
  std::cout <<"  --node=NODE           record all topics subscribed to by a specific node" << std::endl;
  std::cout <<"  -j, --bz2             use BZ2 compression" << std::endl;
  std::cout <<"  --lz4                 use LZ4 compression" << std::endl;
  std::cout <<"  --tcpnodelay          Use the TCP_NODELAY transport hint when subscribing to" << std::endl;
  std::cout <<"                        topics." << std::endl;
  std::cout <<"  --udp                 Use the UDP transport hint when subscribing to topics." << std::endl;
}
 
int main(int argc, char* argv[]) {
  ShowUsage();
  return 0;
}
