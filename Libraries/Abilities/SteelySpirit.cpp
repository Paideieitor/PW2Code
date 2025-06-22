#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// STEELY SPIRIT
extern "C" void HandlerSteelySpirit(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (pokemonSlot == attackingSlot || MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_STEEL) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry SteelySpiritHandlers[]{
    {EVENT_MOVE_POWER, HandlerSteelySpirit},
};
extern "C" BattleEventHandlerTableEntry * EventAddSteelySpirit(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SteelySpiritHandlers);
    return SteelySpiritHandlers;
}
#endif // EXPAND_ABILITIES