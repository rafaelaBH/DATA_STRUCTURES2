#ifndef NODE_H
#define NODE_H

#include <memory>
#include "Hunter.h"

class Node {
    int id;
    std::shared_ptr<Hunter> hunter;
    std::unique_ptr<Node> next;

public:

    Node(int id, std::shared_ptr<Hunter> h, std::unique_ptr<Node> n = nullptr) :
        id(id), hunter(std::move(h)), next(std::move(n)) {}
};

#endif // NODE_H