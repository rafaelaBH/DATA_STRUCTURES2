#pragma once
#include "wet2util.h"

class Squad
{
private:
    int squadId;
    int squadExp;
    int totalFights;
    int totalAura;
    NenAbility nenAbility;
    Hunter* root;
    int hunterCount;
    bool isRemoved;

public:
    Squad(int squadId): squadId(squadId), squadExp(0), totalAura(0), nenAbility(NenAbility::zero()), root(nullptr), hunterCount(0), isRemoved(false) {};
    const int getSquadId() const { return squadId; }
    const int getSquadExp() const{ return squadExp; }
    const int getTotalAura() const{ return totalAura; }
    NenAbility getNenAbility() const { return nenAbility; }
    Hunter* getRoot() const { return root; }
    void addExp(int exp){ this->squadExp+= exp; }
    void addAura(int aura){ this->totalAura += aura; }
    void updateNen(const NenAbility& nen, bool sign)
    {
        if (sign) this->nenAbility += ability;
        else this->nenAbility -= ability;
    }
    void setRoot(Hunter* root) { this->root = root; }
    int getHunterCount() const { return hunterCount; }
    void incrementHunterCounter() { this->hunterCounter++; }
    void addHunterCount(int count) { this->hunterCount += count; }
    void addFight() { this->totalFIghts++; }
    void markRemoved() { isRemoved = true; }
    bool isAlive() const { return !isRemoved; }
};






