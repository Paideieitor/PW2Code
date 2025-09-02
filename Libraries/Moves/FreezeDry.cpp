#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// FREEZE DRY
extern "C" void HandlerFreezeDry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_POKE_TYPE) == TYPE_WATER) {

        BattleEventVar_RewriteValue(VAR_SET_TYPE_EFFECTIVENESS, OVERRIDE_EFFECTIVENESS_2);
    }
}
BattleEventHandlerTableEntry FreezeDryHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerFreezeDry},
};
extern "C" BattleEventHandlerTableEntry* EventAddFreezeDry(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FreezeDryHandlers);
    return FreezeDryHandlers;
}

#endif // EXPAND_MOVES