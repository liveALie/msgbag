#include "thread_pool.h"
namespace nullmax {
namespace msgbag {
ThreadPool::ThreadPool(int num_threads) : queue_(MAX_TASK_COUNT) {
  Start(num_threads);
}

ThreadPool::~ThreadPool() { Stop(); }

void ThreadPool::Stop() {
  std::call_once(flag_, [this] { StopThreadPool(); });
}
void ThreadPool::AddTask(Task &&task) { queue_.Put(std::forward<Task>(task)); }

void ThreadPool::AddTask(const Task &task) { queue_.Put(task); }

void ThreadPool::Start(int num_threads) {
  running_ = true;
  for (int i = 0; i < num_threads; ++i) {
    thread_group_.push_back(
        std::make_shared<std::thread>(std::bind(&ThreadPool::RunThread, this)));
  }
}

void ThreadPool::RunThread() {
  while (running_) {
    std::list<Task> list;
    queue_.Take(list);
    for (auto &task : list) {
      if (!running_) {
        return;
      }
      task();
    }
  }
}

void ThreadPool::StopThreadPool() {
  queue_.Stop();
  running_ = false;

  for (auto &thread : thread_group_) {
    if (thread) {
      thread->join();
    }
  }
  thread_group_.clear();
}
} // namespace msgbag
} // namespace nullmax
