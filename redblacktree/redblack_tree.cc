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
