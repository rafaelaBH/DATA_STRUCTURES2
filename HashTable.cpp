#include "HashTable.h"

static const int primeNumbers[] = {17, 37, 79, 163, 331, 673, 1361, 2729, 5471};

HashTable::HashTable() : size(0), primeI(0)
{
    maxSize = primeNumbers[primeI];
    table = new std::unique_ptr<Node> [maxSize];
}

HashTable::~HashTable() {
    delete[] table;
}

int HashTable::hashFunction(int id) const {
    return id % maxSize;
}

void rehash() {
    primeI++;
    int formerMaxSize = maxSize;
    maxSize = primeNumbers[primeI];
    auto* newTable = new std::unique_ptr<Node> [maxSize];
    for (int i = 0; i < formerMaxSize; i++){
        while (table[i] != nullptr) {
            auto current = std::move(table[i]);
            table[i] = std::move(table[i]->next);
            int newI = current->id % maxSize;
            current->next = std::move(newTable[newI]);
            newTable[newI] = std::move(current);
        }
    }
    delete[] table;
    table = newTable;
}

void HashTable::insert(int id, std::shared_ptr<Hunter> hunter) {
    int i = hashFunction(id);
    table[i] = std::make_unique<Node>(id, hunter, std::move(table[i]));
    size++;
    if (size >= maxSize) rehash();
}

std::shared_ptr<Hunter> HashTable::find(int id) const {
    auto current = table[hashFunction(id)].get();
    while (current != nullptr) {
        if (current->id == id) return current->hunter;
        current = current->next.get();
    }
    return nullptr;
}

bool remove(int id) {
    auto current = table[hashFunction(id)].get();
    if (current == nullptr) return false;
    if (current->id == id) {
        table[hashFunction(id)] = std::move(table[hashFunction(id)]->next);
        return true;
    }
    while (current->next != nullptr) {
        if (current->next->id == id) {
            current->next = std::move(current->next->next);
            return true;
        }
        current = current->next.get();
    }
    return false;
}