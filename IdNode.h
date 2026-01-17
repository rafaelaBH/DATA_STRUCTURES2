#pragma once
#include "Squad.h"
#include <memory>


class IdNode{
public:
    int squadId;
    Squad* squadData;
    std::unique_ptr<IdNode> left;
    std::unique_ptr<IdNode> right;
    IdNode *parent;
    int height;

    IdNode(int id, Squad* data): squadId(id), squadData(data), left(nullptr), right(nullptr), parent(nullptr), height(0) {}
    void updateHeight()
    {
        int left_height = left ? left->height : -1;
        int right_height = right ? right->height : -1;
        height = 1 + (left_height > right_height ? left_height : right_height);
    }
    int getBalance() const
    {
        int left_height = left ? left->height : -1;
        int right_height = right ? right->height : -1;
        return left_height - right_height;
    }
    StatusType addId(int id);
    StatusType removeId(int id);

};