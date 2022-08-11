/**
 * @file executer.cc
 * @author Jamishon
 * @brief
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "task_queue.h"

#define MAX_QUEUE_SIZE 1024

typedef std::function<void(void*)> Callback;

struct Task {
  Callback callback_;
  void* param_;
};

class Executor {
 public:
  Executor(int thread_num)
      : kThreadNum_(thread_num), running_(false), task_queue_(MAX_QUEUE_SIZE) {}

  ~Executor() {
    std::cout << "~Executor" << std::endl;
  }

  void Execute() {
    running_ = true;
    for (int i = 0; i < kThreadNum_; i++) {
      std::thread t(&Executor::Loop, this);
      std::thread::id t_id = t.get_id();
      threads_.push_back(std::move(t));
      thread_ids_.push_back(t_id);
    }
  }

  void Stop() {
    task_queue_.ClearUp();
    running_ = false;

    for (auto t = threads_.begin(); t != threads_.end(); t++) {
      if (t->joinable()) t->join();
    }
  }

  void Loop() {
    std::thread::id t_id = std::this_thread::get_id();
    std::ostringstream oss;
     oss << t_id;
    //thread_ids_.push_back(t_id); //Add the thread id into the std::vector may cause crash
     printf("%s executing...\n", oss.str().c_str());

    while (running_) {
      Task* task = task_queue_.Pop();
      
      if (task != NULL) {
        task->callback_(task->param_);
        delete task;
      }
    }

    printf("%s end...\n", oss.str().c_str());
  }

  void PostTask(std::function<void(void*)> func, void* param) {
    Task* ptask = new Task{func, param};

    task_queue_.Push(ptask);

    printf("Push() TaskQueue size : %d\n", task_queue_.Size());
  }

 private:
  const int kThreadNum_;
  std::vector<std::thread> threads_;
  std::vector<std::thread::id> thread_ids_;
  TaskQueue<Task> task_queue_;
  bool running_;
};