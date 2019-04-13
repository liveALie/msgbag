
#include <gtest/gtest.h>
#include "spdlog/logger.h"
#include "spdlog/sinks/rotating_file_sink.h"

TEST(spdlog,print){
    auto logger = spdlog::rotating_logger_mt("logger","test.log",10240,3);
    for(int i = 0;i < 10; ++i){
        logger->info("this is:{}",i);
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}