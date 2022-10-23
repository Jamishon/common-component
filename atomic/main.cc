/**
 * @file main.cc
 * @author Jamishon
 * @brief  Test case on Atomic
 * @version 0.1
 * @date 2022-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include <thread>
#include <vector>

#include "atomic.h"

int main() {
  atomic::Atomic<int> atomic_i(0);

  atomic_i = 1;

  std::cout << ((int)(atomic_i++)) << std::endl;
  std::cout << (int)++atomic_i << std::endl;
  std::cout << (int)atomic_i-- << std::endl;
  std::cout << (int)--atomic_i << std::endl;

  atomic_i = 0;
  int i = 0;
  int max_num = 2;

  auto la_1 = [&atomic_i] {
    int n = 100000;
    while (n--) atomic_i++;
  };

  auto la_2 = [&i] {
    int k = 100000;
    while (k--) i++;
  };

  std::vector<std::thread> vc_t1;
  std::vector<std::thread> vc_t2;

  for (int i = 0; i < max_num; i++) {
    vc_t1.emplace_back(la_1);
    vc_t2.emplace_back(la_2);
  }

  for (int i = 0; i < max_num; i++) {
    vc_t1[i].join();
    vc_t2[i].join();
  }

  std::cout << (int)atomic_i << std::endl;
  std::cout << i << std::endl;
}