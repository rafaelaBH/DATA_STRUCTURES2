std::unique_ptr<LeaderNode> RanekdLeaderTree::rotateRight(std::unique_ptr<LeaderNode> root)
{
    std::unique_ptr<LeaderNode> newRoot = std::move(root->left);
    root->left = std::move(newRoot->right);
    if (root->left) root->left->parent = root.get();
    newRoot->parent = root->parent;
    newRoot->right = std::move(root);
    newRoot->right->parent = newRoot.get();

    newRoot->right->update();
    newRoot->update();

    return newRoot;
}

std::unique_ptr<LeaderNode> RanekdLeaderTree::rotateLeft(std::unique_ptr<LeaderNode> root)
{
    std::unique_ptr<LeaderNode> newRoot = std::move(root->right);
    root->right = std::move(newRoot->left);
    if (root->right) root->right->parent = root.get();
    newRoot->parent = root->parent;
    newRoot->left = std::move(root);
    newRoot->left->parent = newRoot.get();
    
    newRoot->left->update();
    newRoot->update();

    return newRoot;
}

Squad* RanekdLeaderTree::getIthSquad(int i)
{
    if (i < 1 || i > squadCount || !root) return nullptr;

    LeaderNode* curr = root.get();
    while (curr)
    {
        int leftSize = curr->left ? curr->left->subtreeSize : 0;
        if (i == leftSize + 1) return curr->data;
        else if (i <= leftSize) curr = curr->left.get();
        else
        {
            i = i - (leftSize + 1);
            curr = curr->right.get();
        }
    }
    return nullptr;
}