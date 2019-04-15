#pragma once

#include <assert.h>
#include <memory>
#include <mutex>
namespace nullmax {
namespace msgbag {

template <class T> class Singleton {
public:
  template <typename... Args> static inline T *GetInstance(Args &&... args) {
    static std::mutex init_mtx_;
    std::unique_lock<std::mutex> lck(init_mtx_);
    if (nullptr == instance_) {
      instance_ = std::make_shared<T>(std::forward<Args>(args)...);
    }
    return instance_.get();
  }

  static T *Instance() {
    assert(instance_ != nullptr);
    return instance_.get();
  }

protected:
  Singleton(void) {}
  virtual ~Singleton(void) {}

private:
  Singleton(const Singleton &) {}
  Singleton &operator=(const Singleton &rhs) {}

  static std::shared_ptr<T> instance_;
};

template <class T> std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

} // namespace msgbag
} // namespace nullmax