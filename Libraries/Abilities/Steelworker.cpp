#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// STEELWORKER
extern "C" void HandlerSteelworker(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_STEEL) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6138);
    }
}
BattleEventHandlerTableEntry SteelworkerHandlers[]{
    {EVENT_MOVE_POWER, HandlerSteelworker},
};
extern "C" BattleEventHandlerTableEntry * EventAddSteelworker(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SteelworkerHandlers);
    return SteelworkerHandlers;
}
#endif // EXPAND_ABILITIES