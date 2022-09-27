/**
 * @file main.cc
 * @author Jamishon
 * @brief Red-black tree
 * @version 0.1
 * @date 2022-09-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>

#include <vector>

#include "binary_tree.h"

#include <random>

int main() {
  BinaryTree bt;
  int a[10] = {5, 2, 6, 7, 4, 1, 3, 9, 8, 0};
  std::vector<TreeNode*> vc_bt;
  for (int key : a) {
    TreeNode* node = CreateTreeNode(key, NULL);
    vc_bt.push_back(node);
    bt.Insert(node);
  }

  printf("\ntest case 1:--------------------------\n");

  bt.PostorderTraverse(bt.GetRoot(), PrintKey);
  printf("\n");
  bt.InorderTraverse(bt.GetRoot(), PrintKey);
  printf("\n");
  bt.PreorderTraverse(bt.GetRoot(), PrintKey);
  printf("\n");

  printf("\ntest case 2:--------------------------\n");

  // BinaryTree bt;
  // int a1[] = {13, 14, 16, 21, 54, 10, 11, 99, 54, 81, 88};
  for (int key : a) {
    TreeNode* p1 = bt.Search(key);
    if (p1)
      printf("Search key : %d node:%p\n", key, p1);
    else
      printf("Error ! Not exist int the tree\n");
  }

  printf("\ntest case 3:--------------------------\n");

  TreeNode* p2 = bt.Maximum(bt.GetRoot());
  if (p2)
    printf("Maximum, key:%d, node:%p\n", p2->key, p2);
  else
    printf("Error !\n");

  TreeNode* p3 = bt.Minimum(bt.GetRoot());
  if (p3)
    printf("Mininum, key:%d, node:%p\n", p3->key, p3);
  else
    printf("Error !\n");

  printf("\ntest case 4:--------------------------\n");

  for (TreeNode* node : vc_bt) {
    TreeNode* p4 = bt.InorderSuccessor(node);
    TreeNode* p5 = bt.InorderPredecessor(node);
    printf("key:%d\n", node->key);
    if (p5)
      printf("predecessor:%d\n", p5->key);
    else
      printf("predecssor is NULL\n");

    if (p4)
      printf("successor:%d\n", p4->key);
    else
      printf("successor is NULL\n");
    printf("\n");  
  }

  printf("\ntest case 5:--------------------------\n");
  int ret = -1;
  int t = 10;
  srand(0);
  while ( t--) {
    int r = rand() % 10;
    TreeNode* node = vc_bt.at(r);
    ret = bt.Delete(node);
    printf("node key:%d, node:%p, ret:%d\n", node->key, node, ret);
    bt.InorderTraverse(bt.GetRoot(), PrintKey);
    printf("\n");
  }
}