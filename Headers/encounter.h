#ifndef __ENCOUNTER_H
#define __ENCOUNTER_H

#include "defs.h"

#include "save/save_control.h"

struct SWAN_ALIGNED(4) RoamingPokemon
{
    u16 nowZoneID;
    u8 nature;
    u32 ivs;
    u32 PID;
    u16 species;
    u16 currentHP;
    u8 level;
    u8 u8_11;
    u8 status;
    u8 u8_13;
};

struct SWAN_ALIGNED(2) EncountSave
{
    RoamingPokemon roamingPokemon[2];
    u8 roamingPokemonZoneClock[2];
    u8 field_2A[2];
    u8 currentSwarmLocation;
    u8 repelSteps;
    u16 nowRepelItemID;
    u32 field_30;
};

extern "C" EncountSave * SaveControl_GetEncountSave(SaveControl* saveCtrl);

#endif // __ENCOUNTER_H
