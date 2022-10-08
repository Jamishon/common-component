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

#include <random>
#include <vector>

#include <algorithm>

#include "binary_tree.h"
#include "redblack_tree.h"

#define DEBUG_

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
  while (t--) {
    int r = rand() % 10;
    TreeNode* node = vc_bt.at(r);
    ret = bt.Delete(node);
    printf("node key:%d, node:%p, ret:%d\n", node->key, node, ret);
    bt.InorderTraverse(bt.GetRoot(), PrintKey);
    printf("\n");
  }

  printf("\n Red-Balck tree: \n");
  printf("\ntest case 5:--------------------------\n");

  RedBlackTree rbt;
  std::vector<RBTreeNode*> vc_rbt;
  for (int key : a) {
    RBTreeNode* p = CreateRBTreeNode(key, nullptr);
    // RBTreeNode* root = rbt.GetRoot();
    printf("new rbt node : %p\n", p);
    ret = rbt.Insert(rbt.GetRootP(), p);
    vc_rbt.push_back(p);

    rbt.InorderTraverse(rbt.GetRoot(), PrintRBNode);
    printf("\n");

    printf("RBTreeNode: %p, key: %d, color:%d, insert result:%d\n", p, p->key,
           p->color, ret);
  }

  printf("\ntest case 6:--------------------------\n");

  //rbt.PostorderTraverse(rbt.GetRoot(), PrintRBNode);
  printf("\n");
  rbt.InorderTraverse(rbt.GetRoot(), PrintRBNode);
  printf("\n");
  //rbt.PreorderTraverse(rbt.GetRoot(), PrintRBNode);
  printf("\n");



  printf("\ntest case 7:--------------------------\n");
  ret = -1;
  t = 0;
  while (t < 10) {

    RBTreeNode* node = vc_rbt.at(t++);
    ret = rbt.Delete(rbt.GetRootP(), node);
    printf(" rb node key:%d, node:%p, delete ret:%d\n", node->key, node,
           ret);
    rbt.InorderTraverse(rbt.GetRoot(), PrintRBNode);
    printf("\n");
  }


  printf("\ntest case 8:--------------------------\n");
  int max_num = 100;
  std::vector<int> vc_num;
  for(int i=0; i<max_num; i++) {
    vc_num.push_back(i);
  }
  std::random_shuffle(vc_num.begin(), vc_num.end());
  
  for(RBTreeNode* node : vc_rbt) {
    delete node;
  }
  vc_rbt.clear();
  RBTreeNode** pr = rbt.GetRootP();
  *pr = nullptr;

  for(int key: vc_num) {
    RBTreeNode* node = CreateRBTreeNode(key, nullptr);
    rbt.Insert(rbt.GetRootP(), node);
    vc_rbt.push_back(node);
  }
  rbt.InorderTraverse(rbt.GetRoot(), PrintRBNode);
    printf("\n");


printf("\ntest case 9:--------------------------\n");

  ret = -1;
  t = 0;
  while (t++ < max_num) {
    int r = rand() % max_num;
    RBTreeNode* node = vc_rbt.at(r);
    printf("node key:%d will be delete \n", node->key);
    ret = rbt.Delete(rbt.GetRootP(), node);
    printf("r:%d, rb node key:%d, node:%p, delete ret:%d\n", r, node->key, node,
           ret);
    rbt.InorderTraverse(rbt.GetRoot(), PrintRBNode);
    printf("\n");
  }



}