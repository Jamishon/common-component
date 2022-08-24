/**
 * @file ring_buffer.cc
 * @author Jamishon
 * @brief Ring buffer without sync lock
 * @version 0.1
 * @date 2022-08-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ring_buffer.h"

#include <pthread.h>
#include <unistd.h>

using namespace ringbuffer;

static uint32_t RingBuffer::assign_id_ = 0;

RingBuffer::RingBuffer() {}

RingBuffer::~RingBuffer() {}

int RingBuffer::CreateRing(std::string name, uint32_t size, int flag,
                           uint8_t unit_byte) {
  Ring* r = NULL;
  uint32_t request_size = sizeof(Ring) + size * unit_byte;
  void* all = malloc(request_size);
  if (r == NULL) return -1;

  r->addr = all;
  r->capacity = size - 1;
  r->mask = size - 1;
  r->size = size;
  r->flags = flag;
  r->name = name;
  r->prod.head = 0;
  r->prod.tail = 0;
  r->prod.type = MT;
  r->cons.head = 0;
  r->cons.tail = 0;
  r->prod.type = MT;
  r->ring_id = assign_id_++;

  std::lock_guard<std::mutex> guard(mtx_);

  ring_buffers_.emplace(r->ring_id, r);

  return r->ring_id;
}

// int InitRing(Ring* r) { return 0; }
int RingBuffer::ResetRing(uint32_t ring_id) {
  Ring* r = NULL;
  {
    std::lock_guard<std::mutex> guard(mtx_);

    r = ring_buffers_.at(ring_id);
  }
  if (r == NULL) return -1;

  r->prod.head = 0;
  r->prod.tail = 0;
  r->cons.head = 0;
  r->cons.tail = 0;

  return 0;
}

int RingBuffer::FreeRing(uint32_t ring_id) {
  std::lock_guard<std::mutex> guard(mtx);
  Ring* r = ring_buffers_.at(ring_id);
  if (r == NULL) return -1;
  free(r);
  r = NULL;
  ring_buffers_.erase(ring_id);
}

Ring* RingBuffer::GetRing(uint32_t ring_id) {
  std::lock_guard<std::mutex> guard(mtx);
  Ring* r = ring_buffers_.at(ring_id);
  if (r == NULL) return NULL;

  return r;
}

int RingBuffer::Enqueue(uint32_t ring_id, void* data, int num) {
  Ring* r = GetRing(ring_id);
  if (r == NULL) return -1;

  int result = 0;
  uint32_t old_head = 0;
  uint32_t new_head = 0;

  uint8_t size = sizeof(void*);

  UpdateProducerHead(r, num, &old_head, &new_head);

  result = DataEnqueue(&r[1], data, num);

  UpdateProducerTail(r, num, &old_head, &new_head);

  return result;
}

int RingBuffer::Dequeue(uint32_t ring_id, void* data, int num) {
  Ring* r = GetRing(ring_id);
  if (r == NULL) return -1;

  int result = 0;

  uint8_t size = sizeof(void*);

  UpdateConsumerHead(r, num, size);

  result = DataEnqueue(data, &r[1], num);

  UpdateConsumerTail(r, num, size);

  return result;
}

int RingBuffer::UpdateProducerHead(Ring* r, uint32_t num, uint32_t* old_head,
                                   uint32_t* new_head) {
  int produce_num = 0;
  int success = 0;
  int free_num = 0;
  uint32_t capacity = r->capacity;
  do {
    produce_num = num;

    *old_head = r->prod.head;

    ReadMemoryBarrier();

    free_num = capacity - *old_head + r->cons.tail;
    if (num > free_num) {
      produce_num = free_num;
    }
    *new_head = *old_head + produce_num;

    success = CompareAndExchange(&r->prod.head, old_head, new_head);
  } while (success == 0)

      return produce_num;
}

int RingBuffer::UpdateConsumerHead(Ring* r, uint32_t num, uint32_t* old_head,
                                   uint32_t* new_head) {
  uint32_t capacity = r->capacity;
  uint32_t cons_num = 0;
  uint32_t exist_num = 0;
  int success = 0;
  do {
    cons_num = num;

    *old_head = r->cons.head;

    ReadMemoryBarrier();

    exist_num = r->prod.tail - *old_head;
    if (exist_num < cons_num) cons_num = exist_num;
    *new_head = *old_head + cons_num;

    success = CompareAndExchange(&r->cons.head, old_head, new_head);

  } while (success == 0);

  return cons_num;
}

int RingBuffer::UpdateProducerTail(Ring* r, uint32_t* old_value,
                                   uint32_t* new_value) {
  if (r == NULL) return -1;

  WriteMemoryBarrier();

  while (AutomicLoad(r->prod.tail) != *old_value) {
  }

  r->prod.tail == *new_value;

  return 0;
}

int RingBuffer::UpdateConsumerTail(Ring* r, int num, uint32_t* old_value,
                                   uint32_t* new_value) {
  if (r == NULL) return -1;

  ReadMemoryBarrier();

  while (AutomicLoad(r->cons.tail) != *old_value) {
  }

  r->cons.tail = new_value;
}

int RingBuffer::DataEnqueue(Ring* r, void* dest, void* src, uint32_t num) {
  if (r == NULL || dest == NULL || src == NULL || num == 0) return -1;

  int cur_pos = r->prod.tail % r->size;
  // void* index = dest + cur_pos;

  for (int i = 0; i < num && cur_pos < r->size; i++, cur_pos++) {
    dest[cur_pos] = src[i];
  }

  return num;
}

int RingBuffer::DataDequeue(Ring* r, void* dest, void* src, uint32_t num) {
  if (r == NULL || dest == NULL || src == NULL || num == 0) return -1;

  int cur_pos = r->cons.tail % r->size;
  for (int i = 0; i < num && cur_pos < r->size; i++, cur_pos++) {
    dest[i] = src[i];
  }

  return num;
}

void RingBuffer::ReadMemoryBarrier() { __sync_synchronize(); }

void RingBuffer::WriteMemoryBarrier() {
  do {
    asm volatile("dmb st" : : : "memory");
  } while (0)
}

bool RingBuffer::CompareAndExchange(void* old_value, void* expt_value,
                                    void* new_value) {
  // return __sync_val_compare_and_swap(old_value, expt_value, new_value);
  return __atomic_compare_exchange(old_value, expt_value, new_value, 0,
                                   __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE)
             ? 1
             : 0;
}

uint32_t RingBuffer::AutomicLoad(void* value) {
  return __automic_load_n(value, __ATOMIC_RELAXED);
}