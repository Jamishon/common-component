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
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "atomic.h"

int main() {
  {
    atomic::Atomic<int, atomic::AtomicBase<int>> atomic_i(0);

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

    std::cout << std::endl;
  }

  {
    atomic::Atomic<int, atomic::AtomicBaseGccCall<int>> atomic_i(0);

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

    std::cout << std::endl;
  }


  {
    atomic::Atomic<int, atomic::AtomicBase<int>> a(0);
    int t = 0;

    std::cout << (int)a.Load() << std::endl;
    a.Store(10);
    std::cout << (int)a.Load() << std::endl;

    std::cout << (int)a.AddFetch(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.FetchAdd(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.FetchSub(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.SubFetch(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;

    std::cout << std::endl;

    std::cout << (int)a.CompareAndSet(10, 15) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    int e = 17;
    std::cout << (int)a.Exchange(&e) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.Inc() << std::endl;
    std::cout << (int)a.Dec() << std::endl;

    std::cout << std::endl;

    std::cout << (int)a.FetchAdd(2) << std::endl;
    std::cout << (int)a << std::endl;
    std::cout << (int)a.AddFetch(2) << std::endl;
    std::cout << (int)a << std::endl;
    std::cout << (int)a.FetchSub(2) << std::endl;
    std::cout << (int)a << std::endl;
    std::cout << (int)a.SubFetch(2) << std::endl;
    std::cout << (int)a << std::endl;

    std::cout << std::endl;
  }

  {
    atomic::Atomic<int, atomic::AtomicBaseGccCall<int>> a(0);
    int t = 0;

    std::cout << (int)a.Load() << std::endl;
    a.Store(10);
    std::cout << (int)a.Load() << std::endl;

    std::cout << (int)a.AddFetch(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.FetchAdd(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.FetchSub(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.SubFetch(2) << std::endl;
    std::cout << (int)a.Load() << std::endl;

    std::cout << std::endl;

    std::cout << (int)a.CompareAndSet(10, 15) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    int e = 17;
    std::cout << (int)a.Exchange(&e) << std::endl;
    std::cout << (int)a.Load() << std::endl;
    std::cout << (int)a.Inc() << std::endl;
    std::cout << (int)a.Dec() << std::endl;

    std::cout << std::endl;

    std::cout << (int)a.FetchAdd(2) << std::endl;
    std::cout << (int)a << std::endl;
    std::cout << (int)a.AddFetch(2) << std::endl;
    std::cout << (int)a << std::endl;
    std::cout << (int)a.FetchSub(2) << std::endl;
    std::cout << (int)a << std::endl;
    std::cout << (int)a.SubFetch(2) << std::endl;
    std::cout << (int)a << std::endl;
  }
}