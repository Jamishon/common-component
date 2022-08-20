/**
 * @file main.cc
 * @author Jamishon
 * @brief Memory pool used to manage the allocation and release memory.
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>

#include "memory_pool.h"

int main() {
  MemoryPool mem_pool;
  size_t p_id = mem_pool.InitPool(256);

  // test case 1:
  int t = 10;
  while (t--) {
    int* n = (int*)mem_pool.AllocateMemory(p_id, sizeof(int));
    char* c = (char*)mem_pool.AllocateMemory(p_id, sizeof(char));
    long* l = (long*)mem_pool.AllocateMemory(p_id, sizeof(long));
    double* d = (double*)mem_pool.AllocateMemory(p_id, sizeof(double));
    long long* ll =
        (long long*)mem_pool.AllocateMemory(p_id, sizeof(long long));
    char* c1 = (char*)mem_pool.AllocateMemory(p_id, sizeof(char));
#ifdef DEBUG_
    printf("n:%16p\nc:%16p\nl:%16p\nd:%16p\nll:%16p\nc1:%16p\n\n", n, c, l, d,
           ll, c1);
#endif
  }

  // test case 2:
  t = 3;
  while (t) {
    size_t b_id = mem_pool.InitBlockChain(p_id, 254, t);
    MemoryPool::BlockList* bl = mem_pool.GetBlockList(b_id);
    MemoryPool::ChainNode* n1 = bl->head_block;
    MemoryPool::Block* b1 = bl->head_block->block;
    byte* s1 = bl->head_block->block->start;
    byte* e1 = bl->head_block->block->end;

    printf("\nt:%d\nbl:%p\nn1:%p\nb1:%p\ns1:%p\ne1:%p\n", t, bl, n1, b1, s1,
           e1);
    t--;
  }

  // test case 3:
  mem_pool.ResetPool(p_id);
  t = 10;
  while (t--) {
    int* n = (int*)mem_pool.AllocateMemory(p_id, sizeof(int));
    char* c = (char*)mem_pool.AllocateMemory(p_id, sizeof(char));
    long* l = (long*)mem_pool.AllocateMemory(p_id, sizeof(long));
    double* d = (double*)mem_pool.AllocateMemory(p_id, sizeof(double));
    long long* ll =
        (long long*)mem_pool.AllocateMemory(p_id, sizeof(long long));
    char* c1 = (char*)mem_pool.AllocateMemory(p_id, sizeof(char));
#ifdef DEBUG_
    printf("n:%16p\nc:%16p\nl:%16p\nd:%16p\nll:%16p\nc1:%16p\n\n", n, c, l, d,
           ll, c1);
#endif
  }

  t = 3;
  while (t) {
    size_t b_id = mem_pool.InitBlockChain(p_id, 254, t);
    MemoryPool::BlockList* bl = mem_pool.GetBlockList(b_id);
    MemoryPool::ChainNode* n1 = bl->head_block;
    MemoryPool::Block* b1 = bl->head_block->block;
    byte* s1 = bl->head_block->block->start;
    byte* e1 = bl->head_block->block->end;

    printf("\nt:%d\nbl:%p\nn1:%p\nb1:%p\ns1:%p\ne1:%p\n", t, bl, n1, b1, s1,
           e1);
    t--;
  }

  size_t p_id_1 = mem_pool.InitPool(32);
}