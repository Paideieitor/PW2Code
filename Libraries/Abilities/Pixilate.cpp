#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// PIXILATE
extern "C" void HandlerPixilateTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_FAIRY);
        }
    }
}
extern "C" void HandlerPixilatePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FAIRY) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry PixilateHandlers[]{
    {EVENT_MOVE_PARAM, HandlerPixilateTypeChange},
    {EVENT_MOVE_POWER, HandlerPixilatePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddPixilate(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PixilateHandlers);
    return PixilateHandlers;
}
#endif // EXPAND_ABILITIES