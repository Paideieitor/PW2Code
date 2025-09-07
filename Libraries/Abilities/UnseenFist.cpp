#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// UNSEEN FIST
extern "C" void HandlerUnseenFistBreakProtect(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (IsContact(serverFlow, moveID, pokemonSlot)) {
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 2); // Value of 2 ingnores protect without breaking it
        }
    }
}
BattleEventHandlerTableEntry UnseenFistHandlers[]{
    {EVENT_CHECK_PROTECT_BREAK, HandlerUnseenFistBreakProtect},
};
extern "C" BattleEventHandlerTableEntry * EventAddUnseenFist(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(UnseenFistHandlers);
    return UnseenFistHandlers;
}
#endif // EXPAND_ABILITIES