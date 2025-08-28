#include "settings.h"

#include "poke_party.h"

#include "GameVariables.h"

#if ADD_LEVEL_CAP && DAY_CARE_LVL_CAP

extern "C" void DayCare_CommitPkmGrowth(PartyPkm* pkm, u32 steps);
extern "C" void THUMB_BRANCH_LINK_DayCare_RemovePkm_0x28(PartyPkm* pkm, u32 steps) {
	u32 level = PokeParty_GetParam(pkm, PF_Level, 0);
	u16 levelCap = GetLvlCap();
	if (level < levelCap) {
		DayCare_CommitPkmGrowth(pkm, steps);
	}
}

extern "C" u32 CalcLevelByExp(u16 species, u16 form, u32 exp);
extern "C" u32 PML_UtilGetPkmLvExp(u16 species, u16 form, int level);
extern "C" void THUMB_BRANCH_LINK_DayCare_CommitPkmGrowth_0x58(PartyPkm* pkm, PkmField field, u32 exp) {
	u16 species = PokeParty_GetParam(pkm, PF_Species, 0);
	u16 form = PokeParty_GetParam(pkm, PF_Forme, 0);
	u32 level = CalcLevelByExp(species, form, exp);

	u16 levelCap = GetLvlCap();
	if (level >= levelCap) {
		exp = PML_UtilGetPkmLvExp(species, form, levelCap);
	}

	PokeParty_SetParam(pkm, PF_Experience, exp);
}

extern "C" b32 DayCareSave_GetPkmStatus(void* dayCare, u32 slot);
extern "C" PartyPkm* DayCareSave_GetPkm(void* dayCare, u32 slot);
extern "C" u32 DayCareSave_GetPkmStepCounter(void* dayCare, u32 slot);
extern "C" u32 DayCare_CalcNewExp(u32 exp, u32 steps);
extern "C" u32 THUMB_BRANCH_DayCare_CalcNewLevel(DayCare* dayCare, u32 slot) {
	if (!DayCareSave_GetPkmStatus(dayCare->m_DayCareSave, slot)) {
		return 1;
	}
	PartyPkm* dayCarePkm = DayCareSave_GetPkm(dayCare->m_DayCareSave, slot);

	u32 level = PokeParty_GetParam(dayCarePkm, PF_Level, 0);
	u16 levelCap = GetLvlCap();
	if (level >= levelCap) {
		return level;
	}

	u32 species = PokeParty_GetParam(dayCarePkm, PF_Species, 0);
	u32 form = PokeParty_GetParam(dayCarePkm, PF_Forme, 0);
	u32 exp = PokeParty_GetParam(dayCarePkm, PF_Experience, 0);
	u32 dayCareSteps = DayCareSave_GetPkmStepCounter(dayCare->m_DayCareSave, slot);
	u32 newExp = DayCare_CalcNewExp(exp, dayCareSteps);
	u32 newLevel = CalcLevelByExp(species, form, newExp);
	if (newLevel >= levelCap) {
		return levelCap;
	}
	return newLevel;
}

#endif // ADD_LEVEL_CAP