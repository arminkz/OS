//
// Created by Armin on 11/6/17.
//

#ifndef TREENODE_H
#define TREENODE_H

#include <vector>

class TreeNode {
public:
    TreeNode(int);
    int data;
    std::vector<TreeNode*> children;
};

TreeNode::TreeNode(int data) {
    this->data = data;
}

#endif //TREENODE_H
