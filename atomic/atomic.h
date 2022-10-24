/**
 * @file atomic.h
 * @author Jamishon
 * @brief Atomic of gcc and asm
 * @version 0.1
 * @date 2022-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <bits/atomic_base.h>

namespace atomic {

template <typename T, typename B>
struct Atomic {
  T t_;
  B atomic_base_;

  Atomic() = default;
  ~Atomic() = default;
  Atomic(const Atomic&) = delete;
  Atomic operator=(const Atomic&) = delete;

  constexpr Atomic(T t) : t_(t) {}

  operator T();
  T operator=(T t);
  T operator++(int);
  T operator++();
  T operator--(int);
  T operator--();

  T Load(T* t);
  void Store(T value);
  T FetchAdd(T value);
  T AddFetch(T value);
  T FetchSub(T value);
  T SubFetch(T value);

  T Inc();
  T Dec();

  T CompareAndSet(T exp, T value);
  T Exchange(T* value);
};

template <typename T>
struct AtomicBase;

template <typename T>
struct Atomic<T, AtomicBase<T>> {
  T t_;
  AtomicBase<T> atomic_base_;

  Atomic() = default;
  ~Atomic() = default;
  Atomic(const Atomic&) = delete;
  Atomic operator=(const Atomic&) = delete;

  constexpr Atomic(T t) : t_(t) {}

  operator T() { return atomic_base_.atomic_load(&t_); }

  T operator=(T t) {
    atomic_base_.atomic_store(&t_, t);
    return t;
  }

  T operator++(int) {
    T temp = t_;
    atomic_base_.atomic_inc(&t_);
    return t_;
  }

  T operator++() {
    atomic_base_.atomic_inc(&t_);
    return t_;
  }

  T operator--(int) {
    T temp = t_;
    atomic_base_.atomic_dec(&t_);
    return temp;
  }

  T operator--() {
    atomic_base_.atomic_dec(&t_);
    return t_;
  }

  T Load() { return atomic_base_.atomic_load(&t_); }

  void Store(T value) { atomic_base_.atomic_store(&t_, value); }

  T FetchAdd(T value) { return atomic_base_.atomic_fetch_add(&t_, value); }

  T AddFetch(T value) { return atomic_base_.atomic_add_fetch(&t_, value); }

  T FetchSub(T value) { return atomic_base_.atomic_fetch_sub(&t_, value); }

  T SubFetch(T value) { return atomic_base_.atomic_sub_fetch(&t_, value); }

  T Inc() { return atomic_base_.atomic_inc(&t_); }

  T Dec() { return atomic_base_.atomic_dec(&t_); }

  T CompareAndSet(T exp, T value) {
    return atomic_base_.atomic_compare_and_set(&t_, exp, value);
  }

  T Exchange(T* value) { return atomic_base_.atomic_exchange(&t_, value); }


};

template<typename T> struct AtomicBaseGccCall;

template <typename T>
struct Atomic<T, AtomicBaseGccCall<T>> {
  T t_;
  AtomicBaseGccCall<T> atomic_base_;

  Atomic() = default;
  ~Atomic() = default;
  Atomic(const Atomic&) = delete;
  Atomic operator=(const Atomic&) = delete;

  constexpr Atomic(T t) : t_(t) {}

  operator T() { return atomic_base_.Load(&t_); }

  T operator=(T t) { return atomic_base_.Store(&t_, t); }

  T operator++(int) { return atomic_base_.FetchAdd(&t_, 1); }

  T operator++() { return atomic_base_.AddFetch(&t_, 1); }

  T operator--(int) { return atomic_base_.FetchSub(&t_, 1); }

  T operator--() { return atomic_base_.SubFetch(&t_, 1); }

  T Load() { return atomic_base_.Load(&t_); }

  void Store(T value) { atomic_base_.Store(&t_, value); }

  T FetchAdd(T value) { return atomic_base_.FetchAdd(&t_, value); }

  T AddFetch(T value) { return atomic_base_.AddFetch(&t_, value); }

  T FetchSub(T value) { return atomic_base_.FetchSub(&t_, value); }

  T SubFetch(T value) { return atomic_base_.SubFetch(&t_, value); }

  T Inc() { return atomic_base_.AddFetch(&t_, 1); }

  T Dec() { return atomic_base_.SubFetch(&t_, 1); }

  T CompareAndSet(T exp, T value) {
    return atomic_base_.CompareAndSet(&t_, exp, value);
  }

  T Exchange(T* value) { return atomic_base_.Exchange(&t_, value); }
};

template <typename T>
struct AtomicBase {
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
        : [res] "=a"(res), /* output */
          [dst] "=m"(*dst)
        : [src] "r"(src), /* input */
          "a"(exp), "m"(*dst)
        : "memory"); /* no-clobber list */

    return exp;
  }

  T atomic_exchange(T* dst, T* val) {
    asm volatile(
        "lock;"
        "xchgl %0, %1;"
        : "=r"(*val), "=m"(*dst)
        : "0"(*val), "m"(*dst)
        : "memory"); /* no-clobber list */
    return *val;
  }
};

template <typename T>
struct AtomicBaseGccCall {
  T Load(T* t) {
    T temp;
    __atomic_load(t, &temp, std::memory_order_seq_cst);
    return temp;
  }

  T Store(T* t, T value) {
    __atomic_store(t, &value, std::memory_order_seq_cst);
    return value;
  }

  T FetchAdd(T* t, T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_fetch_add(t, value, m);
  }

  T AddFetch(T* t, T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_add_fetch(t, value, m);
  }

  T FetchSub(T* t, T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_fetch_sub(t, value, m);
  }

  T SubFetch(T* t, T value, std::memory_order m = std::memory_order_seq_cst) {
    return __atomic_sub_fetch(t, value, m);
  }

  T CompareAndSet(
      T* t, T exp, T value,
      std::memory_order success_order = std::memory_order_acquire,
      std::memory_order failed_order = std::memory_order_acquire) noexcept {
    __atomic_compare_exchange(t, &exp, &value, 0, success_order, failed_order);
    return exp;
  }

  T CompareAndSet(T* t, T exp, T value,
                  std::memory_order success_order = std::memory_order_acquire,
                  std::memory_order failed_order =
                      std::memory_order_acquire) volatile noexcept {
    __atomic_compare_exchange(t, &exp, &value, 0, success_order, failed_order);
    return exp;
  }

  T Exchange(T* t, T* value, std::memory_order m = std::memory_order_seq_cst) {
    T temp;
    __atomic_exchange(t, value, &temp, m);
    return temp;
  }
};
};  // namespace atomic