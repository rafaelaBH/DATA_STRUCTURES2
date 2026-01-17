#pragma once
#include "LeaderNode.h"



class RanekdLeaderTree
{
private:
    std::unique_ptr<LeaderNode> root;
    int squadCount;

    static std::unique_ptr<LeaderNode> rotateRight(std::unique_ptr<LeaderNode> root);
    static std::unique_ptr<LeaderNode> rotateLeft(std::unique_ptr<LeaderNode> root);
    static std::unique_ptr<LeaderNode> rebalance(std::unique_ptr<LeaderNode> root);
    std::unique_ptr<LeaderNode> addRecursive(std::unique_ptr<LeaderNode> recursive, Squad* s, LeaderNode* parent);

public:
    RanekdLeaderTree() : root(nullptr), squadCount(0) {}
    Squad* getIthSquad(int i);
    std::unique_ptr<LeaderNode> balance(std::unique_ptr<LeaderNode> node);
    std::unique_ptr<LeaderNode> addRecursive(std::unique_ptr<LeaderNode> curr, Squad* s, LeaderNode* parent_ptr);
    StatusType addSquad(Squad* s);
    StatusType removeSquad(int aura, int id);
    int getCount() const { return squadCount; }
};
