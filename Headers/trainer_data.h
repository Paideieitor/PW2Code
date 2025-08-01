#ifndef __TRAINER_DATA_H
#define __TRAINER_DATA_H

#include "defs.h"

struct TrainerData
{
	u8 trainerType;
	u8 trainerClass;
	u8 battleType;
	u8 pokemonCount;
	u16 heldItems[4];
	u32 AI;
	u8 isHealer;
	u8 money;
	u16 itemAfterBattle;
};

enum TrainerType : u32
{
	SIMPLE_TRAINER,
	MOVES_TRAINER,
	ITEM_TRAINER,
	GOOD_TRAINER,
	PERFECT_TRAINER,
};

enum AbilLock
{
	ABIL_0 = 0x0,
	ABIL_1 = 0x1,
	ABIL_ANY = 0x2,
	ABIL_HIDDEN = 0x3,
};

#endif // __TRAINER_DATA_H
