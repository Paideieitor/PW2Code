#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// DRAGONIZE
extern "C" void HandlerDragonizeTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_DRAGON);
        }
    }
}
extern "C" void HandlerDragonizePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_DRAGON) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry DragonizeHandlers[]{
    {EVENT_MOVE_PARAM, HandlerDragonizeTypeChange},
    {EVENT_MOVE_POWER, HandlerDragonizePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddDragonize(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DragonizeHandlers);
    return DragonizeHandlers;
}
#endif // EXPAND_ABILITIES
