#ifndef __SIDE_EFFECTS_H
#define __SIDE_EFFECTS_H

#include "defs.h"

#include "battle_events.h"

typedef u32 SIDE_EFFECT;
enum SideEffect : u32
{
    SIDEEFF_REFLECT = 0x0,
    SIDEEFF_LIGHT_SCREEN = 0x1,
    SIDEEFF_SAFEGUARD = 0x2,
    SIDEEFF_MIST = 0x3,
    SIDEEFF_TAILWIND = 0x4,
    SIDEEFF_LUCKY_CHANT = 0x5,
    SIDEEFF_SPIKES = 0x6,
    SIDEEFF_TOXIC_SPIKES = 0x7,
    SIDEEFF_STEALTH_ROCK = 0x8,
    SIDEEFF_WIDE_GUARD = 0x9,
    SIDEEFF_QUICK_GUARD = 0xA,
    SIDEEFF_RAINBOW = 0xB,
    SIDEEFF_SEA_OF_FIRE = 0xC,
    SIDEEFF_SWAMP = 0xD,
};

typedef BattleEventHandlerTableEntry* (*SideEffectEventAddFunc)(u32*);

struct SideEffectEventAddTable
{
    SIDE_EFFECT sideEffect;
    SideEffectEventAddFunc func;
    u32 maxCount;
};

extern "C" BattleEventHandlerTableEntry* EventAddSideReflect(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideLightScreen(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideSafeguard(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideMist(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideTailwind(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideLuckyChant(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideSpikes(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideToxicSpikes(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideStealthRock(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideWideGuard(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideQuickGuard(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideRainbow(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideSeaOfFire(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSideSwamp(u32 * handlerAmount);

struct BattleEventItem;

extern "C" u32 BattleSideStatus_GetCountFromBattleEventItem(BattleEventItem* item, u32 side);

#define REMOVE_SIDE_EFFECT_SETUP(flags) flags[0] = 3; flags[1] = 0; flags[2] = 0
#define SET_REMOVE_SIDE_EFFEC_FLAG(sideEffect, flags) flags[(sideEffect >> 3) + 1] = flags[(sideEffect >> 3) + 1] | (1 << (sideEffect & 7))

#endif // __SIDE_EFFECTS_H

