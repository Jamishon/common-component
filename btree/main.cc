/**
 * @file main.cc
 * @author B-Tree
 * @brief Test case of B-Tree
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "b_tree.h"
#include <stdio.h>
#include <vector>
#include <algorithm>

int main() {
  
    printf("test case 1 ---------------------------------\n");
    
    int degree = 2;
    int max_num = 100;
    BTree btree(degree);
    std::vector<int> vc_num;

    for(int i=0; i<max_num; i++) {
        vc_num.push_back(i);
    }

    std::random_shuffle(vc_num.begin(), vc_num.end());

    for(int i : vc_num) {
        printf(" %d ", i);
    }
    printf("\n\n");

    for(int key : vc_num) {
        btree.Insert(btree.GetRootP(), key, nullptr);
    }

    btree.InorderTraverse(btree.GetRoot(), PrintNode);
    printf("\n\n");


    

}