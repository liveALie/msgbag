// hello_test.cpp

// 必须包含 googletest 的头文件
#include <gtest/gtest.h>
#include <iostream>
#include <time.h>
#include <poll.h>
#include <sys/timerfd.h>
#include <thread>
#include <unistd.h>

// TEST(Time,test){
//   while(100){
//     struct timespec ts;
//     clock_gettime(CLOCK_REALTIME,&ts);
//     std::cout << ts.tv_sec << "." << ts.tv_nsec << std::endl;
//   }
// }


TEST(Timer,Poll){
  int fd = timerfd_create(CLOCK_REALTIME,TFD_NONBLOCK | TFD_CLOEXEC);
  if(fd < 0){
    std::cout << "timer create error." << std::endl;
    return;
  }
  struct pollfd* poll_fds;
  struct ::pollfd poll_fd;
  poll_fd.fd = fd;
  poll_fd.events = poll_fd.events | POLLIN;
  // poll_fds.push_back(poll_fd);
  poll_fds = &poll_fd;
  std::thread t([=]{
    while(1){
      int read = poll(poll_fds,1,1);
      if(read <= 0)
        continue;
      for(int i = 0; i < read; ++i){
        if(poll_fds[i].revents & POLLIN){
          uint64_t temp = 0;
          int ret = ::read(poll_fds[i].fd,(char*)&temp,sizeof(uint64_t));
          if(ret != sizeof(uint64_t)){
            std::cout << "read timer fd error." << std::endl;
            break;
          }
          std::cout << "do something." << std::endl;
          struct itimerspec newValue;
          struct itimerspec oldValue;
          bzero(&newValue, sizeof newValue);
          bzero(&oldValue, sizeof oldValue);
          static int expired = 1;
          struct timespec ts;
          ts.tv_sec = ++expired;
          ts.tv_nsec = 0;
          newValue.it_value = ts;
          ret = ::timerfd_settime(poll_fds[i].fd, 0, &newValue, &oldValue);
          if(ret < 0){
            std::cout << "timer settime error." << std::endl;
            break;
          }
        }
      }
    }
  });
  struct itimerspec newValue;
          struct itimerspec oldValue;
          bzero(&newValue, sizeof newValue);
          bzero(&oldValue, sizeof oldValue);
          struct timespec ts;
          ts.tv_sec = 1;
          ts.tv_nsec = 0;
          newValue.it_value = ts;
          int ret = ::timerfd_settime(fd, 0, &newValue, &oldValue);
          if(ret < 0){
            std::cout << "timer settime error." << std::endl;
          }
  t.join();
}

int main(int argc, char **argv) {
  
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

