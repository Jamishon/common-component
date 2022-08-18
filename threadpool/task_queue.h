/**
 * @file task_queue.h
 * @author Jamishon
 * @brief Blocking queue with limited size.
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class TaskQueue {
 public:
  TaskQueue(int max) : kMaxSize_(max), valid_(true) {}

  void ClearUp() {
    Clear();
    valid_ = false;
    cond_not_empty_.notify_all();
    cond_not_full_.notify_all();
  }

  void Clear() {
    std::lock_guard<std::mutex> guard(mtx_);
    while (!queue_.empty()) {
      T* pt = queue_.front();
      queue_.pop();
      delete pt;
    }
  }

  bool Empty() {
    std::lock_guard<std::mutex> guard(mtx_);
    return queue_.empty();
  }

  bool Full() {
    std::lock_guard<std::mutex> guard(mtx_);
    return (queue_.size() >= kMaxSize_);
  }

  int Size() {
    std::lock_guard<std::mutex> guard(mtx_);
    return queue_.size();
  }

  bool Push(T* t) {
    std::unique_lock<std::mutex> guard(mtx_);
    while (queue_.size() >= kMaxSize_) {
      if (valid_)
        cond_not_full_.wait(guard);
      else
        return false;
    }
    queue_.push(t);
    cond_not_empty_.notify_one();

    return true;
  }

  T* Pop() {
    std::unique_lock<std::mutex> guard(mtx_);
    while (queue_.empty()) {
      if (valid_) {
        cond_not_empty_.wait(guard);
      } else {
        return NULL;
      }
    }
    if (!queue_.empty()) {
      T* pt = queue_.front();
      queue_.pop();
      cond_not_full_.notify_one();
      return pt;
    } else {
      return NULL;
    }
  }

 private:
  std::mutex mtx_;
  std::condition_variable cond_not_empty_;
  std::condition_variable cond_not_full_;
  std::queue<T*> queue_;
  const int kMaxSize_;
  bool valid_;
};