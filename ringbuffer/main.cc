/**
 * @file main.cc
 * @author Jamishon
 * @brief Ring buffer without sync lock
 * @version 0.1
 * @date 2022-08-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>

#include <thread>

#include "ring_buffer.h"

using namespace ringbuffer;

int main() {
  printf(
      "\n\ncase "
      "1:-------------------------------------------------------------\n\n");

  RingBuffer rb;
  int r_id = rb.CreateRing(256, 1, sizeof(int));

  int array_1[8];
  int array_2[16];
  int array_3[32] = {0};

  for (int& i : array_1) {
    i = 0x1111;
  }

  for (int& i : array_2) {
    i = 0x2222;
  }

  std::thread t1([&array_1, &rb, r_id]() {
    rb.Enqueue(r_id, array_1, sizeof(array_1) / sizeof(int));

    for (int i : array_1) {
      printf("%x\n", i);
    }
  });

  std::thread t2([&array_2, &rb, r_id]() {
    rb.Enqueue(r_id, array_2, sizeof(array_2) / sizeof(int));

    for (int i : array_2) {
      printf("%x\n", i);
    }
  });

  if(t1.joinable()) t1.join();
  if(t2.joinable()) t2.join();

  rb.Dequeue(r_id, array_3, sizeof(array_3) / sizeof(int));

  for (int i : array_3) {
    printf("%x\n", i);
  }

  printf(
      "\n\ncase "
      "2:-------------------------------------------------------------\n\n");
  struct Foo {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
  };

  uint32_t r_id_1 = rb.CreateRing(32, 1, sizeof(Foo));

  std::thread t3([&rb, r_id_1]() {
    Foo array_foo_1[6];
    for (Foo& f : array_foo_1) f = {0x01, 0x01, 0x01, 0x01};

    rb.Enqueue(r_id_1, array_foo_1, sizeof(array_foo_1) / sizeof(Foo));
  });

  std::thread t4([&rb, r_id_1]() {
    Foo array_foo_2[7];
    for (Foo& f : array_foo_2) f = {0x02, 0x02, 0x02, 0x02};

    rb.Enqueue(r_id_1, array_foo_2, sizeof(array_foo_2) / sizeof(Foo));
  });

  std::thread t5([&rb, r_id_1]() {
    Foo array_foo_3[8];
    for (Foo& f : array_foo_3) f = {0x03, 0x03, 0x03, 0x03};

    rb.Enqueue(r_id_1, array_foo_3, sizeof(array_foo_3) / sizeof(Foo));
  });

  std::thread t6([&rb, r_id_1]() {
    Foo array_foo_4[9];
    for (Foo& f : array_foo_4) f = {0};

    rb.Dequeue(r_id_1, array_foo_4, sizeof(array_foo_4) / sizeof(Foo));

    printf("\n");
    for (Foo& f : array_foo_4) {
      printf("%x %x %x %x\n", f.u8, f.u16, f.u32, f.u64);
    }
  });

  std::thread t7([&rb, r_id_1]() {
    Foo array_foo_5[10];
    for (Foo& f : array_foo_5) f = {0};

    rb.Dequeue(r_id_1, array_foo_5, sizeof(array_foo_5) / sizeof(Foo));

    printf("\n");
    for (Foo& f : array_foo_5) {
      printf("%x %x %x %x\n", f.u8, f.u16, f.u32, f.u64);
    }
  });

  if (t3.joinable()) t3.join();
  if (t4.joinable()) t4.join();
  if (t5.joinable()) t5.join();
  if (t6.joinable()) t6.join();
  if (t7.joinable()) t7.join();

  Foo array_foo_1[32];
  for (Foo& f : array_foo_1) f = {0};
  rb.Dequeue(r_id_1, array_foo_1, sizeof(array_foo_1) / sizeof(Foo));

  printf("\n");
  for (Foo& f : array_foo_1) {
    printf("%x %x %x %x\n", f.u8, f.u16, f.u32, f.u64);
  }



  Foo array_foo_3[7];
}