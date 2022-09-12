/**
 * @file timer.h
 * @author Jamishon
 * @brief Timer
 * @version 0.1
 * @date 2022-09-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <atomic>

struct Timer;

enum TimerStatus { STOP = 0, PENDING, RUNNING, CONFIG };

enum TimerType { SINGLE = 0, PERIODICAL };

typedef void (*timer_callback)(Timer* timer, void* param);

struct Timer {
  // Timer(uint64_t expire, timer_callback callback, void* param = NULL,
  // uint64_t period = 0) :
  //  expire_(expire), callback_(callback), param_(param), period_(period) {

  // }

  void RunTask() { callback_(this, param_); }

  uint64_t expire_;
  uint64_t period_;
  std::atomic<TimerStatus> status_;
  timer_callback callback_;
  void* param_;
};

#endif