#ifndef __TYPES_H
#define __TYPES_H

#include "settings.h"

typedef u32 POKE_TYPE;
enum PokeType : u32
{
	TYPE_NORMAL = 0x0,
	TYPE_FIGHT = 0x1,
	TYPE_FLY = 0x2,
	TYPE_POIS = 0x3,
	TYPE_GROUND = 0x4,
	TYPE_ROCK = 0x5,
	TYPE_BUG = 0x6,
	TYPE_GHOST = 0x7,
	TYPE_STEEL = 0x8,
	TYPE_FIRE = 0x9,
	TYPE_WATER = 0xA,
	TYPE_GRASS = 0xB,
	TYPE_ELEC = 0xC,
	TYPE_PSY = 0xD,
	TYPE_ICE = 0xE,
	TYPE_DRAGON = 0xF,
	TYPE_DARK = 0x10,
#if ADD_FAIRY_TYPE
	TYPE_FAIRY = 0x11,
	TYPE_NULL = 0x12,
#else
	TYPE_NULL = 0x11,
#endif
};

enum TypeEffectiveness : u32
{
	EFFECTIVENESS_IMMUNE = 0x0,
	EFFECTIVENESS_1_4 = 0x1,
	EFFECTIVENESS_1_2 = 0x2,
	EFFECTIVENESS_1 = 0x3,
	EFFECTIVENESS_2 = 0x4,
	EFFECTIVENESS_4 = 0x5,
};

// Type effectiveness labels.
#define NOT_EFFECTIVE 0
#define NOT_VERY_EFFECTIVE 2
#define EFFECTIVE 4
#define SUPER_EFFECTIVE 8

extern "C" u16 PokeTypePair_MakeMonotype(u16 type);
extern "C" u16 PokeTypePair_Make(u16 type1, u16 type2);
extern "C" TypeEffectiveness GetTypeEffectiveness(POKE_TYPE moveType, POKE_TYPE pokemonType);
extern "C" TypeEffectiveness GetTypeEffectivenessVsMon(POKE_TYPE type, u32 typePair);
extern "C" TypeEffectiveness GetTypeEffectivenessMultiplier(TypeEffectiveness effectiveness1, TypeEffectiveness effectiveness2);

#endif // __TYPES_H