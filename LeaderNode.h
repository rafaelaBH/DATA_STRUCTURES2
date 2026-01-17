#pragma once
#include "Squad.h"



class LeaderNode{
public:
    Squad* data;
    LeaderNode* parent;
    std::unique_ptr<LeaderNode> left;
    std::unique_ptr<LeaderNode> right;

    int height;
    int subtreeSize;

    LeaderNode(Squad* s): data(s), parent(nullptr), left(nullptr), right(nullptr), height(0), subtreeSize(1) {};
    const int getBalance() const
    {
        int left_height = (left) ? left->height : -1;
        int right_height = (right) ? right->height : -1;
        return left_height - right_height;
    }
    void update()
    {
        int left_height = (left) ? left->height : -1;
        int right_height = (right) ? right->height : -1;
        this->height = 1 + (left_height > right_height ? left_height : right_height);
        
        int left_subtree = (left) ? left->subtreeSize : 0;
        int right_subtree = (right) ? right->subtreeSize : 0;
        this->subtreeSize = 1 + left_subtree + right_subtree;
    }
};



