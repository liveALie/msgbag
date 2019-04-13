#include "msgbag_conf.h"
#include "cpptoml.h"
#include "log/logger.h"
namespace nullmax {
namespace msgbag {

bool MsgbagConf::LoadConf(const char *file) {
  std::shared_ptr<cpptoml::table> table;
  try {
    table = cpptoml::parse_file(file);
  } catch (const cpptoml::parse_exception &e) {
    std::cout << "parse_file error:" << e.what() << std::endl;
    return false;
  }

  auto Strings = [&](const char *key, const char *value) -> std::string {
    if (table->contains_qualified(key)) {
      return table->get_qualified(key)->as<std::string>()->get();
    }
    if (!value) {
      return std::string();
    }
    return std::string(value);
  };

  auto Integer = [&](const char *key, int value) -> int {
    if (table->contains_qualified(key)) {
      auto integer = table->get_qualified(key)->as<int64_t>()->get();
      return static_cast<int>(integer);
    }
    return value;
  };

  auto Boolean = [&](const char *key, bool value) -> bool {
    if (table->contains_qualified(key)) {
      auto boolean = table->get_qualified(key)->as<bool>()->get();
      return static_cast<bool>(boolean);
    }
    return value;
  };
  ip_ = Strings("ip", "0.0.0.0");
  port_ = Integer("port", 10086);
  buffer_size_ = Integer("buffer_size", 10240);
  r_logger_name_ = Strings("recorder.logger_name", "recorder");
  r_log_file_ = Strings("recorder.log_file", "recorder.log");
  p_logger_name_ = Strings("player.logger_name", "player");
  p_log_file_ = Strings("player.log_file", "player.log");
  logfile_max_size_ = Integer("max_size", 102400);
  max_logfile_num_ = Integer("max_files", 3);
  return true;
}

} // namespace msgbag
} // namespace nullmax
