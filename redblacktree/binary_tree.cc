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

#include <stdio.h>

#include <stack>

void PrintKey(TreeNode* node) {
  if (node) printf(" %d ", node->key);
}

TreeNode* CreateTreeNode(int key, void* data) {
  struct TreeNode* p = new TreeNode{key, data, NULL, NULL, NULL};

  return p;
}

BinaryTree::BinaryTree() : root_(NULL) {}

BinaryTree::~BinaryTree() {}

TreeNode* BinaryTree::Search(int key) {
  if (!root_) return NULL;
  TreeNode* cur = root_;
  while (cur != NULL) {
    if (cur->key == key)
      return cur;
    else if (cur->key > key)
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

TreeNode* BinaryTree::InorderPredecessor(TreeNode* node) {
  if (!node) return NULL;
  TreeNode* pre = node->parent;
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

TreeNode* BinaryTree::InorderSuccessor(TreeNode* node) {
  if (!node) return NULL;
  TreeNode* suc = node->parent;
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
      return -1;
    }

    node->parent = cur;

  } else {
    root_ = node;
    node->parent = NULL;
  }

  return 0;
}

int BinaryTree::Transplant(TreeNode* root, TreeNode* replacer,
                           TreeNode* replaced) {
  if (root == NULL || replaced == NULL) return -1;
  if (replaced->parent == NULL) {
    root = replacer;

    if (replacer != NULL) replacer->parent = NULL;
  } else {
    if (replaced->parent->left == replaced) {
      replaced->parent->left = replacer;
    } else if (replaced->parent->right == replaced) {
      replaced->parent->right = replacer;
    }

    if (replacer != NULL) replacer->parent = replaced->parent;
  }

  return 0;
}

int BinaryTree::Delete(TreeNode* node) {
  if (!node) return -1;

  if (node->left == NULL) {
    Transplant(root_, node->right, node);
  } else if (node->right == NULL) {
    Transplant(root_, node->left, node);
  } else {
    TreeNode* suc = Minimum(node->right);
    if (suc->parent != node) {
      Transplant(root_, suc->right, suc);
      suc->right = node->right;
      node->right->parent = suc;
    }
    Transplant(root_, suc, node);
    suc->left = node->left;
    suc->left->parent = suc;
  }

  return 0;
}

void BinaryTree::PreorderTraverse(TreeNode* root, Visit visit) {
  if (!root) return;

  TreeNode* cur = root;
  std::stack<TreeNode*> stack;
  while (cur != NULL || !stack.empty()) {
    if (cur != NULL) {
      visit(cur);
      if (cur->right != NULL) stack.push(cur);
      cur = cur->left;
    } else if (!stack.empty()) {
      TreeNode* top = stack.top();
      stack.pop();
      cur = top->right;
    }
  }
}

void BinaryTree::InorderTraverse(TreeNode* root, Visit visit) {
  if (!root) return;

  TreeNode* cur = root;
  std::stack<TreeNode*> stack;
  while (cur != NULL || !stack.empty()) {
    if (cur != NULL) {
      stack.push(cur);
      cur = cur->left;
    } else if (!stack.empty()) {
      TreeNode* top = stack.top();
      stack.pop();
      visit(top);
      cur = top->right;
    }
  }
}

void BinaryTree::PostorderTraverse(TreeNode* root, Visit visit) {
  if (!root) return;

  TreeNode* cur = root;
  TreeNode* visited = NULL;
  std::stack<TreeNode*> stack;
  while (cur != NULL || !stack.empty()) {
    if (cur != NULL) {
      stack.push(cur);
      cur = cur->left;
    } else if (!stack.empty()) {
      TreeNode* top = stack.top();
      if (top->right != NULL) {
        if (top->right != visited) {
          cur = top->right;
        } else {
          visit(top);
          stack.pop();
          visited = top;
        }
      } else {
        visit(top);
        stack.pop();
        visited = top;
      }
    }
  }
}