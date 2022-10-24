/**
 * @file atomic.h
 * @author Jamishon
 * @brief Atomic
 * @version 0.1
 * @date 2022-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <bits/atomic_base.h>

namespace atomic {

template <typename T>
struct Atomic {
  T t_;

  Atomic() = default;
  ~Atomic() = default;
  Atomic(const Atomic&) = delete;
  Atomic operator=(const Atomic&) = delete;

  constexpr Atomic(T t) : t_(t) {}

  T atomic_load(T* t) { return *(volatile T*)t; }

  void atomic_store(T* t, T value) { *t = value; }

  T atomic_fetch_add(T* t, T value) {
    T temp = *t;
    asm volatile(
        "lock;"
        "addl %1, %0"
        : "+m"(*t)
        : "ir"(value)
        : "memory");

    return temp;
  }

  T atomic_add_fetch(T* t, T value) {
    asm volatile(
        "lock;"
        "addl %1, %0"
        : "+m"(*t)
        : "ir"(value)
        : "memory");

    return *t;
  }

  T atomic_fetch_sub(T* t, T value) {
    T temp = *t;
    asm volatile(
        "lock;"
        "subl %1, %0"
        : "+m"(*t)
        : "ir"(value)
        : "memory");

    return temp;
  }

  T atomic_sub_fetch(T* t, T value) {
    asm volatile(
        "lock;"
        "subl %1, %0"
        : "+m"(*t)
        : "ir"(value)
        : "memory");

    return *t;
  }

  T atomic_inc(T* t) {
    asm volatile(
        "lock;"
        "incl %0"
        : "+m"(*t)::"memory");

    return *t;
  }

  T atomic_dec(T* t) {
    asm volatile(
        "lock;"
        "decl %0"
        : "+m"(*t)::"memory");

    return *t;    
  }

  T atomic_compare_and_set(T* dst, T exp, T src) {
    uint8_t res;

    asm volatile(
        "lock;"
        "cmpxchgl %[src], %[dst];"
        "sete %[res];"
        : [res] "=a" (res), /* output */
          [dst] "=m" (*dst)
        : [src] "r" (src), /* input */
          "a" (exp), 
          "m" (*dst)
        : "memory"); /* no-clobber list */

    return exp;
  }

  T atomic_exchange(T* dst, T val) {
    asm volatile(
        "lock;"
        "xchgl %0, %1;"
        : "=r"(val), "=m"(*dst)
        : "0"(val), "m"(*dst)
        : "memory"); /* no-clobber list */
    return val;
  }

  operator T() noexcept {
    T temp;
    __atomic_load(&t_, &temp, std::memory_order_seq_cst);
    return temp;
  }

  operator T() volatile noexcept {
    T temp;
    __atomic_load(&t_, &temp, std::memory_order_seq_cst);
    return temp;
  }

  T operator=(T i) noexcept {
    __atomic_store(&t_, &i, std::memory_order_seq_cst);
    return i;
  }

  T operator=(T i) volatile noexcept {
    __atomic_store(&t_, &i, std::memory_order_seq_cst);
    return i;
  }

  T operator++(int) noexcept {
    return __atomic_fetch_add(&t_, 1, std::memory_order_seq_cst);
  }

  T operator++() volatile noexcept {
    return __atomic_add_fetch(&t_, 1, std::memory_order_seq_cst);
  }

  T operator--(int) noexcept {
    return __atomic_fetch_sub(&t_, 1, std::memory_order_seq_cst);
  }

  T operator--() volatile noexcept {
    return __atomic_sub_fetch(&t_, 1, std::memory_order_seq_cst);
  }


  T FetchAdd(T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_fetch_add(&t_, value, m);
  }

  T AddFetch(T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_add_fetch(&t_, value, m);
  }

   T FetchSub(T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_fetch_sub(&t_, value, m);
  }

  T SubFetch(T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_sub_fetch(&t_, value, m);
  }
  

  T CompareAndSet(
      T exp, T value,
      std::memory_order success_order = std::memory_order_acquire,
      std::memory_order failed_order = std::memory_order_acquire) noexcept {
    __atomic_compare_exchange(&t_, &exp, &value, 0, success_order,
                              failed_order);
    return exp;
  }

  T CompareAndSet(T exp, T value,
                  std::memory_order success_order = std::memory_order_acquire,
                  std::memory_order failed_order =
                      std::memory_order_acquire) volatile noexcept {
    __atomic_compare_exchange(&t_, &exp, &value, 0, success_order,
                              failed_order);
    return exp;
  }
};

};  // namespace atomic