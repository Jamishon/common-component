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

#include "binary_tree.h"

BinaryTree::BinaryTree() : root(NULL) {}

BinaryTree::~BinaryTree() {}

TreeNode* BinaryTree::Search(int key) {
  if (!root) return NULL;
  TreeNode* cur = root;
  while (cur != NULL) {
    if (cur->key == key)
      return cur;
    else if (cur->key < key)
      cur = cur->left;
    else
      cur = cur->right;
  }

  return NULL;
}

TreeNode* BinaryTree::Minimum(TreeNode* root) {
  if (!root) return NULL;
  TreeNode* cur = root;
  while (cur->left != NULL) {
    cur = cur->left;
  }

  return cur;
}

TreeNode* BinaryTree::Maximum(TreeNode* root) {
  if (!root) return NULL;
  TreeNode* cur = root;
  while (cur->right != NULL) cur = cur->right;
  return cur;
}

TreeNode* BinaryTree::Predecessor(TreeNode* node) {
  if (!node) return NULL;
  TreeNode* pre = NULL;
  if (node->left) {
    pre = Maximum(node->left);
  } else {
    TreeNode* child = node;
    pre = child->parent;
    while (pre && child == pre->left) {
      child = pre;
      pre = pre->parent;
    }
  }
  return pre;
}

TreeNode* BinaryTree::Successor(TreeNode* node) {
  if (!node) return NULL;
  TreeNode* suc = NULL;
  if (node->right) {
    suc = Minimum(node->right);
  } else {
    TreeNode* child = node;
    while (suc && child == suc->right) {
      child = suc;
      suc = suc->parent;
    }
  }
  return suc;
}

int BinaryTree::Insert(TreeNode* node) {
  if (!node) return -1;
  TreeNode* cur = NULL;
  TreeNode* pre = root_;
  if (pre) {
    while (pre != NULL) {
      cur = pre;
      if (node->key < cur->key) {
        pre = pre->left;
      } else {
        pre = pre->right;
      }
    }

    if (node->key < cur->key) {
      cur->left = node;
    } else if (node->key > cur->key) {
      cur->right = node;
    } else {
      return -2;
    }

    node->parent = cur;

  } else {
    root_ = node;
  }

  return 0;
}

int BinaryTree::Delete(TreeNode* node) {
    return 0;
}