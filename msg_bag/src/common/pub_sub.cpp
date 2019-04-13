#include "pub_sub.h"

namespace nullmax {
namespace msgbag {

PubSub::PubSub(const std::string &ip, const int port, unsigned int buffer_size)
    : msg_id_(0), err_msg_(""), pub_sub_(ip, port, buffer_size) {}

void PubSub::Subscribe(const std::string &topic, CallbackFunc callback) {
  std::unique_lock<std::mutex> lck(subscribe_mtx_);
  pub_sub_.Subscribe(topic, callback, err_msg_);
}
} // namespace msgbag
} // namespace nullmax