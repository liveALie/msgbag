#pragma once

#include "publish_subscribe.h"
#include <atomic>
#include <mutex>
#include <string>

namespace nullmax {
namespace msgbag {

class PubSub {
public:
  PubSub(const std::string &ip, const int port, unsigned int buffer_size);
  void Init(const std::string &ip, const int port, unsigned int buffer_size);
  void Subscribe(const std::string &topic, CallbackFunc callback);
  template <typename T> void Publish(const std::string &topic, const T &msg);

private:
  const static unsigned int BUFFER_LENGTH = 10240;
  std::mutex subscribe_mtx_, publish_mtx_;
  unsigned int msg_id_;
  std::string err_msg_;
  char buffer_[BUFFER_LENGTH] = {0};
  PublishSubscribe pub_sub_;
};

template <typename T>
void PubSub::Publish(const std::string &topic, const T &msg) {
  std::unique_lock<std::mutex> lck(publish_mtx_);
  if (msg.ByteSizeLong() > BUFFER_LENGTH) {
    std::cout << "error data_buf_ size is not enough msg size:"
              << msg.ByteSizeLong() << std::endl;
  } else {
    msg.SerializePartialToArray(buffer_, msg.ByteSizeLong());
    pub_sub_.Publish(topic, buffer_, msg.ByteSizeLong(), msg_id_++, err_msg_);
  }
}
} // namespace msgbag
} // namespace nullmax