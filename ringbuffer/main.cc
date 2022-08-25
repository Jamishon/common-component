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
#include "ring_buffer.h"

using namespace ringbuffer;

int main() { 
   RingBuffer rb;
   int r_id = rb.CreateRing("first", 256, 1, sizeof(void *));
}