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
#include <stddef.h>

#include <vector>

typedef __u_char byte;

//typedef __u_int size_t;

#define LARGE_SIZE 4095

#define DEBUG_

using namespace std;

#define ALIGN_NUM(n, alignment) (((n) + (alignment)-1) & (~((aligment)-1)))
#define ALIGN_PTR(ptr, aligment)                            \
  (__u_char*)(((__u_long)(ptr) + (__u_long)(alignment)-1) & \
              (~((__u_long)(aligment)-1)))

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
    int failed_times;
  };

  struct LargeNode {
    LargeNode* next;
    void* buf;
  };

  struct Block {
    byte* start;
    byte* end;

    byte* cur;
    byte* last;

    void* tag;
  };

  struct ChainNode {
    ChainNode* next;
    Block* block;
  };

  struct Pool {
    PoolNode node;
    LargeNode* large;
    ChainNode* chain;
    unsigned long large_mark;
  };

  struct PoolList {
    Pool* pool_head;
    Pool* current;
    Pool** pool_tail;
  };

  struct BlockList {
    ChainNode* head_block;
    ChainNode** tail_block;
    ChainNode* current;
  };

  int InitPool(size_t size);
  void DestoryPool(size_t pool_id);
  void ResetPool(size_t pool_id);

  void* AllocateMemory(size_t pool_id, size_t size);
  void* AllocateMinMemory(size_t pool_id, size_t size);
  void* AllocateLargeMemory(size_t pool_id, size_t size);
  int FreeLargeMemory(size_t pool_id, void* large);

  int InitBlockChain(size_t pool_id, size_t init_size, size_t node_num = 1);
  int CopyBlockChain(size_t pool_id, size_t dest_chain_id,
                        size_t src_chain_id);
  ChainNode* GetChainNode(size_t pool_id);
  ChainNode* GetFreeChainNode(size_t pool_id, size_t free_chain_id);
  int UpdateChain(size_t pool_id, size_t free_chain_id, size_t busy_chain_id,
                   size_t out_chain_id, void* tag);

  PoolList* GetPoolList(size_t pool_id);
  BlockList* GetBlockList(size_t block_id);               

 private:
  std::vector<PoolList*> mem_pools_;
  std::vector<BlockList*> block_bufs_;
};