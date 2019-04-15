#pragma once
#include <iostream>
#include <memory>
namespace nullmax {
namespace msgbag {
class MsgbagConf {
public:
  bool LoadConf(const char *file);

  std::string GetIp() { return ip_; }

  int GetPort() { return port_; }

  unsigned int GetBufferSize() { return buffer_size_; }

  std::string GetRecorderLoggerName() { return r_logger_name_; }

  std::string GetRecorderLogFile() { return r_log_file_; }

  std::string GetPlayerLoggerName() { return p_logger_name_; }

  std::string GetPlayerLogFile() { return p_log_file_; }

  unsigned int GetLogfileMaxSize() { return logfile_max_size_; }

  unsigned int GetMaxLogfileNum() { return max_logfile_num_; }

private:
  std::string ip_;
  unsigned int port_;
  unsigned int buffer_size_;
  std::string r_logger_name_;
  std::string r_log_file_;
  std::string p_logger_name_;
  std::string p_log_file_;
  unsigned int logfile_max_size_;
  unsigned int max_logfile_num_;
};
} // namespace msgbag
} // namespace nullmax
