/**
 * @file redblack_tree.cc
 * @author Jamishon
 * @brief Red-black tree.
 * @version 0.1
 * @date 2022-09-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "redblack_tree.h"

#include <stdio.h>

#include <stack>


void PrintRBNode(RBTreeNode* node) {
  if (node) printf(" %d(%d) ", node->key, node->color);
}

RBTreeNode* CreateRBTreeNode(int key, void* data) {
  RBTreeNode* p = new RBTreeNode;
  p->key = key;
  p->data = data;
  p->color = 0;
  p->left = nullptr;
  p->right = nullptr;
  p->parent = nullptr;

  return p;
}

RBTreeNode* CreateRBTreeNilNode(int key, void* data) {
  RBTreeNode* p = new RBTreeNode;
  p->key = key;
  p->data = data;
  p->color = -1;
  p->left = nullptr;
  p->right = nullptr;
  p->parent = nullptr;

  return p;
}

RedBlackTree::RedBlackTree() : root_(nullptr) {}

int RedBlackTree::LeftRotate(RBTreeNode** root, RBTreeNode* node) {
  if (root == nullptr || *root == nullptr || node == nullptr ||
      node->right == nullptr)
    return -1;

  RBTreeNode* right_node = node->right;

  // step 1
  node->right = right_node->left;
  if (right_node->left) right_node->left->parent = node;

  // step 2
  right_node->parent = node->parent;
  if (node->parent == nullptr) {
    *root = right_node;
  } else if (node->parent->left == node) {
    node->parent->left = right_node;
  } else {
    node->parent->right = right_node;
  }

  // step 3
  node->parent = right_node;
  right_node->left = node;

  return 0;
}

int RedBlackTree::RightRotate(RBTreeNode** root, RBTreeNode* node) {
  if (root == nullptr || *root == nullptr || node == nullptr ||
      node->left == nullptr)
    return -1;

  RBTreeNode* left_node = node->left;

  // step 1
  node->left = left_node->right;
  if (left_node->right) left_node->right->parent = node;

  // step 2
  left_node->parent = node->parent;
  if (node->parent == nullptr) {
    *root = left_node;
  } else if (node->parent->left == node) {
    node->parent->left = left_node;
  } else {
    node->parent->right = left_node;
  }

  // step 3
  node->parent = left_node;
  left_node->right = node;

  return 0;
}

int RedBlackTree::Insert(RBTreeNode** root, RBTreeNode* node) {
  if (root == nullptr || node == nullptr) return -1;
  RBTreeNode* pre = *root;
  RBTreeNode* cur = nullptr;
  while (pre != nullptr) {
    cur = pre;
    if (node->key > cur->key)
      pre = cur->right;
    else
      pre = cur->left;
  }

  node->parent = cur;
  if (cur == nullptr) {
    *root = node;
  } else if (node->key > cur->key) {
    cur->right = node;
  } else {
    cur->left = node;
  }

  node->left = nullptr;
  node->right = nullptr;
  node->color = 0;

#ifdef DEBUG_

  printf("RedBlackTree::Insert, node: %p, key:%d\n ", node, node->key);

  InorderTraverse(root_, PrintRBNode);
  printf("\n");

#endif

  InsertFixup(root, node);

#ifdef DEBUG_

  InorderTraverse(root_, PrintRBNode);
  printf("\n");

#endif

  return 0;
}

int RedBlackTree::InsertFixup(RBTreeNode** root, RBTreeNode* node) {
  if (root == nullptr || *root == nullptr || node == nullptr) return -1;

  RBTreeNode* index = nullptr;
  RBTreeNode* cur = node;
  while (cur != nullptr && cur->parent != nullptr &&
         cur->parent->parent != nullptr && cur->parent->color == 0) {
    if (cur->parent == cur->parent->parent->left) {  // parent left child
      index = cur->parent->parent->right;
      if (index != nullptr && index->color == 0) {  // case 1
        cur->parent->color = 1;
        index->color = 1;
        cur->parent->parent->color = 0;
        cur = cur->parent->parent;
      } else {
        if (cur == cur->parent->right) {  // case 2
          cur = cur->parent;
          LeftRotate(root, cur);
        }
        cur->parent->color = 1;  // case 3
        cur->parent->parent->color = 0;
        RightRotate(root, cur->parent->parent);
      }

    } else {  // parent right child
      index = cur->parent->parent->left;
      if (index != nullptr && index->color == 0) {  // case 1
        cur->parent->color = 1;
        index->color = 1;
        cur->parent->parent->color = 0;
        cur = cur->parent->parent;
      } else {
        if (cur == cur->parent->left) {  // case 2
          cur = cur->parent;
          RightRotate(root, cur);
        }
        cur->parent->color = 1;  // case 3
        cur->parent->parent->color = 0;
        LeftRotate(root, cur->parent->parent);
      }
    }
  }

  (*root)->color = 1;

  return 0;
}

int RedBlackTree::Transplant(RBTreeNode** root, RBTreeNode* replacer,
                             RBTreeNode* replaced) {
  if (root == nullptr || *root == nullptr || replaced == nullptr) return -1;

  if (replaced->parent == nullptr) {
    *root = replacer;
  } else if (replaced == replaced->parent->left) {
    replaced->parent->left = replacer;
  } else {
    replaced->parent->right = replacer;
  }

  if (replacer) replacer->parent = replaced->parent;

  return 0;
}

int RedBlackTree::Delete(RBTreeNode** root, RBTreeNode* node) {
  if (root == nullptr || *root == nullptr || node == nullptr ||
      node->color == -1)
    return -1;

  RBTreeNode* index = nullptr;
  RBTreeNode* successor = node;
  int original_color = node->color;
  if (node->left == nullptr) {
    index = node->right;

    if (index == nullptr && original_color == 1) {
      index = CreateRBTreeNilNode(-1, nullptr);
    }

    Transplant(root, index, node);
  } else if (node->right == nullptr) {
    index = node->left;

    if (index == nullptr && original_color == 1) {
      index = CreateRBTreeNilNode(-1, nullptr);
    }

    Transplant(root, index, node);
  } else {
    successor = Minimum(node->right);
    original_color = successor->color;
    index = successor->right;

    if (index == nullptr && original_color == 1) {
      index = CreateRBTreeNilNode(-1, nullptr);
    }

    if (successor->parent == node) {
      if (index != nullptr) index->parent = successor;
    } else {
      Transplant(root, index, successor);
      successor->right = node->right;
      successor->right->parent = successor;
    }

    Transplant(root, successor, node);
    successor->left = node->left;
    successor->left->parent = successor;
    successor->color = node->color;
  }

  node->parent = nullptr;
  node->left = nullptr;
  node->right = nullptr;
  node->color = -1;

  if (original_color == 1) {
    DeleteFixup(root, index);
  }

  return 0;
}

int RedBlackTree::DeleteFixup(RBTreeNode** root, RBTreeNode* node) {
  if (root == nullptr || *root == nullptr || node == nullptr) return -1;

  RBTreeNode* index = node;
  RBTreeNode* brother = nullptr;
  while (index != *root && (index->color == 1 || index->color == -1)) {
    if (index == index->parent->left) {
      brother = index->parent->right;
      if (brother->color == 0) {  // case 1
        brother->color = 1;
        index->parent->color = 0;
        LeftRotate(root, index->parent);
        brother = index->parent->right;
      } else if ((brother->left == nullptr || brother->left->color == 1) &&
                 (brother->right == nullptr ||
                  brother->right->color == 1)) {  // case 2
        brother->color = 0;
        index = index->parent;
      } else {
        if (brother->right == nullptr ||
            brother->right->color == 1) {  // case 3
          brother->color = 0;
          if (brother->left != nullptr) brother->left->color = 1;
          RightRotate(root, brother);
          brother = index->parent->right;
        }

        brother->color = index->parent->color;  // case 4
        index->parent->color = 1;
        if (brother->right != nullptr) brother->right->color = 1;
        LeftRotate(root, index->parent);

        break;
      }
    } else {
      brother = index->parent->left;
      if (brother->color == 0) {  // case 1
        index->parent->color = 0;
        brother->color = 1;
        RightRotate(root, index->parent);
        brother = index->parent->left;
      } else if ((brother->left == nullptr || brother->left->color == 1) &&
                 (brother->right == nullptr ||
                  brother->right->color == 1)) {  // case 2
        brother->color = 0;
        index = index->parent;
      } else {
        if (brother->left == nullptr || brother->left->color == 1) {  // case 3
          if (brother->right != nullptr) brother->right->color = 1;
          brother->color = 0;
          LeftRotate(root, brother);
          brother = index->parent->left;
        }

        brother->color = index->parent->color;  // case 4
        index->parent->color = 1;
        if (brother->left != nullptr) brother->left->color = 1;
        RightRotate(root, index->parent);

        break;
      }
    }
  }

  if (node->color == -1) {
    if (node->parent != nullptr) {
      if (node->parent->left == node)
        node->parent->left = nullptr;
      else
        node->parent->right = nullptr;
    } else {
      *root = nullptr;
    }
    node->parent = nullptr;
    delete node;
  }

  index->color = 1;

  return 0;
}

void RedBlackTree::InorderTraverse(RBTreeNode* root, VisitNode visit) {
  if (!root) return;

  RBTreeNode* cur = root;
  std::stack<RBTreeNode*> stack;
  while (cur != NULL || !stack.empty()) {
    if (cur != NULL) {
      stack.push(cur);
      cur = cur->left;
    } else if (!stack.empty()) {
      RBTreeNode* top = stack.top();
      stack.pop();
      visit(top);
      cur = top->right;
    }
  }
}

RBTreeNode* RedBlackTree::Minimum(RBTreeNode* root) {
  if (!root) return NULL;
  RBTreeNode* cur = root;
  while (cur->left != NULL) {
    cur = cur->left;
  }

  return cur;
}
