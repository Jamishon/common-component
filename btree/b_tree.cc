/**
 * @file b_tree.cc
 * @author Jamishon
 * @brief B-tree
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "b_tree.h"

#include <stdio.h>

#include <tuple>
#include <vector>

void PrintNode(BTreeNode* node, int index) {
  if (node && index < node->keys.size()) {
    printf(" %d ", node->keys[index]);
    if (node->leaf) printf("(leaf) ");
  } else {
    printf("error ! node:%p, index:%d\n", node, index);
  }
}

BTree::BTree(int degree) : degree_(degree) { root_ = new BTreeNode{0, true}; }

BTree::~BTree() {}

std::tuple<BTreeNode*, int> BTree::Search(BTreeNode* root, int key) {
  if (root == nullptr) return std::make_tuple(nullptr, -1);

  BTreeNode* cur = root;
  int index = 0;
  do {
    index = 0;
    while (index < cur->num && cur->keys[index] < key) index++;
    if (index < cur->num && cur->keys[index] == key) {
      return std::make_tuple(cur, index);
    } else if (cur->leaf == true) {
      break;
    } else {
      cur = cur->children[index];
    }
  } while (cur != nullptr);

  return std::make_tuple(nullptr, -1);
}

int BTree::Insert(BTreeNode** root, int key, void* data) {
  if (root == nullptr && *root == nullptr) return -1;

  BTreeNode* cur = *root;
  int max_node = degree_ * 2 - 1;
  int index = 0;

  if (cur->num == max_node) {
    BTreeNode* parent = new BTreeNode{0, false};
    BTreeNode* brother = new BTreeNode{0, false};

    *root = parent;
    parent->children.push_back(cur);
    parent->keys.push_back(cur->keys[degree_ - 1]);
    parent->data.push_back(cur->data[degree_ - 1]);
    parent->children.push_back(brother);
    parent->leaf = false;
    parent->num = 1;

    index = 0;
    brother->leaf = cur->leaf;
    while (index < degree_ - 1) {
      brother->keys.push_back(cur->keys[degree_ + index]);
      brother->data.push_back(cur->data[degree_ + index]);
      if (!cur->leaf)
        brother->children.push_back(cur->children[degree_ + index]);
      index++;
    }
    if (!cur->leaf) brother->children.push_back(cur->children[degree_ + index]);
    brother->num = degree_ - 1;

    cur->keys.erase(cur->keys.begin() + degree_ - 1, cur->keys.end());
    cur->data.erase(cur->data.begin() + degree_ - 1, cur->data.end());
    if (!cur->leaf)
      cur->children.erase(cur->children.begin() + degree_, cur->children.end());
    cur->num = degree_ - 1;

    cur = *root;
  }

  while (cur != nullptr) {
    index = 0;
    while (index < cur->num && cur->keys[index] < key) {
      index++;
    }

    if (cur->leaf) {
      cur->keys.insert(cur->keys.begin() + index, key);
      cur->data.insert(cur->data.begin() + index, data);
      cur->num++;

#ifdef DEBUG_
      printf("Insert key:%d, data:%p\n", key, data);
      InorderTraverse(*root, PrintNode);
      printf("\n\n");
#endif

      break;
    } else {
      // cur = cur->children[index];
      BTreeNode* left = cur->children[index];
      if (left->num == max_node) {
        BTreeNode* right = new BTreeNode{0, false};
        right->leaf = left->leaf;

        int i = 0;
        while (i < degree_ - 1) {
          right->keys.push_back(left->keys[i + degree_]);
          right->data.push_back(left->data[i + degree_]);
          if (!left->leaf)
            right->children.push_back(left->children[i + degree_]);
          i++;
        }
        if (!left->leaf) right->children.push_back(left->children[i + degree_]);
        right->num = degree_ - 1;

        cur->keys.insert(cur->keys.begin() + index, left->keys[degree_ - 1]);
        cur->data.insert(cur->data.begin() + index, left->data[degree_ - 1]);
        cur->children.insert(cur->children.begin() + index + 1, right);
        cur->num++;

        left->keys.erase(left->keys.begin() + degree_ - 1, left->keys.end());
        left->data.erase(left->data.begin() + degree_ - 1, left->data.end());
        if (!left->leaf)
          left->children.erase(left->children.begin() + degree_,
                               left->children.end());
        left->num = degree_ - 1;

        if (key > cur->keys[index]) index++;

        cur = cur->children[index];
      } else {
        cur = left;
      }
    }
  }

  return 0;
}

int BTree::Delete(BTreeNode** root, int key) {
  if (root == nullptr || *root == nullptr) return -1;

  

  return 0;
}

void BTree::InorderTraverse(BTreeNode* root, Visit visit) {
  if (root == nullptr || visit == nullptr) return;

  BTreeNode* cur = root;
  std::vector<std::tuple<BTreeNode*, int>> stack;
  int index = 0;

  while (cur != nullptr || !stack.empty()) {
    if (cur != nullptr) {
      if (cur->leaf) {
        for (int i = 0; i < cur->keys.size(); i++) {
          visit(cur, i);
        }
        cur = nullptr;
      } else {
        index = 0;
        for (auto elem = stack.rbegin(); elem != stack.rend(); elem++) {
          if (std::get<0>(*elem) == cur) {
            index = std::get<1>(*elem);
            break;
          }
        }

        if (index < cur->keys.size()) {
          std::tuple<BTreeNode*, int> node = std::make_tuple(cur, index);
          stack.push_back(node);
        }
        cur = cur->children[index];
      }
    } else {
      if (!stack.empty()) {
        std::tuple<BTreeNode*, int> node = stack[stack.size() - 1];
        stack.pop_back();

        cur = std::get<0>(node);
        index = std::get<1>(node);

        visit(cur, index++);

        if (index < cur->keys.size()) {
          std::tuple<BTreeNode*, int> node = std::make_tuple(cur, index);
          stack.push_back(node);
        }
        cur = cur->children[index];
      }
    }
  }
}

std::tuple<BTreeNode*, int> BTree::InorderPredecessor(BTreeNode* root,
                                                      int key) {
  if (root == null) return std::make_tuple(nullptr, -1);

  int index_on_root = 0;
  int index_to_leaf = 0;
  BTreeNode* cur = root;
  BTreeNode* pre = nullptr;
  int index = 0;

  do {
    index = 0;
    while (index < cur->num && key > cur->keys[index]) index++;
    if (cur == root)
      index_on_root = index;
    else if (!cur->leaf && cur->children[index]->leaf) {
      index_to_leaf = index;
      pre = cur;
    }

    if (index < cur->num && key == cur->keys[index]) {
      if (cur->leaf) {
        if (index == 0 && index_to_leaf == 0) {
          if (index_on_root == 0) {
            return std::make_tuple(nullptr, 0);
          } else {
            return std::make_tuple(root, index_on_root - 1);
          }
        } else if (index == 0) {
          return std::make_tuple(pre, index_to_leaf - 1);
        } else {
          return std::make_tuple(cur, index - 1);
        }
      } else {
        auto max = Maximum(cur->children[index]);
        return std::make_tuple(std::get<0>(max), std::get<1>(max));
      }

    } else if (cur->leaf) {
      break;
    } else {
      cur = cur->children[index];
    }

  } while (cur != nullptr);

  return std::make_tuple(nullptr, -1);
}

std::tuple<BTreeNode*, int> BTree::InorderSuccessor(BTreeNode* root, int key) {
  if (root == nullptr) return std::make_tuple(nullptr, -1);

  int index_on_root = 0;
  int index_to_leaf = 0;
  int index = 0;
  BTreeNode* cur = root;
  BTreeNode* next = nullptr;

  do {
    while (index < cur->num && key > cur->keys[index]) index++;
    if (cur == root)
      index_on_root = index;
    else if (!cur->leaf && cur->children[index]->leaf) {
      next = cur;
      index_to_leaf = index;
    }

    if (index < cur->num && key == cur->keys[index]) {
      if (cur->leaf) {
        if (index == cur->num - 1 && index_to_leaf == next->num) {
          if (index_on_root == root->num) {
            return std::make_tuple(nullptr, 0);
          } else {
            return std::make_tuple(root, index_on_root);
          }
        } else if (index == cur->num - 1) {
          return std::make_tuple(next, index_to_leaf);
        } else {
          return std::make_tuple(cur, index + 1);
        }
      } else {
        auto min = Mininum(cur, index + 1);
        return std::make_tuple(std::get<0>(min), std::get<1>::(min));
      }

    } else if (cur->leaf) {
      break;
    } else {
      cur = cur->children[index];
    }
  }

  return std::make_tuple(nullptr, -1);
}

std::tuple<BTreeNode*, int> BTree::Maximum(BTreeNode* root) {
  if (root == nullptr) return std::make_tuple(nullptr, -1);

  BTreeNode* cur = root;
  int index = 0;
  while (!cur->leaf) {
    cur = cur->children[cur->num];
  }

  index = cur->num - 1;

  return std::make_tuple(cur, index);
}

std::tuple<BTreeNode*, int> BTree::Maximum(BTreeNode* root) {
  if (root == nullptr) return std::make_tuple(nullptr, -1);

  BTreeNode* cur = root;
  while (!cur->leaf) {
    cur = cur->children[0];
  }

  return std::make_tuple(cur, 0);
}
