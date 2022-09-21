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

BinaryTree::BinaryTree() : root(NULL) {

}

BinaryTree::~BinaryTree() {

}

TreeNode* BinaryTree::Search(int key) {
    if( !root ) return NULL;
    TreeNode* cur = root;
    while(cur != NULL) {
        if(cur->key == key) return cur;
        else if (cur->key < key )
            cur = cur->left;
        else 
            cur = cur->right;    
    }

    return NULL;
}

TreeNode* BinaryTree::Maximum(TreeNode* root) {
    
}

int BinaryTree::Insert(TreeNode* node) {
    if( !node ) return -1;

}