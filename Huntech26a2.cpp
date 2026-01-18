// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"
#include "RankedLeaderTree.h"
#include "IdTree.h"

Huntech::Huntech(): idTree(), leaderTree(), hunters() {}

Huntech::~Huntech() { idTree.deleteData(); } 

StatusType Huntech::add_squad(int squadId) {
    if (squadId <= 0) return StatusType::INVALID_INPUT;
    if (idTree.findSquad(squadId) != nullptr) return StatusType::FAILURE;
    Squad* newSquad = nullptr;
    try
    {
        newSquad = new Squad(squadId);
    }
    catch(const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    StatusType status = idTree.addSquad(newSquad);
    if (status != StatusType::SUCCESS)
    {
        delete newSquad;
        return res;
    }
    status = leaderTree.addSquad(newSquad);
    if (status != StatusType::SUCCESS)
    {
        idTree.removeSquad(squadId);
        delete newSquad;
        return res;
    }
    return StatusType::SUCCESS;
}

StatusType Huntech::remove_squad(int squadId) {
    if (squadId <= 0) return StatusType::INVALID_INPUT;
    Squad* s = idTree.findSquad(squadId);
    if (!s) return StatusType::FAILURE;
    int aura = s->getTotalAura();
    idTree.removeSquad(squadId);
    leaderTree.removeSquad(aura, squadId);
    delete s;
    return StatusType::SUCCESS;
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
    if (hunterId <= 0 || squadId <= 0 || !nenAbility.isValid() || aura < 0 || fightsHad < 0) return StatusType::INVALID_INPUT;
    if (hunters.find(hunterId) != nullptr) return StatusType::FAILURE;
    Squad* squad = IdTree.findSquad(squadId);
    if (!squad) return StatusType::FAILURE;

    Hunter* newHunter = nullptr;
    try
    {
        newHunter = new Hunter(hunterId, nenType, aura, fightsHad);
    }
    catch (const bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    newHunter->squadFightsAtStart = squad->getSquadExp();
    newHunter->nenOffset = squad->getNenAbility();
    Hunter* root = squad->getRoot();
    if (!root)
    {
        newHunter->parent = nullptr;
        newHunter->squad = squad;
        squad->setRoot(newHunter); 
    }
    else newHunter->parent = root;

    leaderTree.removeSquad(squad->getTotalAura(), squad->getSquadId());
    squad->addAura(aura);
    squad->updateNen(nenType, true);
    leaderTree.addSquad(squad);
    hunters.insert(hunterId, newHunter);
    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    if (squadId1 <=0 || squadId2 <= 0 || squadId1 == squadId2) return StatusType::INVALID_INPUT;
    Squad* squad1 = idTree.findSquad(squadId1);
    Squad* squad2 = idTree.findSquad(squadId2);
    if (squad1 == nullptr || squad2 == nullptr || squad1->getHunterCount() <= 0 || squad2->getHunterCount() <= 0) return StatusType::FAILURE;
    int fightStat1 = squad1->getSquadExp() + squad1->getTotalAura();
    int fightStat2 = squad2->getSquadExp() + squad2->getTotalAura();
    squad1->addFight();
    squad2->addFight();
    leaderTree.removeSquad(squad1->getTotalAura(), squadId1);
    leaderTree.removeSquad(squad2->getTotalAura(), squadId2);
    result = 0;

    if (fightStat1 == fightStat2)
    {
        if (squad1->getNenAbility() > squad2->getNenAbility())
        {
            squad1->addExp(3);
            result = 2;
        }
        else if (squad1->getNenAbility() < squad2->getNenAbility())
        {
            squad2->addExp(3);
            result = 4;
        }
        else
        {
            squad1->addExp(1);
            squad2->addExp(1);
            result = 0;
        }
    }
    else if (fightStat1 > fightStat2)
    {
        squad1->addExp(3);
        result = 1;
    }
    else
    {
        squad2->addExp(3);
        result = 3;
    }

    leaderTree.addSquad(squad1);
    leaderTree.addSquad(squad2);

    return output_t<result>;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    if (hunterId <= 0) return StatusType::INVALID_INPUT;
    auto hunter = hunters.find(hunterId); 
    if (!hunter) return StatusType::FAILURE;
    int totalFights = 0;
    Hunter* root = findRoot(hunter, totalFights);
    Squad* s = root->squad;
    return (s->getSquadExp() - totalFights + hunter->fightsAtStart); 
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    return 0;
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    return 0;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    return NenAbility();
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    return StatusType::FAILURE;
}

Hunter* Huntech::findRoot(Hunter* hunter, int& totalFights)
{
    if (hunter->parent == nullptr || hunter->parent == hunter)
    {
        totalFights = 0;
        return hunter;
    }
    int parentTotalFights = 0;
    Hunter* root = findRoot(hunter->parent, parentTotalFights);
    if (hunter->parent != root)
    {
        h->squadFightsAtStart += parentTotalFights;
        h->parent = root;
    }
    totalFights = h->fightsAtStart + parentTotalFights;
    return root;
}