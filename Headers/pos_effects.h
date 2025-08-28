#ifndef __POS_EFFECTS_H
#define __POS_EFFECTS_H

#include "defs.h"

#include "battle_events.h"

typedef u32 POS_EFFECT;
enum PosEffect : u32
{
    POSEFF_WISH = 0x0,
    POSEFF_LUNAR_DANCE = 0x1,
    POSEFF_HEALING_WISH = 0x2,
    POSEFF_FUTURE_SIGHT = 0x3,
    POSEFF_BATON_PASS = 0x4,
};


typedef BattleEventHandlerTableEntry* (*PosEffectEventAddFunc)(u32*);

struct PosEffectEventAddTable
{
    POS_EFFECT posEffect;
    PosEffectEventAddFunc func;
};

extern "C" b32 PosEffectEvent_CanEffectBeRegistered(POS_EFFECT posEffect, u32 battleSlot);

#endif // __POS_EFFECTS_H

