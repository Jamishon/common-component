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
#include <atomic>

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

  std::atomic<int> a_i;

  atomic::Atomic<int> a(0);
  int t = 0;
  
  std::cout << (int)a.atomic_load(&t) << std::endl;
  a.atomic_store(&t, 10);
  std::cout << (int)a.atomic_load(&t) << std::endl;

  std::cout << (int)a.atomic_add_fetch(&t, 2) << " " << (int)a.atomic_load(&t) << std::endl;
  std::cout << (int)a.atomic_fetch_add(&t, 2) << " " << (int)a.atomic_load(&t) << std::endl;
  std::cout << (int)a.atomic_fetch_sub(&t, 2) << " " << (int)a.atomic_load(&t) << std::endl;
  std::cout << (int)a.atomic_sub_fetch(&t, 2) << " " << (int)a.atomic_load(&t) << std::endl;

  std::cout << (int)a.atomic_compare_and_set(&t, 10, 15) << " " << (int)a.atomic_load(&t) << std::endl;
  std::cout << (int)a.atomic_exchange(&t, 17) << " " << (int)a.atomic_load(&t) << std::endl;
  std::cout << (int)a.atomic_inc(&t) << std::endl;
  std::cout << (int)a.atomic_dec(&t) << std::endl;


  std::cout << (int)a.FetchAdd(2) << " " << (int)a << std::endl;
  std::cout << (int)a.AddFetch(2) << " " << (int)a << std::endl;
  std::cout << (int)a.FetchSub(2) << " " << (int)a << std::endl;
  std::cout << (int)a.SubFetch(2) << " " << (int)a << std::endl;
}