#pragma once
#include "IdNode.h"
#include <memory>

class IdTree{
private:
    std::unique_ptr<IdNode> root;

    static std::unique_ptr<IdNode> rotateRight(std::unique_ptr<IdNode> root);
    static std::unique_ptr<IdNode> rotateLeft(std::unique_ptr<IdNode> root);
    static std::unique_ptr<IdNode> balance(std::unique_ptr<IdNode> root);
    std::unique_ptr<IdNode> addRecursive(std::unique_ptr<IdNode> curr, Squad* s, IdNode* parent);
    std::unique_ptr<IdNode> removeRecursive(std::unique_ptr<IdNode> curr, int id);

public:
    IdTree(): root(nullptr) {}
    StatusType addSquad(Squad* s);
    StatusType removeSquad(int id);
    Squad* findSquad(int id);
};