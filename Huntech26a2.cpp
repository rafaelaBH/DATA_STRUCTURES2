// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"
#include "RankedLeaderTree.h"
#include "IdTree.h"
#include "exception"

Huntech::Huntech(): idTree(), leaderTree(), hunters(), squadDelete(nullptr) {}

Huntech::~Huntech() {
    Squad* current = squadDelete;
    while (current != nullptr)
    {
        Squad* next = current->nextSquadToDelete;
        delete current;
        current = next;
    }
} 

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
        return status;
    }
    status = leaderTree.addSquad(newSquad);
    if (status != StatusType::SUCCESS)
    {
        idTree.removeSquad(squadId);
        delete newSquad;
        return status;
    }
    newSquad->nextSquadToDelete = squadDelete;
    squadDelete = newSquad;
    return StatusType::SUCCESS;
}

StatusType Huntech::remove_squad(int squadId) {
    if (squadId <= 0) return StatusType::INVALID_INPUT;
    Squad* s = idTree.findSquad(squadId);
    if (!s || !s->isAlive()) return StatusType::FAILURE;
    int aura = s->getTotalAura();
    StatusType status = leaderTree.removeSquad(aura, squadId);
    if (status != StatusType::SUCCESS) return StatusType::FAILURE;
    idTree.removeSquad(squadId);
    s->markRemoved();
    return StatusType::SUCCESS;
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
    if (hunterId <= 0 || squadId <= 0 || !nenType.isValid() || aura < 0 || fightsHad < 0) return StatusType::INVALID_INPUT;
    if (hunters.find(hunterId) != nullptr) return StatusType::FAILURE;
    Squad* squad = idTree.findSquad(squadId);
    if (!squad || !squad->isAlive()) return StatusType::FAILURE;

    int oldAura = squad->getTotalAura();
    if (leaderTree.removeSquad(oldAura, squad->getSquadId()) != StatusType::SUCCESS) return StatusType::FAILURE;

    std::shared_ptr<Hunter> newHunter = nullptr;
    try
    {
        newHunter = std::make_shared<Hunter>(hunterId, nenType, aura, fightsHad);
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    std::shared_ptr<Hunter> root = squad->getRoot();
    if (!root)
    {
        newHunter->nenOffset = NenAbility::zero(); 
        newHunter->parent = nullptr;
        newHunter->squad = squad;
        squad->setRoot(newHunter);
        newHunter->squadFightsAtStart = 0;
    }
    else
    {
        newHunter->nenOffset = squad->getNenAbility();
        newHunter->parent = root;
        newHunter->squadFightsAtStart = squad->getFights() - root->squadFightsAtStart;
    }

    squad->addAura(aura);
    squad->updateNen(nenType, true);
    squad->incrementHunterCounter();
    leaderTree.addSquad(squad);
    hunters.insert(hunterId, newHunter);
    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    if (squadId1 <=0 || squadId2 <= 0 || squadId1 == squadId2) return StatusType::INVALID_INPUT;
    Squad* squad1 = idTree.findSquad(squadId1);
    Squad* squad2 = idTree.findSquad(squadId2);
    if (squad1 == nullptr || squad2 == nullptr || squad1->getHunterCount() <= 0 || squad2->getHunterCount() <= 0
        || !squad1->isAlive() || !squad2->isAlive()) return StatusType::FAILURE;
    int fightStat1 = squad1->getSquadExp() + squad1->getTotalAura() + squad1->getNenAbility().getEffectiveNenAbility();
    int fightStat2 = squad2->getSquadExp() + squad2->getTotalAura() + squad2->getNenAbility().getEffectiveNenAbility();
    if (leaderTree.removeSquad(squad1->getTotalAura(), squadId1) != StatusType::SUCCESS) return StatusType::FAILURE;
    if (leaderTree.removeSquad(squad2->getTotalAura(), squadId2) != StatusType::SUCCESS) return StatusType::FAILURE;
    squad1->addFight();
    squad2->addFight();
    int result = 0;

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

    StatusType status1 = leaderTree.addSquad(squad1);
    StatusType status2 = leaderTree.addSquad(squad2);
    if (status1 != StatusType::SUCCESS || status2 != StatusType::SUCCESS) return StatusType::FAILURE;
    return result;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    int result = 0;
    if (hunterId <= 0) return StatusType::INVALID_INPUT;
    auto hunter = hunters.find(hunterId); 
    if (!hunter) return StatusType::FAILURE;
    int totalFights = 0;
    std::shared_ptr<Hunter> root = findRoot(hunter, totalFights);
    Squad* s = root->squad;
    if (!s) return StatusType::FAILURE;
    result = s->getFights() - totalFights + hunter->fightsAtStart;
    return result; 
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    int exp = 0;
    if (squadId <= 0) return StatusType::INVALID_INPUT;
    Squad* squad = idTree.findSquad(squadId);
    if (squad == nullptr || !squad->isAlive()) return StatusType::FAILURE;
    exp = squad->getSquadExp();
    return exp;
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    int id = 0;
    if (i < 1) return StatusType::FAILURE;
    Squad* squad = leaderTree.getIthSquad(i);
    if (!squad || !squad->isAlive()) return StatusType::FAILURE;
    id = squad->getSquadId();
    return id;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    NenAbility result = NenAbility::zero();
    if (hunterId <= 0) return StatusType::INVALID_INPUT;
    std::shared_ptr<Hunter> hunter = hunters.find(hunterId);
    if (hunter == nullptr) return StatusType::FAILURE;

    NenAbility total = NenAbility::zero();
    std::shared_ptr<Hunter> root = findRoot(hunter, total);
    if (!root || !root->squad   ) return StatusType::FAILURE;
    Squad* squad = root->squad;

    if (squad == nullptr) return StatusType::FAILURE;
    result = squad->getNenAbility() - total;
    return result;
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcedSquadId <= 0 || forcingSquadId <= 0 || forcingSquadId == forcedSquadId) return StatusType::INVALID_INPUT;
    Squad* forcingSquad = idTree.findSquad(forcingSquadId);
    Squad* forcedSquad = idTree.findSquad(forcedSquadId);
    if (!forcingSquad || !forcedSquad || forcingSquad->getHunterCount() == 0 || forcedSquad->getHunterCount() == 0
        || !forcingSquad->isAlive() || !forcedSquad->isAlive()) return StatusType::FAILURE;
    if (forcedSquad->getHunterCount() > 0)
    {
        int forcingStats = forcingSquad->getSquadExp() + forcingSquad->getTotalAura() + forcingSquad->getNenAbility().getEffectiveNenAbility();
        int forcedStats = forcedSquad->getSquadExp() + forcedSquad->getTotalAura() + forcedSquad->getNenAbility().getEffectiveNenAbility();
        if (forcingStats <= forcedStats) return StatusType::FAILURE;
    }

    std::shared_ptr<Hunter> root1 = forcingSquad->getRoot();
    std::shared_ptr<Hunter> root2 = forcedSquad->getRoot();
    int dummyFights = 0;
    if (root1 && root2 && findRoot(root1, dummyFights) == findRoot(root2, dummyFights)) return StatusType::FAILURE;

    if (leaderTree.removeSquad(forcingSquad->getTotalAura(), forcingSquadId) != StatusType::SUCCESS) return StatusType::FAILURE;
    if (leaderTree.removeSquad(forcedSquad->getTotalAura(), forcedSquadId) != StatusType::SUCCESS) return StatusType::FAILURE;
    mergeUnion(forcingSquad, forcedSquad);

    forcingSquad->addExp(forcedSquad->getSquadExp());
    forcingSquad->addAura(forcedSquad->getTotalAura());
    forcingSquad->updateNen(forcedSquad->getNenAbility(), true);
    forcingSquad->addHunterCount(forcedSquad->getHunterCount());
    idTree.removeSquad(forcedSquadId);
    forcedSquad->markRemoved();
    leaderTree.addSquad(forcingSquad);
    return StatusType::SUCCESS;
}

std::shared_ptr<Hunter> Huntech::findRoot(std::shared_ptr<Hunter> hunter, int& totalFights)
{
    if (hunter->parent == nullptr)
    {
        totalFights = hunter->squadFightsAtStart;
        return hunter;
    }
    int parentTotalFights = 0;
    std::shared_ptr<Hunter> root = findRoot(hunter->parent, parentTotalFights);
    hunter->squadFightsAtStart += parentTotalFights;
    hunter->parent = root;
    totalFights = hunter->squadFightsAtStart;
    return root;
}

std::shared_ptr<Hunter> Huntech::findRoot(std::shared_ptr<Hunter> hunter, NenAbility& totalNenOffset)
{
    if (hunter->parent == nullptr || hunter->parent == hunter)
    {
        totalNenOffset = hunter->nenOffset;
        return hunter;
    }

    NenAbility parentTotalNen = NenAbility::zero();
    std::shared_ptr<Hunter> root = findRoot(hunter->parent, parentTotalNen);
    hunter->nenOffset = hunter->nenOffset + parentTotalNen;
    hunter->parent = root;
    totalNenOffset = hunter->nenOffset;
    return root;
}

void Huntech::mergeUnion(Squad* forcingSquad, Squad* forcedSquad)
{
    if (!forcingSquad || !forcedSquad) return;
    std::shared_ptr<Hunter> forcingRoot = forcingSquad->getRoot();
    std::shared_ptr<Hunter> forcedRoot = forcedSquad->getRoot();

    if (!forcedRoot) return;
    if (!forcingRoot)
    {
        forcingSquad->setRoot(forcedRoot);
        forcedRoot->squad = forcingSquad;
        return;
    }

    int forcingNum = forcingSquad->getHunterCount();
    int forcedNum = forcedSquad->getHunterCount();

    if (forcingNum >= forcedNum)
    {
        forcedRoot->parent = forcingRoot;
        forcedRoot->squadFightsAtStart = forcedSquad->getFights() - forcingSquad->getFights();
        forcedRoot->nenOffset = forcedSquad->getNenAbility() - forcingSquad->getNenAbility();
        forcingRoot->squad = forcingSquad;
    }
    else
    {
        forcingRoot->parent = forcedRoot;
        forcingRoot->squadFightsAtStart = forcingSquad->getFights() - forcedSquad->getFights();
        forcingRoot->nenOffset = forcingSquad->getNenAbility() - forcedSquad->getNenAbility();
        forcingSquad->setRoot(forcedRoot);
        forcedRoot->squad = forcingSquad;
    }
}