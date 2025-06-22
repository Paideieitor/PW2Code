#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// AERILATE
extern "C" void HandlerAerilateTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_FLY);
        }
    }
}
extern "C" void HandlerAerilatePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FLY) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry AerilateHandlers[]{
    {EVENT_MOVE_PARAM, HandlerAerilateTypeChange},
    {EVENT_MOVE_POWER, HandlerAerilatePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddAerilate(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AerilateHandlers);
    return AerilateHandlers;
}
#endif // EXPAND_ABILITIES