#ifndef __POKE_PARTY_2_H
#define __POKE_PARTY_2_H

#include "pml/poke_party.h"

#include "defs.h"

#include "items.h"
#include "moves.h"
#include "species.h"

extern "C" u32 PokeParty_GetParam(PartyPkm * pPkm, PkmField field, void* extra);
extern "C" void PokeParty_SetParam(PartyPkm * pPkm, PkmField field, u32 data);
extern "C" u32 PokeParty_GetStatusCond(PartyPkm* partyPkm);
extern "C" u32 PokeParty_CheckPPNeedsReplenish(PartyPkm* pkm, u32 moveSlot);
extern "C" b32 PokeList_CanFriendshipBeIncreased(PartyPkm* partyPkm, ItemData* itemData);
extern "C" void PokeParty_SetMove(PartyPkm* pPkm, MOVE_ID moveIdx, u8 moveSlot);
extern "C" b32 PokeParty_AddPkm(PokeParty* party, PartyPkm* pPkm);
extern "C" void PokeParty_RecalcStats(PartyPkm * pPoke);
extern "C" void PokeParty_CreatePkm(PartyPkm * pkm, SPECIES species, u8 level, u32 tidSet, int abilLock, u32 ivs, u32 pid, u32 abilMask);

extern "C" u32 PML_PkmGetParamCore(BoxPkm * boxPkm, PkmField field, void* extra);
extern "C" void PML_PkmSetParamCore(BoxPkm * pPkmBlock, PkmField field, u32 value);

extern "C" void* PML_PkmGetParamBlockCore(BoxPkm * pkm, u32 pid, int blockNum);

#endif // __POKE_PARTY_2_H
