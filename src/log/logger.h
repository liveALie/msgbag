#pragma once
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include <iostream>

std::shared_ptr<spdlog::logger> logger = nullptr;

void SpdlogErrCallback(const std::string &err_msg) {
  std::cout << "SpdlogErrCallback err_msg:" << err_msg << std::endl;
}

#define INIT_LOGGER(LOGGER_NAME, FILENAME, MAX_SIZE, MAX_FILES)                \
  {                                                                            \
    logger = spdlog::rotating_logger_mt(LOGGER_NAME, FILENAME, MAX_SIZE,       \
                                        MAX_FILES);                            \
    assert(logger != nullptr);                                                 \
    logger->set_error_handler(SpdlogErrCallback);                              \
    logger->set_pattern("[thread-%t]%+");                                      \
    logger->set_level(spdlog::level::debug);                                   \
  }

#define LOG_TRACE(fmt, ...)                                                    \
  { logger->trace(fmt, ##__VA_ARGS__); }

#define LOG_DEBUG(fmt, ...)                                                    \
  { logger->debug(fmt, ##__VA_ARGS__); }

#define LOG_INFO(fmt, ...)                                                     \
  { logger->info(fmt, ##__VA_ARGS__); }

#define LOG_WARN(fmt, ...)                                                     \
  { logger->warn(fmt, ##__VA_ARGS__); }

#define LOG_ERROR(fmt, ...)                                                    \
  { logger->error(fmt, ##__VA_ARGS__); }

#define LOG_CRITICAL(fmt, ...)                                                 \
  { logger->critical(fmt, ##__VA_ARGS__); }
