#include "IdTree.h"

std::unique_ptr<IdNode> IdTree::balance(std::unique_ptr<IdNode> node)
{
    int bf = node->getBalance();
    if (bf > 1)
    {
        if (node->left->getBalance() < 0)
        {
            node->left = rotateLeft(std::move(node->left));
        }
        return rotateRight(std::move(node));
    }
    if (bf < -1)
    {
        if (node->right->getBalance() > 0)
        {
            node->right = rotateRight(std::move(node->right));
        }
        return rotateLeft(std::move(node));
    }
    return node;
}

std::unique_ptr<IdNode> IdTree::rotateLeft(std::unique_ptr<IdNode> root)
{
    std::unique_ptr<IdNode> newRoot = std::move(root->right);
    root->right = std::move(newRoot->left);
    if (root->right) root->right->parent = root.get();
    newRoot->parent = root->parent;
    newRoot->left = std::move(root);
    newRoot->left->parent = newRoot.get();
    
    newRoot->left->updateHeight();
    newRoot->updateHeight();

    return newRoot;
}

std::unique_ptr<IdNode> IdTree::rotateRight(std::unique_ptr<IdNode> root)
{
    std::unique_ptr<IdNode> newRoot = std::move(root->left);
    root->left = std::move(newRoot->right);
    if (root->left) root->left->parent = root.get();
    newRoot->parent = root->parent;
    newRoot->right = std::move(root);
    newRoot->right->parent = newRoot.get();

    newRoot->right->updateHeight();
    newRoot->updateHeight();

    return newRoot;
}

std::unique_ptr<IdNode> IdTree::addRecursive(std::unique_ptr<IdNode> curr, Squad* s, IdNode* parent_pt)
{
    if (!curr)
    {
        auto newNode = std::make_unique<IdNode>(s->getSquadId(), s);
        newNode->parent = parent_pt;
        return newNode;
    }

    if (s->getSquadId() < curr->squadId)
    {
        curr->left = addRecursive(std::move(curr->left), s, curr.get());
    }
    else
    {
        curr->right = addRecursive(std::move(curr->right), s, curr.get());
    }
    curr->updateHeight();
    return balance(std::move(curr));
}

std::unique_ptr<IdNode> IdTree::removeRecursive(std::unique_ptr<IdNode> curr, int id)
{
    if (!curr) return nullptr;

    if (id < curr->squadId)
    {
        curr->left = removeRecursive(std::move(curr->left), id);
        if (curr->left) curr->left->parent = curr.get();
    }
    else if (id > curr->squadId)
    {
        curr->right = removeRecursive(std::move(curr->right), id);
        if (curr->right) curr->right->parent = curr.get();
    }
    else
    {
        if (!curr->left)
        {
            std::unique_ptr<IdNode> temp = std::move(curr->right);
            if (temp) temp->parent = curr->parent;
            return temp;
        }
        else if (!curr->right)
        {
            std::unique_ptr<IdNode> temp = std::move(curr->left);
            if (temp) temp->parent = curr->parent;
            return temp;
        }
        IdNode* next = curr->right.get();
        while (next->left)
        {
            next = next->left.get();
        }

        curr->squadData = next->squadData;
        curr->squadId = next->squadId;

        curr->right = removeRecursive(std::move(curr->right), next->squadId);
        if (curr->right) curr->right->parent = curr.get();
    }

    curr->updateHeight();
    return balance(std::move(curr));
}

Squad* IdTree::findSquad(int id)
{
    IdNode* curr = root.get();
    while (curr)
    {
        if (id == curr->squadId) return curr->squadData;
        curr = (id < curr->squadId) ? curr->left.get() : curr->right.get();
    }
    return nullptr;
}

StatusType IdTree::addSquad(Squad* s)
{
    try
    {
        root = addRecursive(std::move(root), s, nullptr);
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType IdTree::removeSquad(int id) {
    root = removeRecursive(std::move(root), id);
    return StatusType::SUCCESS;
}

void IdTree::deleteNodes(IdNode* curr)
{
    if (!curr) return;
    deleteNodes(curr->left.get());
    deleteNodes(curr->right.get());
    delete curr->squadData;
}

void IdTree::deleteData()
{
    deleteNodes(root.get());
}
