/**
 * @file timer_manager.cc
 * @author Jamishon
 * @brief Timer manager
 * @version 0.1
 * @date 2022-09-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <time.h>
#include <malloc.h>

#include "pthread.h"
#include "skiplist.h"
#include "timer.h"


class TimerManager {
 public:
  TimerManager() { pthread_spin_init(&lock_, 0); }

  ~TimerManager() { pthread_spin_destroy(&lock_); }

  int AddTimer(long expire, timer_callback callback, void* param) {
    Timer* t = (Timer*)malloc(sizeof(Timer));
    t->expire_ = expire;
    t->callback_ = callback;
    t->param_ = param;
    t->status_ = PENDING;

    AddTimer(t);
  }

  int AddTimer(Timer* timer) {
    if (timer == NULL) return -1;

    Node* n = list_.InitNode(timer->expire_, timer);

    pthread_spin_lock(&lock_);
    list_.AddNode(n);
    pthread_spin_unlock(&lock_);

    return 0;
  }

  int RemoveTimer(Timer* timer) {
    if (timer != NULL) return -1;

    Node* n = (Node*)malloc(sizeof(Node));
    n->data = timer;
    n->key = timer->expire_;

    pthread_spin_lock(&lock_);
    list_.RemoveNode(n);
    pthread_spin_unlock(&lock_);

    return 0;
  }

  void RunTimerTask() {
    long now = GetCurrentTime();

    Node* n = (Node*)malloc(sizeof(Node));
    n->data = NULL;
    n->key = now - 1;
    n->next = NULL;

    Node* prev[list_.MaxDepth()];
    Node* head;
    Node* first;

    pthread_spin_lock(&lock_);

    list_.GetPrevNode(n, prev);
    head = list_.GetHead();
    first = head->next[0];

    for(int i=list_.CurrentDepth(); i>=0; i--) {
      head->next[i] = prev[i]->next[i];
      prev[i]->next[i] = NULL;

      if(head->next[i] == NULL ) list_.MinusCurrentDepth();
    }

    pthread_spin_unlock(&lock_);

    Node* next = first;
    Node* cur = NULL;
    while (next != NULL) {
      cur = next;
      Timer* t = (Timer*)cur->data;
      if (t != NULL) {
        t->status_ = RUNNING;
        t->RunTask();
        t->status_ = STOP;
      }

      next = cur->next[0];

      free(cur->data);
      free(cur);
    }

    
  }

  static long GetCurrentTime() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_nsec;
  }

  //   /**
  //    * @brief The result of rdtsc is CPU cycle, that could be converted to
  //    nanoseconds by a simple calculation. ns = CPU cycles * (ns_per_sec / CPU
  //    freq)
  //    *
  //    * @return uint64_t
  //    */
  //   static uint64_t x86_64_rdtsc(void) {
  //     uint64_t var;
  //     uint32_t hi, lo;

  //     __asm volatile("rdtsc" : "=a"(lo), "=d"(hi));

  //     var = ((uint64_t)hi << 32) | lo;
  //     return (var);
  //   }

  //   /** Read generic counter */
  //   static uint64_t arm64_cntvct(void) {
  //     uint64_t tsc;

  //     asm volatile("mrs %0, cntvct_el0" : "=r"(tsc));
  //     return tsc;
  //   }

 private:
  SkipList list_;
  pthread_spinlock_t lock_;
};

#endif