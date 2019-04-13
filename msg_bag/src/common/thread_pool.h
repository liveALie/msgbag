#ifndef _C11TEST_THREADPOOL_HPP_
#define _C11TEST_THREADPOOL_HPP_

#include "sync_queue.h"
#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <thread>

namespace nullmax
{
namespace msgbag
{
const int MAX_TASK_COUNT = 100;
class ThreadPool
{
public:
  using Task = std::function<void()>;
  ThreadPool(int num_threads = std::thread::
                 hardware_concurrency());

  ~ThreadPool();

  void Stop();
  void AddTask(Task &&task);

  void AddTask(const Task &task);

private:
  void Start(int num_threads);
  void RunThread();
  void StopThreadPool();

private:
  std::list<std::shared_ptr<std::thread>> thread_group_; //线程池
  SyncQueue<Task> queue_;                                //任务队列
  atomic_bool running_;                                  //用于标志线程池开始和结束的标志位
  std::once_flag flag_;                                  //用于call_once的标志位
};
} // namespace msgbag
} // namespace nullmax

#endif // _C11TEST_THREADPOOL_HPP_