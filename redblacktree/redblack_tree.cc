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

RedBlackTree::RedBlackTree() : nil_(nullptr), root_(nullptr) {

}

int RedBlackTree::LeftRotate(RBTreeNode* root, RBTreeNode* node) {
    if(root == nullptr || node == nullptr || node->right = nullptr) return -1;

    RBTreeNode* right_node = node->right;

    //step 1
    node->right = right_node->left;
    if( right_node->left) 
        right_node->left->parent = node;

    //step 2
    right_node->parent = node->parent;
    if(node->parent == nullptr) {
       root_ = right_node;
    } else if(node->parent->left == node) {
            right_node = node->parent->left;
        } else {
            right_node = node->parent->right;
        }

    //step 3  
    node->parent = right_node;
    right_node->left = node;

    return 0;
}

int RedBlackTree::RightRotate(RBTreeNode* root, RBTreeNode* node) {
    if( root == nullptr || node== nullptr || node->left == nullptr) return -1;

    RBTreeNode* left_node = node->left;
    
    //step 1
    node->left = left_node->right;
    if(left_node->right)
        left_node->right->parent = node;

    //step 2
    left_node->parent = node->parent;
    if(node->parent == nullptr) {
        root = left_node;
    } else if( node->parent->left == node) {
        left_node->parent = node->parent->left;
    } else {
        left_node->parent = node->parent->right;
    }

    //step 3
    node->parent = left_node;
    left_node->right = node;

    return 0;
}



int RedBlackTree::Insert(RBTreeNode* root, RBTreeNode* node) {
  if (node == nullptr) return -1;
  RBTreeNode* pre = root;
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
    root = node;
  } else if (node->key > cur->key) {
    cur->right = node;
  } else {
    cur->left = node;
  }

  node->left = nullptr;
  node->right = nullptr;
  node->color = 0;

  InsertFixup(root, node);

  return 0;
}


int RedBlackTree::InsertFixup(RBTreeNode* root, RBTreeNode* node) {
    if( root == nullptr || node == nullptr) return -1;

    RBTreeNode* index = nullptr;
    while(node->parent->color == 0) {
        if( node->parent == node->parent->parent->left) {   // parent left child
            index = node->parent->parent->right;
            if(index->color == 0) {                  //case 1        
                node->parent->color = 1;
                index->color = 1;
                node->parent->parent = 0;
                node = node->parent->parent;
            } else {
                if(node == node->parent->right ) {  // case 2
                    node = node->parent;
                    LeftRotate(T, node);
                }
                node->parent->color = 1;            // case 3
                node->parent->parent->color = 0;
                RightRotate(root, node->parent->parent)
            } 

        } else {                                    // parent right child
            index = node->parent->parent->left;
            if(index->color == 0)  {             // case 1
                node->parent->color = 1;
                index->color = 1;
                node->parent->parent->color = 0;
                node = node->parent->parent
            } else {
                if ( node = node->parent->left) { // case 2
                    node = node->parent;
                    RightRotate(root, node);
                }
                node->parent->color = 1;
                node->parent->parent->color = 0;
                LeftRotate(root, node->parent->parent)
            }
        }
    }

    root->color = 1;

    return 0;
}

int RedBlackTree::Transplant(RBTreeNode* root, RBTreeNode* replacer, RBTreeNode* replaced) {
    if(root == nullptr || replaced == nullptr) reutrn -1;

    if(replaced->parent == nullptr) {
        root = replacer;
    } else if(replaced = replaced->parent->left) {
        replaced->parent->left = replacer;
    } else {
        replaced->parent->right = replacer;
    }

    if(replacer)
        replacer->parent = replaced->parent;


     return 0;
}

int RedBlackTree::Delete(RBTreeNode* root, RBTreeNode* node) {
    if( root == nullptr || node == nullptr ) return -1;

    RBTreeNode* index = nullptr;
    RBTreeNode* successor = node;
    int original_color = node->color;
    if(node->left == nullptr) {
        index = node->right;
        Transplant(root, index, node);
    } else if ( node->right == nullptr) {
        index = node->left;
        Transplant(root, index, node);
    } else {
        successor = Minimum(node->right);
        original_color = successor->color;
        index = successor->right;
        if( successor->parent == node ) {
            index->parent = successor;
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

    if( original_color == 1 ) {
        DeleteFixup(root, index);
    }

    return 0;
}

int RedBlackTree::DeleteFixup(RBTreeNode* root, RBTreeNode* node) {
    if(root == nullptr || node == nullptr) return -1;

    RBTreeNode* index = node;
    RBTreeNode* brother = nullptr;
    while(index != root && index->color == 1) {
        if(index == index->parent->left) {
            brother = index->parent->right;
            if(brother->color == 0) {   // case 1
                brother->color = 1;
                index->parent->color = 0;
                LeftRotate(root, index->parent);
                brother = index->parent->right;
            } else if ( brother->left->color == 1 && brother->right->color == 1) { // case 2
                brother->color = 0;
                index = index->parent;
            } else {
                if( brother->right->color == 1) {  // case 3
                    brother->color = 0;
                    brother->left->color = 1;
                    RightRotate(root, brother);
                    brother = index->parent->right;
                }

                brother->color = index->parent->color;  // case 4
                index->parent->color = 1;
                brother->right->color = 1;
                LeftRotate(root, index->parent);
                break;
            }
        } else {
            brother = index->parent->left;
            if(brother->color == 0) {      // case 1
                index->parent->color = 0;
                brother->color = 1;
                RightRotate(root, index->parent);
                brother = index->parent->left;
            } else if(brother->left->color == 1 && brother->right->color == 1) { // case 2
                brother->color = 0;
                index = index->parent;
            } else {
                if(brother->left->color == 1) {
                    brother->right->color = 1;
                    brother->color = 0;
                    LeftRotate(root, brother);
                    brother = index->parent->left;
                }

                brother->color = index->parent->color;
                index->parent->color = 1;
                brother->left->color = 1;
                RightRotate(root, index->parent);

                break;
            }
        }
    }
    index->color = 1;

    return 0;
}




