#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// MIND'S EYE
extern "C" void CommonIgnoreEvasionHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleEventVar_RewriteValue(VAR_ACCURACY_STAGE, 6);
    }
}
extern "C" void HandlerScrappy(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerKeenEyeCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerKeenEyeGuard(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry MindsEyeHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerScrappy},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerKeenEyeCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerKeenEyeGuard},
    {EVENT_MOVE_ACCURACY_STAGE, CommonIgnoreEvasionHandler},
};
extern "C" BattleEventHandlerTableEntry * EventAddMindsEye(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MindsEyeHandlers);
    return MindsEyeHandlers;
}
#endif // EXPAND_ABILITIES