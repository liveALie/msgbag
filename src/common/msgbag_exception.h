#ifndef _MSGBAG_EXCEPTION_
#define _MSGBAG_EXCEPTION_
#include <exception>
#include <string>
namespace nullmax {
namespace msgbag {
class MsgbagException : public std::exception {
public:
  explicit MsgbagException(const std::string &str) : msg_(str) {}
  virtual ~MsgbagException() {}
  virtual const char *what() const _GLIBCXX_USE_NOEXCEPT {
    return msg_.c_str();
  }

private:
  std::string msg_;
};
} // namespace msgbag
} // namespace nullmax

#endif