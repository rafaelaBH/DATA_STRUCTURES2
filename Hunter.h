
#ifndef HUNTER_H
#define HUNTER_H
#include "wet2util.h"
#include "Squad.h"


class Hunter {
public:
  int id;
  NenAbility nenAbility;
  int aura;
  std::shared_ptr<Hunter> parent;
  Squad* squad;
  int fightsAtStart;
  int squadFightsAtStart;
  NenAbility nenOffset;

  Hunter(int id, const NenAbility& nen, int aura, int fights) : id(id), nenAbility(nen), aura(aura),
        parent(nullptr), squad(nullptr), fightsAtStart(fights), squadFightsAtStart(0), nenOffset(NenAbility::zero()) {}
};
#endif //HUNTER_H
