/**
 * @file binary_tree.cc
 * @author Jamishon
 * @brief Binary search tree.
 * @version 0.1
 * @date 2022-09-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

struct TreeNode {
  int key;
  void* data;
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;
};

typedef void (*Visit)(TreeNode*);

TreeNode* CreateTreeNode(int key, void* data);

void PrintKey(TreeNode* node);

class BinaryTree {
 public:
  BinaryTree();
  ~BinaryTree();

  TreeNode* Search(int key);
  TreeNode* Minimum(TreeNode* root);
  TreeNode* Maximum(TreeNode* root);
  TreeNode* InorderPredecessor(TreeNode* node);
  TreeNode* InorderSuccessor(TreeNode* node);
  int Insert(TreeNode* node);
  int Delete(TreeNode* node);
  int Transplant(TreeNode* root, TreeNode* replacer, TreeNode* replaced);

  void PreorderTraverse(TreeNode* root, Visit visit);
  void PostorderTraverse(TreeNode* root, Visit visit);
  void InorderTraverse(TreeNode* root, Visit visit);

  TreeNode* GetRoot() { return root_; }

 private:
  TreeNode* root_;
};

#endif