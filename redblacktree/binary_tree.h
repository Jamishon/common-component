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

struct TreeNode {
  void* data;
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;
  int key; 
};

class BinaryTree {
 public:
  BinaryTree();
  ~BinaryTree();

  TreeNode* Search(int key);
  TreeNode* Minimum(TreeNode* root);
  TreeNode* Maximum(TreeNode* root);
  TreeNode* Predecessor(TreeNode* node);
  TreeNode* Successor(TreeNode* node);
  int Insert(TreeNode* node);
  int Delete(TreeNode* node);

  int PreorderTraverse(TreeNode* root);
  int PostorderTraverse(TreeNode* root);
  int InorderTraverse(TreeNode* root);

 private:
  TreeNode* root;
};