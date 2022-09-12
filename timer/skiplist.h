/**
 * @file skiplist.h
 * @author Jamishon
 * @brief Skip list for traverse, insert, delete data.
 * @version 0.1
 * @date 2022-09-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <malloc.h>
#include <stdint.h>
#include <time.h>

#include <cstdlib>

struct Node {
  Node** next;
  void* data;
  uint64_t key;
};

class SkipList {
 public:
  SkipList(uint32_t depth = 10) : max_depth_(depth), num_(0), cur_depth_(1) {
    Node** p = (Node**)calloc(max_depth_, sizeof(Node*));
    if (p != NULL) {
      head_.next = p;
    }
    head_.data = NULL;
    head_.key = 0;

    srand((unsigned)time(NULL));
  }

  ~SkipList() {
    Node* cur = head_.next[0];

    Node* next = NULL;
    while (cur != NULL) {
      next = cur->next[0];
      free(cur->next);
      free(cur->data);
      free(cur);
      cur = next;
    }

    free(head_.next);
  }

  int GetPrevNode(Node* cur, Node** prev) {
    if (cur == NULL || prev == NULL) return -1;
    prev[cur_depth_] = &head_;
    for (int i = cur_depth_ - 1; i >= 0; i--) {
      prev[i] = prev[i + 1];
      while (prev[i]->next[i] != NULL && cur->key > prev[i]->next[i]->key) {
        prev[i] = prev[i]->next[i];
      }
    }

    return 0;
  }

  int GetNextNode(Node* cur, Node** next) {
    if (cur == NULL || next == NULL) return -1;
    next[cur_depth_] = head_.next[cur_depth_ - 1];
    for (int i = cur_depth_ - 1; i >= 0; i--) {
      next[i] = next[i + 1];
      while (next[i] != NULL && cur->key > next[i]->key) {
        next[i] = next[i]->next[i];
      }
    }

    return 0;
  }

  int AddNode(Node* cur) {
    if (cur == NULL) return -1;
    Node** prev = (Node**)calloc(max_depth_ + 1, sizeof(Node*));
    if (prev == NULL) return -1;

    if (GetPrevNode(cur, prev) != 0) return -1;

    uint32_t d = GetRadomNewNodeDepth();
    if (d == cur_depth_) cur_depth_++;

    for (int i = d; i >= 0; i--) {
      cur->next[i] = prev[i]->next[i];
      prev[i]->next[i] = cur;
    }

    free(prev);

    num_++;

    return 0;
  }

  int RemoveNode(Node* cur) {
    if (cur == NULL) return -1;
    Node** prev = (Node**)calloc(max_depth_ + 1, sizeof(Node*));
    if (prev == NULL) return -1;

    if (GetPrevNode(cur, prev) != 0) return -1;
    bool removed = false;
    for (int i = cur_depth_ - 1; i >= 0; i--) {
      if (prev[i]->next[i] == cur) {
        prev[i]->next[i] = cur->next[i];
        cur->next[i] = NULL;
        removed = true;
      }

      if (head_.next[i] == NULL) cur_depth_--;
    }

    if (removed) num_--;

    free(prev);

    return 0;
  }

  uint32_t GetRadomNewNodeDepth() {
    uint32_t r = rand();
    // int32_t temp = r == 0 ? max_depth_ : (__buildin_ctz(r) - 1) / 2;
    uint32_t temp = r == 0 ? max_depth_ : (GetTrailingZeroNum(r)) / 2;
    if (temp > max_depth_)
      temp = cur_depth_;
    else if (temp == max_depth_)
      temp = max_depth_ - 1;

    return temp;
  }

  Node* InitNode(uint32_t key, void* data) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL) return NULL;
    p->data = data;
    p->key = key;
    Node** n = (Node**)calloc(max_depth_, sizeof(Node*));
    if (n == NULL) return NULL;
    p->next = n;

    return p;
  }

  static uint32_t GetTrailingZeroNum(uint32_t n) {
    int z = 0;
    for (int i = 0; i < 32; i++) {
      if (n & (0x01 << i) == 0)
        z++;
      else
        break;
    }

    return z;
  }

  uint32_t MaxDepth() {
    return max_depth_;
  }

  Node* GetHead() {
    return &head_;
  }

  uint32_t CurrentDepth() {
    return cur_depth_;
  }

  void AddCurrentDepth() {
    cur_depth_++;
  }

  void MinusCurrentDepth() {
    cur_depth_--;
  }

 private:
  Node head_;           // Dummy head
  uint32_t max_depth_;  // Max depth of skip list
  uint32_t num_;        // Current node num except head
  uint32_t cur_depth_;  // Current depth
};

#endif