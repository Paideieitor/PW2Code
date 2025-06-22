#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// POWER SPOT
extern "C" void HandlerPowerSpot(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (pokemonSlot != attackingSlot &&
        MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {

        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5461);
    }
}
BattleEventHandlerTableEntry PowerSpotHandlers[]{
    {EVENT_MOVE_POWER, HandlerPowerSpot},
};
extern "C" BattleEventHandlerTableEntry * EventAddPowerSpot(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PowerSpotHandlers);
    return PowerSpotHandlers;
}
#endif // EXPAND_ABILITIES