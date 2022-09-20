/**
 * @file log.h
 * @author Jamishon
 * @brief Log
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <pthread.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../threadpool/task_queue.h"

enum LogLevel { DEBUG = 0, INFO, WARNING, ERROR, CRASH };

typedef void* (*thread_fun)(void* param);

class Log {
 public:
  Log(LogLevel level = DEBUG, std::string outputpath = "")
      : level_(level), outputpath_(outputpath), queue_(1024) {}

  ~Log() {}

  void Start() {
    to_file_ = !outputpath_.empty();
    running = true;

    pthread_create(&thread_, NULL, Log::ThreadFun, this);
  }

  void Stop() {
    queue_.ClearUp();
    running = false;
  }

  static void* ThreadFun(void* param) {
    Log* plog = static_cast<Log*>(param);
    if (!plog) return NULL;

    if (plog->to_file_) {
      plog->file_.open(plog->outputpath_);
    }

    while (plog->running) {
      std::string* log = plog->queue_.Pop();
      if(!log) continue;

      if (plog->to_file_)
        plog->file_ << *log << std::endl;
      else
        std::cout << *log << std::endl;
    }

    plog->file_.close();
  }

  void Print(LogLevel level, std::string* log) {
    if (level < level_) return;

    queue_.Push(log);
  }

  pthread_t LogThread() {
    return thread_;
  }

 private:
  LogLevel level_;
  bool running;

  pthread_t thread_;
  TaskQueue<std::string> queue_;
  std::string outputpath_;
  std::ofstream file_;
  bool to_file_;
};