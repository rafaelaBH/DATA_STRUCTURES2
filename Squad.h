#pragma once
#include "wet2util.h"

class Squad
{
private:
    int squadId;
    int squadExp;
    int totalAura;
    NenAbility nenAbility;


public:
    Squad(int squadId): squadId(squadID), squadExp(0), totalAura(0), nenAbility(NenAbility::zero()) {};
    const int getSquadId() const { return squadId; }
    const int getSquadExp() const{ return squadExp; }
    const int getTotalAura() const{ return totalAura; }
    NenAbility getNenAbility() const { return nenAbility; }
    void addExp(int exp){ this->squadExp+= exp; }
    void addAura(int aura){ this->totalAura += aura; }
    void updateNen(const NenAbility& nen, bool sign)
    {
        if (sign) this->nenAbility += ability;
        else this->nenAbility -= ability;
    }
};






