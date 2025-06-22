#ifndef __TRAINER_DATA_H
#define __TRAINER_DATA_H

#include "defs.h"

struct TrainerData
{
	u8 flags;
	u8 trainerClass;
	u8 battleType;
	u8 pokemonCount;
	u16 items[4];
	u32 AI;
	u8 healer;
	u8 money;
	u16 itemAfterBattle;
};

#endif // __TRAINER_DATA_H
