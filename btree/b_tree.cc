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

#include <stack>
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

  int index = 0;
  BTreeNode* del = nullptr;
  BTreeNode* cur = *root;
  BTreeNode* pre = nullptr;
  BTreeNode* next = nullptr;

  while (cur != nullptr) {
    index = 0;
    while (index < cur->num && key > cur->keys[index]) index++;

    if (index < cur->num && key == cur->keys[index]) {
      if (cur->leaf) {
        cur->keys.erase(cur->keys.begin() + index);  // case 1
        cur->data.erase(cur->data.begin() + index);
        cur->num--;

        return 0;
      } else {
        std::tuple<BTreeNode*, int> replacer;
        int next_children = 0;
        if (cur->children[index]->num > degree_ - 1) {
          replacer = InorderPredecessor(cur, key);  // case 2a
          next_children = index;
        } else if (cur->children[index + 1]->num > degree_ - 1) {
          replacer = InorderSuccessor(cur, key);  // case 2b
          next_children = index + 1;
        } else {
          pre = cur->children[index];  // case 2c
          pre->data.emplace_back(cur->data[index]);
          pre->keys.emplace_back(key);

          cur->keys.erase(cur->keys.begin() + index);
          cur->keys.erase(cur->data.begin() + index);
          cur->num--;

          next = cur->children[index + 1];
          for (int i = 0; i < next->num; i++) {
            pre->data.emplace_back(next->data[i]);
            pre->keys.emplace_back(next->keys[i]);
            pre->children.emplace_back(next->children[i]);
          }
          pre->children.emplace_back(next->children[next->num]);
          pre->num += next->num;

          delete next;
          next = nullptr;
          cur = pre;

          continue;
        }

        BTreeNode* replacer_node = std::get<0>(replacer);
        int idx = std::get<1>(replacer);
        key = replacer_node->keys[idx];  // delete replacer key
        cur->keys[index] = key;
        cur->data[index] = replacer_node->data[idx];
        cur = cur->children[next_children];
      }
    } else if (cur->leaf) {
      break;
    } else {
      BTreeNode* cur_child = cur->children[index];
      if (cur_child->num <= degree_ - 1) {
        if (index - 1 >= 0 && cur->children[index - 1]->num > degree_ - 1) {
          cur_child->keys.insert(cur_child->keys.begin(), cur->keys[index - 1]);
          cur_child->data.insert(cur_child->data.begin(), cur->data[index - 1]);
          cur_child->num++;

          pre = cur->children[index - 1];
          cur->keys[index] = pre->keys.back();
          cur->data[index] = pre->data.back();
          pre->keys.pop_back();
          pre->data.pop_back();
          pre->num--;

        } else if (index + 1 <= cur->num &&
                   cur->children[index + 1] > degree_ - 1) {
          cur_child->keys.insert(cur_child->keys.end(), cur->keys[index]);
          cur_child->data.insert(cur_child->data.end(), cur->data[index]);
          cur_child->num++;

          next = cur->children[index + 1];
          cur->keys[index] = next->keys.front();
          cur->data[index] = next->data.front();
          next->keys.erase(next->keys.begin());
          next->data.erase(next->data.begin());
          next->num--;

        } else {
          if (index == 0) {
            next = cur->children[index + 1];
            next->keys.insert(next->keys.begin(), cur->keys[index]);
            next->data.insert(next->data.begin(), cur->data[index]);
            next->num++;
            cur->keys.erase(cur->keys.begin() + index);
            cur->data.erase(cur->data.begin() + index);
            cur->num--;

            for (int i = 0; i < cur_child->num, i++) {
              next->keys.insert(next->keys.begin(), cur_child->keys[i]);
              next->data.insert(next->data.begin(), cur_child->data[i]);
              next->children.insert(next->children.begin(),
                                    cur_child->children[i]);
            }
            next->children.insert(next->children.begin(),
                                  cur_child->children[cur_child->num]);
            next->num += cur_child->num;

            delete cur_child;
            cur = next;
            next = nullptr;

          } else {
            pre = cur->children[index - 1];
            pre->keys.emplace_back(cur->keys[index]);
            pre->data.emplace_back(cur->data[index]);
            pre->num++;
            cur->keys.erase(cur->keys.begin() + index);
            cur->data.erase(cur->data.begin() + index);
            cur->num--;

            for (int i = 0; i < cur_child->num; i++) {
              pre->keys.push_back(cur_child->keys[i]);
              pre->data.push_back(cur_child->data[i]);
              pre->children.push_back(cur_child->children[i]);
            }
            pre->children.push_back(cur_child->num);
            pre->num += cur_child->num;

            delete cur_child;
            cur = pre;
            pre = nullptr;
          }
        }
      }

      cur = cur->children[index];
    }
  }

  return -1;
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
  if (root == nullptr) return std::make_tuple(nullptr, -1);

  BTreeNode* cur = root;
  int index = 0;
  std::stack<std::tuple<BTreeNode*, int>> stack;

  do {
    index = 0;
    while (index < cur->num && key > cur->keys[index]) index++;

    stack.emplace(std::make_tuple(cur, index));

    if (index < cur->num && key == cur->keys[index]) {
      if (cur->leaf) {
        if (index != 0) {
          return std::make_tuple(cur, index - 1);
        } else {
          stack.pop();
          while (!stack.empty()) {
            auto node_and_index = stack.top();
            stack.pop();
            index = std::get<1>(node_and_index);
            if (index != 0) {
              cur = std::get<0>(node_and_index);
              return std::make_tuple(cur, index - 1);
            }
          }
          return std::make_tuple(nullptr, 0);
        }
      } else {
        return Maximum(cur->children[index]);
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

  int index = 0;
  BTreeNode* cur = root;
  std::stack<std::tuple<BTreeNode*, int>> stack;

  do {
    index = 0;
    while (index < cur->num && key > cur->keys[index]) index++;

    stack.emplace(std::make_tuple(cur, index));

    if (index < cur->num && key == cur->keys[index]) {
      if (cur->leaf) {
        if (index < cur->num - 1) {
          return std::make_tuple(cur, index + 1);
        } else {
          stack.pop();
          while (!stack.empty()) {
            auto node_and_index = stack.top();
            stack.pop();
            index = std::get<1>(node_and_index);
            cur = std::get<0>(node_and_index);
            if (index < cur->num) {
              return std::make_tuple(cur, index);
            }
          }
          return std::make_tuple(nullptr, 0);
        }
      } else {
        return Mininum(cur->children[index + 1]);
      }
    } else if (cur->leaf) {
      break;
    } else {
      cur = cur->children[index];
    }
  } while (cur != nullptr);

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

std::tuple<BTreeNode*, int> BTree::Mininum(BTreeNode* root) {
  if (root == nullptr) return std::make_tuple(nullptr, -1);

  BTreeNode* cur = root;
  while (!cur->leaf) {
    cur = cur->children[0];
  }

  return std::make_tuple(cur, 0);
}
