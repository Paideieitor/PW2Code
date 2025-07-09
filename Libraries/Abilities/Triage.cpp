#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// TRIAGE
extern "C" void HandlerTriage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        int moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_HEALING)) {
            BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 3);
        }
    }
}
BattleEventHandlerTableEntry TriageHandlers[]{
    {EVENT_GET_MOVE_PRIORITY, HandlerTriage},
};
extern "C" BattleEventHandlerTableEntry * EventAddTriage(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TriageHandlers);
    return TriageHandlers;
}
#endif // EXPAND_ABILITIES