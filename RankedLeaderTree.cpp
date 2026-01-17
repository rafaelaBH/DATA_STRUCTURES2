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

std::unique_ptr<LeaderNode> RankedLeaderTree::balance(std::unique_ptr<LeaderNode> node)
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

std::unique_ptr<LeaderNode> RankedLeaderTree::addRecursive(std::unique_ptr<LeaderNode> curr, Squad* s, LeaderNode* parent_ptr)
{
    if (!curr)
    {
        squadCounter++;
        auto newNode = std::make_unique<LeaderNode>(s);
        newNode->parent = parent_pt;
        return newNode;
    }

    if (s->getTotalAura() < curr->data->getTotalAura() || (s->getTotalAura() == curr->data->getTotalAura() && 
    s->getSquadId() < curr->data->getSquadId()))
    {
        curr->left = addRecursive(std::move(curr->left), s, curr.get());
    }
    else
    {
        curr->right = addRecursive(std::move(curr->right), s, curr.get());
    }
    curr->update();
    return balance(std::move(curr));

}

std::unique_ptr<LeaderNode> RankedLeaderTree::removeRecursive(std::unique_ptr<LeaderNode> curr, int aura, int id, bool& found)
{
    if (!curr) return nullptr;
    if (aura == curr->data->getTotalAura() && id == curr->data->getSquadId()) found = true;

    if (aura < curr->data->getTotalAura() || (aura == curr->data->getTotalAura() &&
        id < curr->data->getSquadId()))
    {
        curr->left = removeRecursive(std::move(curr->left), aura, id);
        if (curr->left) curr->left->parent = curr.get();
    }
    else if (aura > curr->data->getTotalAura() || (aura == curr->data->getTotalAura() &&
        id > curr->data->getSquadId()))
    {
        curr->right = removeRecursive(std::move(curr->right), aura, id);
        if (curr->right) curr->right->parent = curr.get();
    }
    else
    {
        squadCount--;
        if (!curr->left)
        {
            std::unique_ptr<LeaderNode> temp = std::move(curr->right);
            if (temp) temp->parent = curr->parent;
            return temp;
        }
        else if (!curr->right)
        {
            std::unique_ptr<LeaderNode> temp = std::move(curr->left);
            if (temp) temp->parent = curr->parent;
            return temp;
        }
        LeaderNode* next = curr->right.get();
        while (next->left)
        {
            next = next->left.get();
        }

        Squad* tempSquad = curr->data;
        curr->data = next->data;
        next->data = tempSquad;

        curr->right = removeRecursive(std::move(curr->right), next->data->getTotalAura(), next->data->getSquadId());
        if (curr->right) curr->right->parent = curr.get();
        squadCount++;
    }

    curr->update();
    return balance(std::move(curr));

}

StatusType RanekdLeaderTree::addSquad(Squad* s)
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

StatusType RanekdLeaderTree::removeSquad(int aura, int id)
{
    bool found = false;
    root = removeRecursive(std::move(root), aura, id, &found);
    if (found) return StatusType::SUCCESS;
    return StatusType::FAILURE;
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