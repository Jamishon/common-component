/**
 * @file redblack_tree.h
 * @author Jamishon
 * @brief   Red-balck tree.
 * @version 0.1
 * @date 2022-09-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

struct RBTreeNode {
    int key;
    RBTreeNode* left;
    RBTreeNode* right;
    RBTreeNode* parent;
    bool color;  // red 0, black 1
    void* data;
};

class RedBlackTree {
public:
    RedBlackTree();


    int LeftRotate(RBTreeNode* root, RBTreeNode* node);
    int RightRotate(RBTreeNode* root, RBTreeNode* node);

    int Insert(RBTreeNode* root, RBTreeNode* node);
    int InsertFixup(RedBlackTree* root, RBTreeNode* node);

    int Delete(RBTreeNode* root, RBTreeNode* node);
    int Transplant(RBTreeNode* root, RBTreeNode* replacer, RBTreeNode* replaced);
    int DeleteFixup(RBTreeNode* root, RBTreeNode* node);


    RBTreeNode* GetRoot() {
        return root_;
    }

    RBTreeNode* nil_;
private:
    RBTreeNode* root_;
};