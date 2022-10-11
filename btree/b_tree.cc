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

  bool inserted = false;
  BTreeNode* cur = root;
  int max_node = degree_ * 2 - 1;
  int index = 0;

  if (cur->num == max_node) {
    BTreeNode* parent = new BTreeNode{0, false};
    BTreeNode* brother = new BTreeNode{0, false};

    *root = parent;
    parent->children[0] = cur;
    parent->keys[0] = cur->keys[degree_ - 1];
    parent->data[0] = cur->keys[degree_ - 1];
    parent->children[1] = brother;
    parent->leaf = false;
    parent->num = 1;

    index = 0;
    while (index < degree_ - 1) {
      brother->keys[index] = cur->keys[degree_ + index];
      brother->data[index] = cur->data[degree_ + index];
      brother->children[index] = cur->children[degree_ + index];
      index++;
    }
    brother->children[index] = cur->children[degree_ + index];
    brother->leaf = cur->leaf;
    brother->num = degree_ - 1;

    cur->keys.erase(cur->keys.begin() + degree_ - 1, cur->keys.end());
    cur->data.erase(cur->data.begin() + degree_ - 1, cur->data.end());
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
      cur->keys.insert(index, key);
      cur->keys.insert(index, data);
      cur->num++;

      break;
    } else {
      // cur = cur->children[index];
      BTreeNode* left = cur->children[index];
      if (left->num == max_node) {
        BTreeNode* right = new BTreeNode{0, false};
        right->leaf = left->leaf;

        int i = 0;
        while (i < degree_ - 1) {
          right->keys[i] = left->keys[i + degree_];
          right->data[i] = left->data[i + degree_];
          right->children[i] = left->children[i + degree_];
          i++;
        }
        right->children[i] = left->children[i + degree_];
        right->num = degree_ - 1;

        cur->keys.insert(index, right->keys[degree_ - 1]);
        cur->data.insert(index, right->data[degree_ - 1]);
        cur->children(index + 1, right);
        cur->num++;

        left->keys.erase(left->keys.begin() + degree_ - 1, left->keys.end());
        left->data.erase(left->data.begin() + degree_ - 1, left->data.end());
        left->children.erase(left->children.begin() + degree_,
                             left->children.end());
        left->num = degree_ - 1;

        if (key > cur->keys[index]) index++;
      } else {
        cur = left;
      }
    }
  }
}
