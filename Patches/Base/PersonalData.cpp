#include "settings.h"

#include "personal.h"
#include "battle_mon.h"

#if EXPAND_ABILITIES

// Allow the personal narc to read up to 1024 abilities
extern "C" u32 THUMB_BRANCH_PML_PersonalGetParam(PersonalData * personal, PersonalField field)
{
    switch (field)
    {
    case Personal_HP:
        return personal->baseHP;
    case Personal_ATK:
        return personal->baseATK;
    case Personal_DEF:
        return personal->baseDEF;
    case Personal_SPE:
        return personal->baseSPE;
    case Personal_SPA:
        return personal->baseSPA;
    case Personal_SPD:
        return personal->baseSPD;
    case Personal_Type1:
        return personal->type1;
    case Personal_Type2:
        return personal->type2;
    case Personal_CaptureRate:
        return personal->captureRate;
    case Personal_BaseEXP:
        return personal->baseEXP;
    case Personal_EvHP:
        return personal->evYield & 0x0003;
    case Personal_EvATK:
        return (personal->evYield & 0x000C) >> 2;
    case Personal_EvDEF:
        return (personal->evYield & 0x0030) >> 4;
    case Personal_EvSPE:
        return (personal->evYield & 0x00C0) >> 6;
    case Personal_EvSPA:
        return (personal->evYield & 0x0300) >> 8;
    case Personal_EvSPD:
        return (personal->evYield & 0x0C00) >> 10;
    case Personal_SpriteGrounded:
        return !((personal->evYield & 0x1000) == 0);
    case Personal_WildItem50:
    case Personal_WildItem5:
    case Personal_WildItem1:
        // Last 2 bits of the item value are reserved for the respective ability slot
        return personal->items[field - Personal_WildItem50] & 0x3FFF;
    case Personal_GenderProb:
        return personal->genderProb;
    case Personal_EggHappiness:
        return personal->eggHappiness;
    case Personal_BaseHappiness:
        return personal->baseHappiness;
    case Personal_GrowthRate:
        return personal->expGroup;
    case Personal_EggGroup1:
        return personal->eggGroup1;
    case Personal_EggGroup2:
        return personal->eggGroup2;
    case Personal_Abil1:
    case Personal_Abil2:
    case Personal_AbilH:
    {
        // We take the last 2 bits of the corresponding item value to get the ability value
        u8 idx = (u8)(field - Personal_Abil1);
        return (u16)personal->abilities[idx] + ((personal->items[idx] & 0xC000) >> 6);
    }
    case Personal_EscapeRate:
        return personal->escapeRate;
    case Personal_FormeDataOffs:
        return personal->formeDataOffs;
    case Personal_FormeSpritesOffset:
        return personal->formeSpriteOffs;
    case Personal_FormeCount:
        return personal->formeCount;
    case Personal_Color:
        return personal->color & 0x3F;
    case Personal_SpriteFlip:
        return (personal->color & 0x40) >> 6;
    case Personal_SpriteForme:
        return (personal->color & 0x80) >> 7;
    case Personal_EvoStage:
        return personal->evoStage;
    case Personal_Height:
        return personal->heightCm;
    case Personal_Weight:
        return personal->weightCg;
    case Personal_TMHM1:
        return personal->tmhm1;
    case Personal_TMHM2:
        return personal->tmhm2;
    case Personal_TMHM3:
        return personal->tmhm3;
    case Personal_TMHM4:
        return personal->tmhm4;
    case Personal_TypeTutor:
        return personal->typeTutors;
    case Personal_SpecialTutor1:
        return personal->specialTutors[0];
    case Personal_SpecialTutor2:
        return personal->specialTutors[1];
    case Personal_SpecialTutor3:
        return personal->specialTutors[2];
    case Personal_SpecialTutor4:
        return personal->specialTutors[3];
    }
    return 0;
}

// Allow the game to read up to 1024 abilities
extern "C" u32 PML_PkmGetParamMod(BoxPkm * boxPkm, PkmField field, void* extra)
{
    if (field == PF_Ability)
    {
        BoxPkmBlock0* block = (BoxPkmBlock0*)PML_PkmGetParamBlockCore(boxPkm, boxPkm->pid, 0);
        u32 ability = block->Ability;
        ability += ((u32)block->Markings & 0xC0) << 2;
        return ability;
    }
    if (field == PF_Markings)
    {
        BoxPkmBlock0* block = (BoxPkmBlock0*)PML_PkmGetParamBlockCore(boxPkm, boxPkm->pid, 0);
        return block->Markings & 0x3F;
    }
    return PML_PkmGetParamCore(boxPkm, field, extra);
}
extern "C" u32 THUMB_BRANCH_LINK_PML_PkmGetParam_0x12(BoxPkm * boxPkm, PkmField field, void* extra)
{
    return PML_PkmGetParamMod(boxPkm, field, extra);
}
extern "C" u32 THUMB_BRANCH_LINK_PokeParty_GetParamCore_0x7A(BoxPkm * boxPkm, PkmField field, void* extra)
{
    return PML_PkmGetParamMod(boxPkm, field, extra);
}

// Allow the game to store up to 1024 abilities
extern "C" void PML_PkmSetParamMod(BoxPkm * boxPkm, PkmField field, u32 value)
{
    if (field == PF_Ability)
    {
        BoxPkmBlock0* block = (BoxPkmBlock0*)PML_PkmGetParamBlockCore(boxPkm, boxPkm->pid, 0);
        block->Ability = value & 0xFF;
        block->Markings = ((value & 0x0300) >> 2) + (block->Markings & 0x3F);
    }
    if (field == PF_Markings)
    {
        BoxPkmBlock0* block = (BoxPkmBlock0*)PML_PkmGetParamBlockCore(boxPkm, boxPkm->pid, 0);
        block->Markings = (block->Markings & 0xC0) + (value & 0x3F);
    }
    PML_PkmSetParamCore(boxPkm, field, value);
}
extern "C" void THUMB_BRANCH_LINK_PML_PkmSetParam_0x16(BoxPkm * boxPkm, PkmField field, u32 value)
{
    PML_PkmSetParamMod(boxPkm, field, value);
}
extern "C" void THUMB_BRANCH_LINK_PokeParty_SetParamCore_0x7A(BoxPkm * boxPkm, PkmField field, u32 value)
{
    PML_PkmSetParamMod(boxPkm, field, value);
}

#endif // EXPAND_ABILITIES