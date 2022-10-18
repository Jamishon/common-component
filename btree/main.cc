/**
 * @file main.cc
 * @author B-Tree
 * @brief Test case of B-Tree
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

#include <algorithm>
#include <vector>

#include "b_tree.h"

int main() {
  printf("test case 1 ---------------------------------\n");

  int degree = 2;
  int max_num = 1000;
  BTree btree(degree);
  std::vector<int> vc_num;

  for (int i = 0; i < max_num; i++) {
    vc_num.push_back(i);
  }

  std::random_shuffle(vc_num.begin(), vc_num.end());

  for (int i : vc_num) {
    printf(" %d ", i);
  }
  printf("\n\n");

  for (int key : vc_num) {
    btree.Insert(btree.GetRootP(), key, nullptr);
  }

  btree.InorderTraverse(btree.GetRoot(), PrintNode);
  printf("\n\n");

  printf("test case 2 ---------------------------------\n");
  for (int i : vc_num) {
    auto r = btree.Search(btree.GetRoot(), i);
    BTreeNode* node = std::get<0>(r);
    int index = std::get<1>(r);

    if (node != nullptr && node->keys.size() > index && index >= 0)
      printf("Search key:%d, result: %d, node:%p, index:%d, key in node:%d\n",
             i, node != nullptr && node->keys[index] == i, node, index,
             node->keys[index]);
    else
      printf("Search key:%d, result:false, node:%p, index:%d", i, node, index);
  }

  printf("\n\n\ntest case 3 ---------------------------------\n");

  for(int i:vc_num) {
    auto result = btree.InorderPredecessor(btree.GetRoot(), i);
    BTreeNode* pre = std::get<0>(result);
    int index = std::get<1>(result);
    if(pre != nullptr) {
      
      printf("key:%d, pre:%d\n", i, pre->keys[index]);
    } else {
      printf("key:%d, predecessor not exist !\n", i);
    }
  }

  printf("\n\n\ntest case 4 ---------------------------------\n");

  for(int i:vc_num) {
    auto result = btree.InorderSuccessor(btree.GetRoot(), i);
    BTreeNode* pre = std::get<0>(result);
    int index = std::get<1>(result);
    if(pre != nullptr) {
      
      printf("key:%d, suc:%d\n", i, pre->keys[index]);
    } else {
      printf("key:%d, successor not exist !\n", i);
    }
  }

  printf("\n\n\ntest case 5 ---------------------------------\n");

  for (int i : vc_num) {
    printf(" %d ", i);
  }
  printf("\n\n");

  std::random_shuffle(vc_num.begin(), vc_num.end());

  for (int i : vc_num) {
    printf(" %d ", i);
  }
  printf("\n\n");


  int result = -1;
  for(int i:vc_num) {
    result = btree.Delete(btree.GetRootP(), i);
    printf("delete key:%d, result:%d\n", i, result);
    btree.InorderTraverse(btree.GetRoot(), PrintNode);
    printf("\n\n");
  }

}