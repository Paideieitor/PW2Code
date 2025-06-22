#ifndef __POKE_PARTY_2_H
#define __POKE_PARTY_2_H

#include "defs.h"

#include "items.h"

#include "pml/poke_party.h"

extern "C" u32 PokeParty_GetParam(PartyPkm* pPkm, PkmField field, void* extra);
extern "C" u32 PokeParty_GetStatusCond(PartyPkm* partyPkm);
extern "C" u32 PokeParty_CheckPPNeedsReplenish(PartyPkm* pkm, u32 moveSlot);
extern "C" b32 PokeList_CanFriendshipBeIncreased(PartyPkm* partyPkm, ItemData* itemData);

#endif // __POKE_PARTY_2_H
