#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// NORMALIZE
extern "C" void HandlerNormalize(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerNormalizePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry NormalizeHandlers[]{
    {EVENT_MOVE_PARAM, HandlerNormalize},
    {EVENT_MOVE_POWER, HandlerNormalizePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddNormalizeUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(NormalizeHandlers);
    return NormalizeHandlers;
}
#endif // EXPAND_ABILITIES