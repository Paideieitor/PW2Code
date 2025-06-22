#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// REFRIGERATE
extern "C" void HandlerRefrigerateTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_ICE);
        }
    }
}
extern "C" void HandlerRefrigeratePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ICE) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry RefrigerateHandlers[]{
    {EVENT_MOVE_PARAM, HandlerRefrigerateTypeChange},
    {EVENT_MOVE_POWER, HandlerRefrigeratePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddRefrigerate(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RefrigerateHandlers);
    return RefrigerateHandlers;
}
#endif // EXPAND_ABILITIES