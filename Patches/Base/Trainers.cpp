#include "settings.h"

#include "gfl/core/gfl_random.h"

#include "poke_party.h"
#include "trainer_data.h"

#ifdef PERFECT_TRAINERS

struct SWAN_PACKED TrainerPokemonExt
{
	u32 genetic; // 2 bits [sex] + 30 bits [IVs (HP, ATK, DEF, SPA, SPD, SPE)]
	u16 species;
	u8 form;
	u8 level;
	u16 heldItem;
	u16 moves[4]; // 1 bit [PP Max] + 15 bits [Move ID]
	u16 ability;
	u8 evs[6]; // (HP, ATK, DEF, SPE, SPA, SPD)
	u8 nature;
	u8 happiness;
	u8 status;
	u8 hp;
};
#define GET_TRPOKE_SEX(trainerPokemon) (trainerPokemon.genetic >> 30)
#define GET_TRPOKE_IV(trainerPokemon) (trainerPokemon.genetic & 0x3FFFFFFF)
#define GET_TRMOVE_PPMAX(trainerPokemon, slot) (trainerPokemon.moves[slot] & 0x8000 != 0) 
#define GET_TRMOVE_ID(trainerPokemon, slot) (trainerPokemon.moves[slot] & 0x7FFF) 

extern "C" void TrainerUtil_CalcBasePID(u16 species, u16 form, u8 abilAndSex, u32* dest, HeapID heapId);
extern "C" void THUMB_BRANCH_LINK_TrainerUtil_LoadParty_0x4C8(PokeParty* party, PartyPkm* partyPkm, 
	TrainerData* trainerData, TrainerPokemonExt* team) {

	if (trainerData->trainerType == PERFECT_TRAINER) {
		for (u32 slot = 0; slot < trainerData->pokemonCount; ++slot) {
			
			u32 PID = 0;
			TrainerUtil_CalcBasePID(team[slot].species, team[slot].form, GET_TRPOKE_SEX(team[slot]), &PID, 0x76/*HEAPID_BATTLE_LOAD*/);
			PID += (team[slot].species + team[slot].form + team[slot].level + GFL_RandomLC(0xFF)) << 8;

			PokeParty_CreatePkm(partyPkm, team[slot].species, team[slot].level, 0, -1, GET_TRPOKE_IV(team[slot]), PID, 0);
		
			PokeParty_SetParam(partyPkm, PF_Item, team[slot].heldItem);
			for (u32 moveSlot = 0; moveSlot < 4; ++moveSlot) {
				PokeParty_SetParam(partyPkm, (PkmField)(PF_Move1PPMax + moveSlot), GET_TRMOVE_PPMAX(team[slot], moveSlot));
				PokeParty_SetMove(partyPkm, GET_TRMOVE_ID(team[slot], moveSlot), moveSlot);
			}

			PokeParty_SetParam(partyPkm, PF_Forme, team[slot].form);
			PokeParty_SetParam(partyPkm, PF_Ability, team[slot].ability);
			PokeParty_SetParam(partyPkm, PF_Nature, team[slot].nature);
			PokeParty_SetParam(partyPkm, PF_Happiness, team[slot].happiness);

			for (u32 ev = 0; ev < 6; ++ev)
				PokeParty_SetParam(partyPkm, (PkmField)(PF_EvHP + ev), team[slot].evs[ev]);

			PokeParty_RecalcStats(partyPkm);

			PokeParty_SetParam(partyPkm, PF_StatusCond, team[slot].status);
			
			u32 maxHP = PokeParty_GetParam(partyPkm, PF_MaxHP, nullptr);
			PokeParty_SetParam(partyPkm, PF_NowHP, (team[slot].hp * maxHP) / 100);
			
			PokeParty_AddPkm(party, partyPkm);
		}
	}

	GFL_HeapFree(trainerData);
	GFL_HeapFree(team);
	GFL_HeapFree(partyPkm);
}

#endif // PERFECT_TRAINERS