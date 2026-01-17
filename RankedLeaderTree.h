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
    void addSquad(Squad* s);
    void removeSquad(int aura, int id);
    int getCount() const { return squadCount; }
};
