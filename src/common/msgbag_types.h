#ifndef _MSGBAG_TYPES_H_
#define _MSGBAG_TYPES_H_

#include "common/timestamp.h"
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

class MessageRecord {
public:
  MessageRecord(long long timestamp, const std::string &topic,
                const std::string &serialized_msg)
      : timestamp_(timestamp), topic_(topic), serialized_msg_(serialized_msg) {}

  bool operator<(const MessageRecord &record) {
    return this->timestamp_ < record.timestamp_;
  }

  Timestamp timestamp_;
  std::string topic_;
  std::string serialized_msg_;
};

typedef std::shared_ptr<MessageRecord> MessageRecordPtr;

enum BagMode { Write = 1, Read = 2, Append = 4 };

typedef std::function<void()> TimerCallback;

typedef std::multimap<Timestamp, MessageRecordPtr> BagRecordsType;

} // namespace msgbag
} // namespace nullmax

#endif
