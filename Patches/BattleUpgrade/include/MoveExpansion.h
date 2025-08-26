#ifndef MOVE_EXPANSION_H
#define MOVE_EXPANSION_H

#include "battle_events.h"

extern "C" BattleEventHandlerTableEntry * EventAddFlyingPress(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMatBlock(u32 * handlerAmount); 
extern "C" BattleEventHandlerTableEntry* EventAddRototiller(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStickyWeb(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFellStinger(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTrickOrTreat(u32* handlerAmount);

extern "C" BattleEventHandlerTableEntry* EventAddForestsCurse(u32* handlerAmount);

extern "C" BattleEventHandlerTableEntry * EventAddCeaselessEdge(u32 * handlerAmount);

extern "C" BattleEventHandlerTableEntry* EventAddRapidSpinUpdated(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDefogUpdated(u32* handlerAmount);

extern "C" void GetSideBattleSlots(ServerFlow * serverFlow, u32 side, u8 battleSlots[3]);

#endif // MOVE_EXPANSION_H
