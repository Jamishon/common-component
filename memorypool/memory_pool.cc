/**
 * @file memory_pool.cc
 * @author Jamishon
 * @brief Implemation function if memory pool.
 * @version 0.1
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "memory_pool.h"

#include <malloc.h>

MemoryPool::MemoryPool() { InitPool(pool_head_); }

MemoryPool::~MemoryPool() {}

void MemoryPool::InitPool() {
  Pool* pool = (byte*)malloc(LARGE_SIZE);
  if (pool == NULL) return;

  pool->node.start = pool + sizeof(Pool);
  pool->node.end = pool + LARGE_SIZE;
  pool->node.last = pool->node.start;
  pool->node.next = NULL;

  pool->chain = NULL;
  pool->current = pool;
  pool->large = NULL;
  pool->min_large_division = LARGE_SIZE;

  pool_head_ = pool;
  pool_tail_ = &pool->node.next;
}

void MemoryPool::DestoryPool() {
  Pool* cur = NULL;
  LargeNode* large = NULL;

  while (pool_head_ != NULL) {
    cur = pool_head_;
    pool_head_ = cur->node.next;

    while (cur->large != NULL) {
      large = cur->large;
      cur->large = large->next;
      free(large->data);
      large->next = NULL;
      free(large);
    }

    cur->current = NULL;
    cur->chain = NULL;

    free(cur);
  }
}

void MemoryPool::ResetPool() {
  if (pool_head_ != NULL) {
    Pool* cur = NULL;
    Pool** ll = &pool_head_->node.next;
    LargeNode* large = NULL;

    while (*ll != NULL) {
      cur = *ll;
      ll = &cur->node.next;

      while (cur->large != NULL) {
        large = cur->large;
        cur->large = large->next;
        large->next = NULL;
        free(large->data);
        free(large);
      }

      cur->current = NULL;
      cur->chain = NULL;

      free(cur);
    }

    pool_head_->large = NULL;
    pool_head_->chain = NULL;
    pool_head_->current = pool_head_;

    pool_head_->node.last = pool_head_->node.start;
  }
}


