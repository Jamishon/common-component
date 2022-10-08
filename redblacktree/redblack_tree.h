/**
 * @file redblack_tree.h
 * @author Jamishon
 * @brief   Red-balck tree.
 * @version 0.1
 * @date 2022-09-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef REDBLACK_TREE_H
#define REDBLACK_TREE_H

#include <stdio.h>

#include "binary_tree.h"

struct RBTreeNode {
  int key;
  void* data;
  int color = 0;  // red 0, black 1, nil -1
  RBTreeNode* left = nullptr;
  RBTreeNode* right = nullptr;
  RBTreeNode* parent = nullptr;
};

RBTreeNode* CreateRBTreeNode(int key, void* data);

RBTreeNode* CreateRBTreeNilNode(int key, void* data);

typedef void (*VisitNode)(RBTreeNode*);

void PrintRBNode(RBTreeNode* node);

class RedBlackTree {
 public:
  RedBlackTree();

  int LeftRotate(RBTreeNode** root, RBTreeNode* node);
  int RightRotate(RBTreeNode** root, RBTreeNode* node);

  int Insert(RBTreeNode** root, RBTreeNode* node);
  int InsertFixup(RBTreeNode** root, RBTreeNode* node);

  int Delete(RBTreeNode** root, RBTreeNode* node);
  int Transplant(RBTreeNode** root, RBTreeNode* replacer, RBTreeNode* replaced);
  int DeleteFixup(RBTreeNode** root, RBTreeNode* node);

  void InorderTraverse(RBTreeNode* root, VisitNode visit);
  RBTreeNode* Minimum(RBTreeNode* root);

  RBTreeNode* GetRoot() { return root_; }

  RBTreeNode** GetRootP() { return &root_; }

 private:
  RBTreeNode* root_;
};

#endif