#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// STRONG JAW
extern "C" void HandlerStrongJaw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_BITE)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry StrongJawHandlers[]{
    {EVENT_MOVE_POWER, HandlerStrongJaw},
};
extern "C" BattleEventHandlerTableEntry * EventAddStrongJaw(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StrongJawHandlers);
    return StrongJawHandlers;
}
#endif // EXPAND_ABILITIES