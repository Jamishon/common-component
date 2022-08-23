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
  ring_buffers_.erase(ring_id);
}

Ring* GetRing(uint32_t ring_id) {
  std::lock_guard<std::mutex> guard(mtx);
  Ring* r = ring_buffers_.at(ring_id);
  if (r == NULL) return NULL;

  return r;
}

int Enqueue(void* data, int num) { return 0; }

int Dequeue(void* data, int num) { return 0; }
