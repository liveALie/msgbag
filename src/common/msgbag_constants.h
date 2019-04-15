#ifndef _MSGBAG_CONSTANTS_H_
#define _MSGBAG_CONSTANTS_H_

#include <string>

namespace nullmax {
namespace msgbag {

const static unsigned char FILE_HEADER = 0x01;

const static uint32_t FILE_HEADER_LEN = 1024;

const static std::string OP_FIELD_NAME = "op";
const static std::string INDEX_POS_FIELD_NAME = "index_pos";
const static std::string START_TIMESTAMP_FIELD_NAME = "start_time";
const static std::string END_TIMESTAMP_FIELD_NAME = "end_time";
const static std::string MSG_COUNT_FIELD_NAME = "msg_count";
} // namespace msgbag
} // namespace nullmax

#endif