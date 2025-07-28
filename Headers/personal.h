#ifndef __PERSONAL_H
#define __PERSONAL_H

#include "defs.h"

enum PersonalField : u32
{
	Personal_HP = 0x0,
	Personal_ATK = 0x1,
	Personal_DEF = 0x2,
	Personal_SPE = 0x3,
	Personal_SPA = 0x4,
	Personal_SPD = 0x5,
	Personal_Type1 = 0x6,
	Personal_Type2 = 0x7,
	Personal_CaptureRate = 0x8,
	Personal_BaseEXP = 0x9,
	Personal_EvHP = 0xA,
	Personal_EvATK = 0xB,
	Personal_EvDEF = 0xC,
	Personal_EvSPE = 0xD,
	Personal_EvSPA = 0xE,
	Personal_EvSPD = 0xF,
	Personal_SpriteGrounded = 0x10,
	Personal_WildItem50 = 0x11,
	Personal_WildItem5 = 0x12,
	Personal_WildItem1 = 0x13,
	Personal_GenderProb = 0x14,
	Personal_EggHappiness = 0x15,
	Personal_BaseHappiness = 0x16,
	Personal_GrowthRate = 0x17,
	Personal_EggGroup1 = 0x18,
	Personal_EggGroup2 = 0x19,
	Personal_Abil1 = 0x1A,
	Personal_Abil2 = 0x1B,
	Personal_AbilH = 0x1C,
	Personal_EscapeRate = 0x1D,
	Personal_FormeDataOffs = 0x1E,
	Personal_FormeSpritesOffset = 0x1F,
	Personal_FormeCount = 0x20,
	Personal_Color = 0x21,
	Personal_SpriteFlip = 0x22,
	Personal_SpriteForme = 0x23,
	Personal_EvoStage = 0x24,
	Personal_Height = 0x25,
	Personal_Weight = 0x26,
	Personal_TMHM1 = 0x27,
	Personal_TMHM2 = 0x28,
	Personal_TMHM3 = 0x29,
	Personal_TMHM4 = 0x2A,
	Personal_TypeTutor = 0x2B,
	Personal_SpecialTutor1 = 0x2C,
	Personal_SpecialTutor2 = 0x2D,
	Personal_SpecialTutor3 = 0x2E,
	Personal_SpecialTutor4 = 0x2F,
};

struct PersonalData
{
	u8 baseHP;
	u8 baseATK;
	u8 baseDEF;
	u8 baseSPE;
	u8 baseSPA;
	u8 baseSPD;
	u8 type1;
	u8 type2;
	u8 captureRate;
	u8 evoStage;
	u16 evYield;
	u16 items[3];
	u8 genderProb;
	u8 eggHappiness;
	u8 baseHappiness;
	u8 expGroup;
	u8 eggGroup1;
	u8 eggGroup2;
	u8 abilities[3];
	u8 escapeRate;
	u16 formeDataOffs;
	u16 formeSpriteOffs;
	u8 formeCount;
	u8 color;
	u16 baseEXP;
	u16 heightCm;
	u16 weightCg;
	u32 tmhm1;
	u32 tmhm2;
	u32 tmhm3;
	u32 tmhm4;
	u32 typeTutors;
	u32 specialTutors[4];
};

extern "C" u32 PML_PersonalGetParamSingle(u16 species, u16 form, PersonalField field);
extern "C" PersonalData* PML_PersonalLoad(u16 species, u16 form, u16 heapId);
extern "C" u32 PML_PersonalGetParam(PersonalData* personal, PersonalField field);
extern "C" void PML_PersonalFree(PersonalData* personal);

#endif // __PERSONAL_H
