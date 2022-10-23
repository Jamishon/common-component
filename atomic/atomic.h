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
};

};  // namespace atomic