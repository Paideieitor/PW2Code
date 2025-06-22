#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// BATTERY
extern "C" void HandlerBattery(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (pokemonSlot != attackingSlot && MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {
        if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {
            BattleEventVar_MulValue(VAR_RATIO, 5325);
        }
    }
}
BattleEventHandlerTableEntry BatteryHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerBattery},
};
extern "C" BattleEventHandlerTableEntry * EventAddBattery(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BatteryHandlers);
    return BatteryHandlers;
}
#endif // EXPAND_ABILITIES