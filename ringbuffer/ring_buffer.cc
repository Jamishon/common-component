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

#include <atomic>

using namespace ringbuffer;

uint32_t RingBuffer::assign_id_ = 0;

RingBuffer::RingBuffer() {}

RingBuffer::~RingBuffer() {
  for(auto& t : ring_buffers_) {
    delete std::get<1>(t);
  }
}

int RingBuffer::CreateRing(uint32_t size, int flag, uint8_t unit_byte) {
  Ring* r = NULL;
  uint32_t request_size = sizeof(Ring) + size * unit_byte;
  void* all = malloc(request_size);
  if (all == NULL) return -1;

  r = (Ring*)all;
  r->addr = all;
  r->unit_byte = unit_byte;
  r->capacity = size - 1;
  r->mask = size - 1;
  r->size = size;
  r->flags = flag;
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
  std::lock_guard<std::mutex> guard(mtx_);
  Ring* r = ring_buffers_.at(ring_id);
  if (r == NULL) return -1;
  free(r);
  r = NULL;
  ring_buffers_.erase(ring_id);
}

Ring* RingBuffer::GetRing(uint32_t ring_id) {
  std::lock_guard<std::mutex> guard(mtx_);
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

  result = UpdateProducerHead(r, num, &old_head, &new_head);

  if (result < 1) return result;

  DataEnqueue(r, data, old_head, result, size);

#ifdef DEBUG_
  printf("\nEnqueue, prod.head:%u, prod.tail:%u, old_head:%u, new_head:%u, num:%d\n",
         r->prod.head, r->prod.tail, old_head, new_head, result);
#endif         

  UpdateProducerTail(r, &old_head, &new_head);

  return result;
}

int RingBuffer::Dequeue(uint32_t ring_id, void* data, int num) {
  Ring* r = GetRing(ring_id);
  if (r == NULL) return -1;

  int result = 0;
  uint32_t old_head = 0;
  uint32_t new_head = 0;

  uint8_t size = sizeof(void*);

  result = UpdateConsumerHead(r, num, &old_head, &new_head);

  if (result < 1) return result;

  DataDequeue(r, data, old_head, result, size);

#ifdef DEBUG_
  printf("\nDequeue, cons.head:%u, cons.tail:%u, old_head:%u, new_head:%u, num:%d\n",
         r->prod.head, r->prod.tail, old_head, new_head, result);
#endif         

  UpdateConsumerTail(r, &old_head, &new_head);

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
    if (produce_num > free_num) produce_num = free_num;

    *new_head = *old_head + produce_num;

    success = AutomicCompareAndSwap((volatile uint32_t*)&r->prod.head, old_head,
                                    new_head);
  } while (success == 0);

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

    success = AutomicCompareAndSwap(&r->cons.head, old_head, new_head);

  } while (success == 0);

  return cons_num;
}

int RingBuffer::UpdateProducerTail(Ring* r, uint32_t* old_value,
                                   uint32_t* new_value) {
  if (r == NULL) return -1;

  WriteMemoryBarrier();

#ifdef DEBUG_
  printf(
      "\nUpdateProducerTail, prod.head:%u, prod.tail:%u, old_value:%u, "
      "new_value:%u\n",
      r->prod.head, r->prod.tail, *old_value, *new_value);
#endif      

  while (AtomicLoad(&r->prod.tail) != *old_value) {
  }

  r->prod.tail = *new_value;

  return 0;
}

int RingBuffer::UpdateConsumerTail(Ring* r, uint32_t* old_value,
                                   uint32_t* new_value) {
  if (r == NULL) return -1;

  ReadMemoryBarrier();

#ifdef DEBUG_
  printf(
      "\nUpdateConsumerTail, cons.head:%u, cons.tail:%u, old_value:%u, "
      "new_value:%u\n",
      r->cons.head, r->cons.tail, *old_value, *new_value);
#endif      

  while (AtomicLoad(&r->cons.tail) != *old_value) {
  }

  r->cons.tail = *new_value;
}

int RingBuffer::DataEnqueue(Ring* r, void* src, uint32_t old_head, uint32_t num,
                            uint8_t byte_) {
  if (r == NULL || src == NULL || num == 0) return -1;

  uint32_t c = r->capacity;
  uint32_t index_dest = old_head;
  uint32_t index_src = 0;
  uint32_t unit_byte = r->unit_byte;

  void* start = &r[1];
  byte* pindex_dest = NULL;
  byte* pindex_src = (byte*)src;
  int i = 0;
  int in = 0;

  for (i = 0; i < num; i++) {
    pindex_dest = (byte*)start + (index_dest++ % c) * unit_byte;

    for (in = 0; in < unit_byte; in++) {
      *(pindex_dest + in) = *(pindex_src + in);
    }
    pindex_src += unit_byte;
  }

  return num;
}

int RingBuffer::DataDequeue(Ring* r, void* dest, uint32_t old_head,
                            uint32_t num, uint8_t byte_) {
  if (r == NULL || dest == NULL || num == 0) return -1;

  uint32_t c = r->capacity;
  uint32_t unit_byte = r->unit_byte;
  uint32_t index_src = old_head;
  void* start = &r[1];
  byte* pindex_src = NULL;
  byte* pindex_dest = (byte*)dest;
  int i = 0;
  int in = 0;

  for (i = 0; i < num; i++) {
    pindex_src = (byte*)start + (index_src++ % c) * unit_byte;

    for (in = 0; in < unit_byte; in++) {
      *(pindex_dest + in) = *(pindex_src + in);
    }
    pindex_dest += unit_byte;
  }

  return num;
}

void RingBuffer::ReadMemoryBarrier() {
  __sync_synchronize();
}

void RingBuffer::WriteMemoryBarrier() {
  do {
    asm volatile("mfence" : : : "memory");
  } while (0);
  
}

bool RingBuffer::AutomicCompareAndSwap(volatile uint32_t* old_value,
                                       uint32_t* expt_value,
                                       uint32_t* new_value) {
  return __sync_bool_compare_and_swap(old_value, *expt_value, *new_value);
}

uint32_t RingBuffer::AtomicLoad(volatile uint32_t* value) {

  return __sync_fetch_and_add(value, 0);
}