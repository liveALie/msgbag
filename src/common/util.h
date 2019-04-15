#ifndef _UTIL_H_
#define _UTIL_H_

#include "common/timestamp.h"
#include "msgbag_types.h"

namespace nullmax {
namespace msgbag {

class Recorder;
class Player;

extern std::map<std::string, HandlerFunc> recorder_handler_map;
extern std::map<std::string, HandlerFunc> player_handler_map;

extern void InitRecorderHandlerMap(Recorder *recorder);
extern void InitPlayerHandlerMap(Player *player);

extern uint32_t WriteHeader(const HeaderType &header, Buffer &buffer);
extern uint32_t ReadHeader(HeaderType &header, Buffer &buffer);

extern std::string HexToString(const unsigned char hex);

extern int CreateTimerfd();

extern struct timespec HowMuchTimeFromNow(Timestamp when);

extern void ReadTimerfd(int timerfd, Timestamp now);

extern void ResetTimerfd(int timerfd, Timestamp expiration);

} // namespace msgbag
} // namespace nullmax

#endif