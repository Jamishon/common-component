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
#include <malloc.h>
#include <stdint.h>

struct Node {
  Node** next;
  void* data;
  uint32_t key;
};

class SkipList {
 public:
  SkipList(uint32_t depth = 10, float depth_ratio = 1 / 4)
      : depth_(depth), depth_ratio_(depth_ratio), num_(0), cur_depth_(0) {
    head_.next = &(Node*)calloc(depth_, sizeof(Node*));
    head_.data = NULL;
    head_.key = 0;
  }

  ~SkipList() {
    Node* cur = head_.next[0];

    Node* next = NULL;
    while (cur != NULL) {
      next = cur->next[0];
      free(cur);
      cur = next;
    }
  }

  int GetPrevNode(Node* cur, Node** prev) {
    if (cur == NULL) return -1;
    Node* p = (Node*)calloc(cur_depth_ + 1, sizeof(Node*));
    if (p == NULL) return -1;
    prev = &p;
    prev[cur_depth_] = head_.next[cur_depth_ - 1];
    for (int i = cur_depth_ - 1; i >= 0; i--) {
      prev[i] = prev[i + 1];
      while (prev[i]->next[i] != NULL && cur->key > prev[i]->next[i]->key) {
        prev[i] = prev[i]->next;
      }
    }

    return 0;
  }

  int GetNextNode(Node* cur, Node** next) {
    if (cur == NULL) return -1;
    Node* p = (Node*)calloc(cur_depth_ + 1, sizeof(Node*));
    if (p == NULL) return -1;
    next = &p;
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
    Node* prev;
    if (GetPrevNode(cur, prev) != 0) return -1;



    for (int i = cur_depth_ - 1; i >= 0; i--) {
      cur->next[i] = prev[i]->next[i];
      prev[i]->next = cur;
    }

    return 0;
  }

  int RemoveNode(Node* cur) {
    if (cur == NULL) return -1;
    Node** prev = NULL;
    if (GetPrevNode(cur, prev) != 0) return -1;

    for (int i = cur_depth_ - 1; i >= 0; i--) {
      prev[i]->next[i] = cur->next[i];
      cur->next[i] = NULL;

      if(head_.next[i] == NULL) cur_depth_--;
    }

    return 0;
  }

  uint32_t GetDepth() {
    return 0;
  }

 private:
  Node head_;           // Dummy head
  float depth_ratio_;   //  (depth 'num') / (depth + 1 'num')
  uint32_t depth_;      // Max depth of skip list
  uint32_t num_;        // Current node num except head
  uint32_t cur_depth_;  // Current depth
};