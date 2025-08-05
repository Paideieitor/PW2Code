#ifndef MOVE_EXPANSION_H
#define MOVE_EXPANSION_H

#include "battle_events.h"

extern "C" BattleEventHandlerTableEntry * EventAddFlyingPress(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMatBlock(u32 * handlerAmount);

extern "C" BattleEventHandlerTableEntry * EventAddCeaselessEdge(u32 * handlerAmount);

extern "C" void GetSideBattleSlots(ServerFlow * serverFlow, u32 side, u8 battleSlots[3]);

#endif // MOVE_EXPANSION_H
