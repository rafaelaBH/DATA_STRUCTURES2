#ifndef NODE_H
#define NODE_H
#include "Hunter.h"
#include <memory>

class Node {
    Hunter* hunter;
    std::unique_ptr<Node> next;

    public:
        // constructor
        Node(Hunter* h = nullptr, std::unique_ptr<Node> n = nullptr) : hunter(h), next(std::move(n)) {}

        // getter
        int getData() const;

        // setter
        void setData(int value);
};

#endif // NODE_H