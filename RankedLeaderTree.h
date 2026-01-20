#pragma once
#include "LeaderNode.h"



class RankedLeaderTree
{
private:
    std::unique_ptr<LeaderNode> root;
    int squadCount;

    static std::unique_ptr<LeaderNode> rotateRight(std::unique_ptr<LeaderNode> root);
    static std::unique_ptr<LeaderNode> rotateLeft(std::unique_ptr<LeaderNode> root);
    static std::unique_ptr<LeaderNode> rebalance(std::unique_ptr<LeaderNode> root);
    std::unique_ptr<LeaderNode> addRecursive(std::unique_ptr<LeaderNode> curr, Squad* s, LeaderNode* parent_ptr);
    std::unique_ptr<LeaderNode> removeRecursive(std::unique_ptr<LeaderNode> curr, int aura, int id, bool& found);

public:
    RankedLeaderTree() : root(nullptr), squadCount(0) {}
    Squad* getIthSquad(int i);
    std::unique_ptr<LeaderNode> balance(std::unique_ptr<LeaderNode> node);
    StatusType addSquad(Squad* s);
    StatusType removeSquad(int aura, int id);
    int getCount() const { return squadCount; }
};
