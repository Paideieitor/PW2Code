#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// ASSAULT VEST
extern "C" void HandlerAssaultVestDefense(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_SPECIAL)
            BattleEventVar_MulValue(VAR_RATIO, 6144);
    }
}
BattleEventHandlerTableEntry AssaultVestHandlers[] = {
    {EVENT_DEFENDER_GUARD, HandlerAssaultVestDefense}, // not using status moves effect is here -> IsUnselectableMove
};
extern "C" BattleEventHandlerTableEntry* EventAddAssaultVest(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AssaultVestHandlers);
    return AssaultVestHandlers;
}
#endif // EXPAND_ITEMS