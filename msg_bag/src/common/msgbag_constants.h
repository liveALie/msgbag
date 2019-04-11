#ifndef _MSGBAG_TYPES_H_
#define _MSGBAG_TYPES_H_

#include <chrono>
#include <common/buffer.h>
#include <functional>
#include <map>
#include <memory>

namespace nullmax {
namespace msgbag {

class Recorder;

typedef std::function<void(std::string)> HandlerFunc;

extern std::map<std::string, HandlerFunc> msg_handler_map;

typedef std::function<void(void)> LoopFunc;

typedef std::chrono::duration<double> TimeDuration;

typedef std::shared_ptr<Buffer> Buffer_t;

extern void InitHandlerMap(Recorder *recorder);

enum CompressionType {
  Uncompressed = 0,
  BZ2 = 1,
  LZ4 = 2,
};

enum BagMode { Write = 1, Read = 2, Append = 4 };

} // namespace msgbag
} // namespace nullmax

#endif
