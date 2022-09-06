/**
 * @file main.cc
 * @author Jamishon
 * @brief Timer for run task on time.
 * @version 0.1
 * @date 2022-09-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "skiplist.h"

int main() {
  SkipList sl;
  int* data = (int *)malloc(sizeof(int));
  *data = 0xff;
  Node* n1 = sl.InitNode(3, data);
  Node* n2 = sl.InitNode(7, data);
  Node* n3 = sl.InitNode(9, data);
  Node* n4 = sl.InitNode(10, data);

  sl.AddNode(n2);
  sl.AddNode(n1);
  sl.AddNode(n4);
  sl.AddNode(n3);

  sl.RemoveNode(n1);
  sl.RemoveNode(n4);
  sl.RemoveNode(n3);
}