#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// SHARPNESS
extern "C" void HandlerSharpness(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_SHARP)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry SharpnessHandlers[]{
    {EVENT_MOVE_POWER, HandlerSharpness},
};
extern "C" BattleEventHandlerTableEntry * EventAddSharpness(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SharpnessHandlers);
    return SharpnessHandlers;
}
#endif // EXPAND_ABILITIES