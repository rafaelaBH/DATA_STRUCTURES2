#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <memory>
#include "Node.h"

class HashTable {
  std::unique_ptr<Node>* table;
   int maxSize;
   int size;
   int primeI;

   int hashFunction(int id) const;
   void rehash();

public:
    HashTable();
    ~HashTable();

    void insert(int key, std::shared_ptr<Hunter> hunter);
    std::shared_ptr<Hunter> find(int id) const;
    bool remove(int id);
};

#endif