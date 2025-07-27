#include "settings.h"

#include "poke_party.h"
#include "trainer_data.h"

#ifdef PERFECT_TRAINERS

struct TrainerPokemonExt
{
	u16 IVs;
};

extern "C" void THUMB_BRANCH_LINK_TrainerUtil_LoadParty_0x4C8(PokeParty* party, PartyPkm* partyPkm, TrainerData* trainerData, TrainerPokemonExt* team) {
	if (trainerData->trainerType == PERFECT_TRAINER) {
		for (u32 slot = 0; slot < trainerData->pokemonCount; ++slot) {
			team[slot];
		}
	}

	GFL_HeapFree(trainerData);
	GFL_HeapFree(team);
	GFL_HeapFree(partyPkm);
}

#endif // PERFECT_TRAINERS