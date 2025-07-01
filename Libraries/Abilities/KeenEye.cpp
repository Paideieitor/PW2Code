#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// KEEN EYE
extern "C" void HandlerKeenEyeCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerKeenEyeGuard(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerKeenEyeEvasion(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_EVASION_STAGE) > 6) {
        BattleEventVar_RewriteValue(VAR_EVASION_STAGE, 6);
    }
}
BattleEventHandlerTableEntry KeenEyeHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerKeenEyeCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerKeenEyeGuard},
    {EVENT_MOVE_ACCURACY_STAGE, HandlerKeenEyeEvasion},
};
extern "C" BattleEventHandlerTableEntry * EventAddKeenEyeUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(KeenEyeHandlers);
    return KeenEyeHandlers;
}
#endif // EXPAND_ABILITIES