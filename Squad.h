#pragma once
#include "wet2util.h"
#include <memory>

class Hunter;

class Squad
{
private:
    int squadId;
    int squadExp;
    int totalFights;
    int totalAura;
    NenAbility nenAbility;
    std::shared_ptr<Hunter> root;
    int hunterCount;
    bool isRemoved;

public:
    Squad* nextSquadToDelete;
    Squad(int squadId): squadId(squadId), squadExp(0), totalFights(0), totalAura(0), nenAbility(NenAbility::zero()),
             root(nullptr), hunterCount(0), isRemoved(false), nextSquadToDelete(nullptr) {};
    const int getSquadId() const { return squadId; }
    const int getSquadExp() const{ return squadExp; }
    const int getTotalAura() const{ return totalAura; }
    NenAbility getNenAbility() const { return nenAbility; }
    std::shared_ptr<Hunter> getRoot() const { return root; }
    int getFights() const { return totalFights; }
    void addExp(int exp){ this->squadExp+= exp; }
    void addAura(int aura){ this->totalAura += aura; }
    void updateNen(const NenAbility& nen, bool sign)
    {
        if (sign) this->nenAbility += nen;
        else this->nenAbility -= nen;
    }
    void setRoot(std::shared_ptr<Hunter> root) { this->root = root; }
    int getHunterCount() const { return hunterCount; }
    void incrementHunterCounter() { this->hunterCount++; }
    void addHunterCount(int count) { this->hunterCount += count; }
    void addFight() { this->totalFights++; }
    void markRemoved() { isRemoved = true; }
    bool isAlive() const { return !isRemoved; }
};






