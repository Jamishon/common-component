/**
 * @file main.cc
 * @author Jamishon
 * @brief Timer for run task on time.
 * @version 0.1
 * @date 2022-09-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

#include "skiplist.h"
#include "timer_manager.h"

void fun1(Timer* t, void* param) {
  int* p = (int*)param;
  (*p)++;

  printf("\nfun1 param:%d\n", *p);
}

void fun2(Timer* t, void* param) {
  int* p = (int*)param;
  (*p)--;

  printf("\nfun2 param:%d\n", *p);
}

int main() {
  printf("\ncase 1: --------------------------------------------\n");

  SkipList sl;
  int* data = (int*)malloc(sizeof(int));
  *data = 0xff;
  Node* n1 = sl.InitNode(3, data);
  Node* n2 = sl.InitNode(7, data);
  Node* n3 = sl.InitNode(9, data);
  Node* n4 = sl.InitNode(10, data);

  sl.AddNode(n2);
  sl.AddNode(n1);
  sl.AddNode(n4);
  sl.AddNode(n3);

  sl.RemoveNode(n1);
  sl.RemoveNode(n4);
  sl.RemoveNode(n3);

  printf("\ncase 2: --------------------------------------------\n");

  TimerManager tm;
  int i = 0;
  int n = 100;
  while (n--) {
    tm.AddTimer(tm.GetCurrentTime(), fun1, &i);
    tm.AddTimer(tm.GetCurrentTime(), fun2, &i);
  }
  tm.RunTimerTask();
}