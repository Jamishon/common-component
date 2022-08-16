/**
 * @file memory_pool.h
 * @author Jamishon
 * @brief Request memory in the thread pool beforehand, then use it.
 * @version 0.1
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <bits/types.h>

typedef __u_char byte;

#define LARGE_SIZE 4095

#define ALIGN_NUM(n, alignment)  (((n) + (alignment) - 1) &( ~((aligment)-1)))
#define ALIGN_PTR(ptr, aligment)                    \
(__u_char *) (( (__u_long)(ptr) + (__u_long)(alignment) - 1) & ( ~( (__u_long)(aligment)-1)))


class MemoryPool {
 public:
   MemoryPool();
   ~MemoryPool();

  struct Pool;

  struct PoolNode {
    Pool* next;
    byte* start;
    byte* last;
    byte* end;
  };

  struct LargeNode {
    LargeNode* next;
    byte* data;
  };

  struct Block {
    byte* start;
    byte* last;
    byte* end;
  };

  struct ChainNode {
    ChainNode* next;
    Block* cur;
  };

  struct Pool {
    PoolNode node;
    Pool* current;
    LargeNode* large;
    ChainNode* chain;
    unsigned long min_large_division;
  };

  void InitPool();
  void DestoryPool();
  void ResetPool();

  byte* AllocateMinMemory(Pool* p, int size);
  byte* AllocateLargeMemory(Pool* p, int size);

private:
  Pool*  pool_head_;
  Pool** pool_tail_;
};