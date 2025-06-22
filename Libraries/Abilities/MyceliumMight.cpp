#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// MYCELIUM MIGHT 
extern "C" void HandlerMoldBreakerStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerMoldBreakerEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerMyceliumMightPriority(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            BattleEventVar_RewriteValue(VAR_PRIORITY, 0);
        }
    }
}
extern "C" void HandlerMyceliumMightMoldBreaker(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && !work[0]) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            HandlerMoldBreakerStart(item, serverFlow, pokemonSlot, work);
        }
    }
}
BattleEventHandlerTableEntry MyceliumMightHandlers[]{
    {EVENT_CHECK_SPECIAL_PRIORITY, HandlerMyceliumMightPriority},
    {EVENT_MOVE_SEQUENCE_START, HandlerMyceliumMightMoldBreaker},
    {EVENT_MOVE_SEQUENCE_END, HandlerMoldBreakerEnd},
};
extern "C" BattleEventHandlerTableEntry * EventAddMyceliumMight(u32 * handlerAmount) {
    *handlerAmount = GetNumHandlersWithPriority(EVENTPRI_ABILITY_STALL, ARRAY_COUNT(MyceliumMightHandlers));
    return MyceliumMightHandlers;
}
#endif // EXPAND_ABILITIES