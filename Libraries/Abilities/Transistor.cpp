#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// TRANSISTOR
extern "C" void HandlerTransistor(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry TransistorHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerTransistor},
};
extern "C" BattleEventHandlerTableEntry * EventAddTransistor(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TransistorHandlers);
    return TransistorHandlers;
}
#endif // EXPAND_ABILITIES