#ifndef __FIELD_EFFECTS_H
#define __FIELD_EFFECTS_H

#include "defs.h"

#include "battle_events.h"

typedef u32 WEATHER;
enum Weather : u32
{
    WEATHER_NULL = 0,
    WEATHER_SUN = 1,
    WEATHER_RAIN = 2,
    WEATHER_HAIL = 3,
    WEATHER_SANDSTORM = 4,
};

typedef u32 FIELD_EFFECT;
enum FieldEffect : u32
{
    FLDEFF_WEATHER = 0x0,
    FLDEFF_TRICK_ROOM = 0x1,
    FLDEFF_GRAVITY = 0x2,
    FLDEFF_IMPRISON = 0x3,
    FLDEFF_WATER_SPORT = 0x4,
    FLDEFF_MUD_SPORT = 0x5,
    FLDEFF_WONDER_ROOM = 0x6,
    FLDEFF_MAGIC_ROOM = 0x7,
};

typedef BattleEventHandlerTableEntry* (*FieldEffectEventAddFunc)(u32*);

struct FieldEffectEventAddTable
{
    FIELD_EFFECT fieldEffect;
    FieldEffectEventAddFunc handlers;
};

extern "C" BattleEventHandlerTableEntry * EventAddFieldWeather(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldTrickRoom(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldGravity(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldImprison(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldWaterSport(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldMudSport(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldWonderRoom(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldMagicRoom(u32 * handlerAmount);

#endif // __FIELD_EFFECT_H
