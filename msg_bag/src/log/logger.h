#ifndef _MSGBAG_LOGGER_H_
#define _MSGBAG_LOGGER_H_
#include "common/singleton.h"
#include "sinks/basic_file_sink.h"
#include "sinks/rotating_file_sink.h"
#include "spdlog.h"
#include <iostream>
namespace nullmax {
namespace msgbag {

static void SpdlogErrCallback(const std::string &err_msg) {
  std::cout << "SpdlogErrCallback err_msg:" << err_msg << std::endl;
}

class MyLogger {
public:
  MyLogger(const std::string &logger_name, const std::string &filename,
           int max_size, int max_files) {
    logger_ =
        spdlog::rotating_logger_mt(logger_name, filename, max_size, max_files);
    assert(logger_ != nullptr);
    logger_->set_error_handler(SpdlogErrCallback);
    logger_->set_pattern("[thread-%t]%+");
    logger_->set_level(spdlog::level::debug);
  }

  std::shared_ptr<spdlog::logger> GetLogger() { return logger_; }

private:
  std::shared_ptr<spdlog::logger> logger_;
};

#define LOG_TRACE(fmt, ...)                                                    \
  { Singleton<MyLogger>::Instance()->GetLogger()->trace(fmt, ##__VA_ARGS__); }

#define LOG_DEBUG(fmt, ...)                                                    \
  { Singleton<MyLogger>::Instance()->GetLogger()->debug(fmt, ##__VA_ARGS__); }

#define LOG_INFO(fmt, ...)                                                     \
  { Singleton<MyLogger>::Instance()->GetLogger()->info(fmt, ##__VA_ARGS__); }

#define LOG_WARN(fmt, ...)                                                     \
  { Singleton<MyLogger>::Instance()->GetLogger()->warn(fmt, ##__VA_ARGS__); }

#define LOG_ERROR(fmt, ...)                                                    \
  { Singleton<MyLogger>::Instance()->GetLogger()->error(fmt, ##__VA_ARGS__); }

#define LOG_CRITICAL(fmt, ...)                                                 \
  {                                                                            \
    Singleton<MyLogger>::Instance()->GetLogger()->critical(fmt,                \
                                                           ##__VA_ARGS__);     \
  }

} // namespace msgbag
} // namespace nullmax

#endif
