/**
 * @file b_tree.h
 * @author Jamishon
 * @brief B-Tree
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <vector>
#include <tuple>

struct BTreeNode {
    int num;    // num of keys
    bool leaf;  // 1 leaf node, 0 not leaf node
    std::vector<int> keys; // keys in this node
    std::vector<void *> data; //satellite information data for the key
    std::vector<BTreeNode*> children;   // pointer of num+1 children node
};

class BTree {
public:
    BTree(int degree);
    ~BTree();
    std::tuple<BTreeNode*, int> Search(BTreeNode* root, int key);
    BTreeNode* Create(BTreeNode* root);

    BTreeNode* SplitChild(BTreeNode* node, int child);
    int Insert(BTreeNode** root, int key, void* data = nullptr);
    int InsertNotFull(BTree* root, int key, void* data = nullptr);

    int Delete(BTreeNode* root, int key);

private:
    unsigned int degree_;
    BTreeNode* root_;
    std::vector<BTreeNode*> nodes_;             
};