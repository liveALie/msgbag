#ifndef _MSGBAG_TYPES_H_
#define _MSGBAG_TYPES_H_

#include <chrono>
#include <common/buffer.h>
#include <functional>
#include <map>
#include <memory>

namespace nullmax {
namespace msgbag {

typedef std::function<void(std::string)> HandlerFunc;

typedef std::function<void(void)> LoopFunc;

typedef std::chrono::duration<double> TimeDuration;

typedef std::shared_ptr<Buffer> MsgbagBuffer;

typedef std::map<std::string, std::string> HeaderType;

enum CompressionType {
  Uncompressed = 0,
  BZ2 = 1,
  LZ4 = 2,
};

enum BagMode { Write = 1, Read = 2, Append = 4 };

} // namespace msgbag
} // namespace nullmax

#endif
