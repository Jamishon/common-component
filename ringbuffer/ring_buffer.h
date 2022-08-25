/**
 * @file ring_buffer.h
 * @author Jamishon
 * @brief Ring buffer without sync lock.
 * @version 0.1
 * @date 2022-08-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <mutex>
#include <string>
#include <map>

#define ALIGN_BYTE 4
#define ALIGNED __attribute__((__aligned__(ALIGN_BYTE)))

#define byte __uint8_t

namespace ringbuffer {


struct EntryNext {
  struct Entry* next;
  struct Entry** prev;
};

struct Entry {
  struct EntryNext next;
  void* data;
};

// struct RingList {
//   Entry* first;
//   Entry** last;
// };

enum SyncType {
  MT,  // multi-thread access
  ST,  // single-thread
  MT_RTS,
  MT_HTS
};

struct RingHeadTail {
  volatile uint32_t head;
  volatile uint32_t tail;
  SyncType type;
};

struct Ring {
  uint32_t ring_id;
  std::string name;
  int flags;
  uint32_t size;
  uint32_t mask;
  uint32_t capacity;

  void* addr;  // Allocated ring + array conent address

  RingHeadTail prod;

  RingHeadTail cons;
};

class RingBuffer {
 public:
  RingBuffer();
  ~RingBuffer();

  int CreateRing(std::string name, uint32_t size, int flag, uint8_t unit_byte = sizeof(void *));
  //int InitRing(Ring* r);
  int ResetRing(uint32_t ring_id);
  int FreeRing(uint32_t ring_id);
  //int AddRing(Ring *r);
  int RemoveRing(uint32_t ring_id);
  Ring* GetRing(uint32_t ring_id);

  int Enqueue(uint32_t ring_id, void* data, int num);
  int Dequeue(uint32_t ring_id, void* data, int num);

  int UpdateConsumerHead(Ring *r, uint32_t num, uint32_t* old_value, uint32_t* new_value);
  int UpdateProducerHead(Ring *r, uint32_t num, uint32_t* old_value, uint32_t* new_value);
  int DataEnqueue(Ring* r, void* src, uint32_t num, uint8_t unit_byte = sizeof(void *));
  int DataDequeue(Ring* r, void* dest, uint32_t num, uint8_t unit_byte = sizeof(void *));

  int UpdateConsumerTail(Ring *r, uint32_t* old_value, uint32_t* new_value);
  int UpdateProducerTail(Ring *r, uint32_t* old_value, uint32_t* new_value);

  void ReadMemoryBarrier();
  void WriteMemoryBarrier();
  bool AutomicCompareAndSwap(volatile uint32_t* old_value, uint32_t* expt_value, uint32_t* new_value);
  uint32_t AutomicFetchAdd(volatile uint32_t* value, uint32_t add);

 private:
  //RingList ring_list_;
  std::map<uint32_t, Ring *> ring_buffers_;
  std::mutex mtx_;
  static uint32_t assign_id_;
};

}  // namespace ringbuffer
