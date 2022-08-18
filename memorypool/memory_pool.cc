/**
 * @file memory_pool.cc
 * @author Jamishon
 * @brief Implemation function of memory pool.
 * @version 0.1
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "memory_pool.h"

#include <malloc.h>

MemoryPool::MemoryPool() {}

MemoryPool::~MemoryPool() {}

size_t MemoryPool::InitPool(size_t init_size) {
  Pool* pool = (Pool*)malloc(init_size + sizeof(Pool));
  if (pool == NULL) return;

  pool->node.start = (byte*)pool + sizeof(Pool);
  pool->node.end = (byte*)pool + init_size + sizeof(Pool);
  pool->node.last = pool->node.start;
  pool->node.next = NULL;
  pool->node.failed_times = 0;

  pool->chain = NULL;
  // pool->current = pool;
  pool->large = NULL;
  pool->large_mark = init_size < LARGE_SIZE ? init_size : LARGE_SIZE;

  PoolList* list = (PoolList*)malloc(sizeof(PoolList));

  list->pool_head = pool;
  list->current = pool;
  list->pool_tail = &pool->node.next;

  mem_pools_.emplace_back(list);

  return mem_pools_.size() - 1;
}

void MemoryPool::DestoryPool(size_t pool_id) {
  Pool* pool_head = mem_pools_.at(pool_id)->pool_head;
  Pool* cur = NULL;
  LargeNode* large = NULL;

  while (pool_head != NULL) {
    cur = pool_head;
    pool_head = cur->node.next;

    while (cur->large != NULL) {
      large = cur->large;
      cur->large = large->next;
      free(large->buf);
      large->next = NULL;
      free(large);
    }

    // cur->current = NULL;
    cur->chain = NULL;

    free(cur);
  }
}

/**
 * @brief Free pool in the list except head, and free large list of the head.
 *
 */
void MemoryPool::ResetPool(size_t pool_id) {
  PoolList* list = mem_pools_.at(pool_id);
  Pool* pool_head = list->pool_head;
  Pool** pool_tail = list->pool_tail;
  if (pool_head != NULL) {
    Pool* cur = NULL;
    Pool** ll = &pool_head->node.next;
    LargeNode* large = NULL;

    while (*ll != NULL) {
      cur = *ll;
      ll = &cur->node.next;

      while (cur->large != NULL) {
        large = cur->large;
        cur->large = large->next;
        large->next = NULL;
        free(large->buf);
        free(large);
      }

      // cur->current = NULL;
      cur->chain = NULL;

      free(cur);
    }

    while (pool_head->large != NULL) {
      large = pool_head->large;
      pool_head->large = large->next;
      large->next = NULL;
      free(large->buf);
      free(large);
    }

    pool_head->large = NULL;
    pool_head->chain = NULL;
    // pool_head->current = pool_head;
    pool_head->node.last = pool_head->node.start;
    pool_head->node.next = NULL;
    pool_head->node.failed_times = 0;

    pool_tail = &pool_head->node.next;
  }

  list->current = list->pool_head;
}

void* MemoryPool::AllocateMemory(size_t pool_id, size_t size) {
  PoolList* list = mem_pools_[pool_id];
  Pool* head = list->pool_head;
  if (head != NULL) {
    if (size < head->large_mark)
      return AllocateMinMemory(pool_id, size);
    else
      return AllocateLargeMemory(pool_id, size);
  }

  return NULL;
}

void* MemoryPool::AllocateMinMemory(size_t pool_id, size_t size) {
  PoolList* list = mem_pools_[pool_id];
  Pool* cur = list->current;
  byte* start = NULL;

  while (cur) {
    if (size < (size_t)(cur->node.end - cur->node.last)) {
      start = cur->node.last;
      cur->node.last += size;
      return start;
    }

    if (cur->node.failed_times++ > 4 && cur->node.next != NULL)
      list->current = cur->node.next;

    cur = cur->node.next;
  }

  Pool** tail = list->pool_tail;
  cur = list->pool_head;
  size_t alloc_size = (size_t)(cur->node.end - cur->node.start);
  Pool* new_pool = (Pool*)malloc(alloc_size);
  if (new_pool == NULL) return NULL;

  new_pool->node.start = (byte*)new_pool + sizeof(Pool);
  new_pool->node.end = (byte*)new_pool + alloc_size;
  new_pool->node.last = new_pool->node.start;
  new_pool->node.next = NULL;

  // new_pool->current = new_pool;
  new_pool->chain = NULL;
  new_pool->large = NULL;
  new_pool->large_mark = alloc_size;

  start = new_pool->node.start;
  new_pool->node.last += size;

  *list->pool_tail = new_pool;
  list->pool_tail = &new_pool->node.next;

  return start;
}

void* MemoryPool::AllocateLargeMemory(size_t pool_id, size_t size) {
  PoolList* list = mem_pools_.at(pool_id);

  void* buf = malloc(size);
  if (buf == NULL) return NULL;

  LargeNode* large = list->pool_head->large;
  int i = 0;
  while (large) {
    if (large->buf == NULL) {
      large->buf = buf;
      return buf;
    }

    if (i++ > 3) break;
  }

  large = (LargeNode*)AllocateMinMemory(pool_id, sizeof(LargeNode));
  if (large == NULL) {
    free(large);
    return NULL;
  }

  large->buf = buf;
  large->next = list->pool_head->large;
  list->pool_head->large = large;

  return buf;
}

size_t MemoryPool::InitBlockChain(size_t pool_id, size_t init_size,
                                  size_t node_num) {
  BlockList* bl = (BlockList*)AllocateMinMemory(pool_id, sizeof(BlockList));
  if (bl == NULL) return 0;  // todo
  bl->current = NULL;
  bl->head_block = NULL;
  bl->tail_block = &bl->head_block;

  byte* block_buf = (byte*)AllocateMemory(pool_id, init_size * node_num);
  if (block_buf == NULL) return 0;  // todo

  for (int i = 0; i < node_num; i++) {
    ChainNode* pc = GetChainNode(pool_id);
    if (pc == NULL) return 0;  // todo;
    Block* b = (Block*)AllocateMemory(pool_id, sizeof(Block));
    if (b == NULL) return 0;  // todo
    b->start = block_buf;
    b->end = block_buf + init_size;
    b->cur = b->start;
    b->last = b->start;
    block_buf += init_size;

    pc->block = b;
    pc->next = NULL;

    *bl->tail_block = pc;
    bl->tail_block = &pc->next;
  }

  bl->current = bl->head_block;
  block_bufs_.push_back(bl);

  return block_bufs_.size() - 1;
}

MemoryPool::ChainNode* MemoryPool::GetChainNode(size_t pool_id) {
  PoolList* bl = mem_pools_[pool_id];
  ChainNode* pc = bl->pool_head->chain;
  if (pc != NULL) {
    bl->pool_head->chain = pc->next;
    return pc;
  }

  pc = (ChainNode*)AllocateMemory(pool_id, sizeof(ChainNode));
  if (pc == NULL) return NULL;
  pc->block = NULL;
  pc->next = NULL;

  return pc;
}

size_t MemoryPool::CopyBlockChain(size_t pool_id, size_t dest_chain_id,
                                  size_t src_chain_id) {
  BlockList* dest = block_bufs_[dest_chain_id];
  BlockList* src = block_bufs_[src_chain_id];
  ChainNode* cur = src->head_block;

  while (cur != NULL) {
    ChainNode* pc = GetChainNode(pool_id);
    if (pc == NULL) continue;
    pc->block = cur->block;
    pc->next = NULL;
    *dest->tail_block = pc;
    dest->tail_block = &pc->next;

    cur = cur->next;
  }

  return dest_chain_id;
}

MemoryPool::ChainNode* MemoryPool::GetFreeChainNode(size_t pool_id,
                                                    size_t free_chain_id) {
  BlockList* free = block_bufs_[free_chain_id];
  ChainNode* pc = NULL;
  if (free->head_block) {
    pc = free->head_block;
    free->head_block = pc->next;
    pc->next = NULL;

    return pc;
  }

  pc = GetChainNode(pool_id);
  if (pc == NULL) return NULL;

  Block* b = (Block*)AllocateMemory(pool_id, sizeof(Block));
  if (b = NULL) return NULL;
  pc->block = b;
  pc->next = NULL;

  return pc;
}

void MemoryPool::UpdateChain(size_t pool_id, size_t free_chain_id,
                             size_t busy_chain_id, size_t out_chain_id,
                             void* tag) {
  BlockList* free = block_bufs_[free_chain_id];
  BlockList* busy = block_bufs_[busy_chain_id];
  BlockList* out = block_bufs_[out_chain_id];
  Pool* cur = mem_pools_[pool_id]->pool_head;

  *busy->tail_block = out->head_block;
  busy->tail_block = out->tail_block;

  ChainNode* pc = busy->head_block;
  while (pc != NULL) {
    if (pc->block->last - pc->block->cur != 0) break;

    if (pc->block->tag != tag) {
      busy->head_block = pc->next;

      pc->next = cur->chain;
      cur->chain = pc;

      continue;
    }

    pc->block->cur = pc->block->start;
    pc->block->last = pc->block->start;

    busy->head_block = pc->next;
    pc->next = free->head_block;
    free->head_block = pc;
  }

  free->current = free->head_block;
}
