#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// ROCKY PAYLOAD
extern "C" void HandlerRockyPayload(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ROCK) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry RockyPayloadHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerRockyPayload},
};
extern "C" BattleEventHandlerTableEntry * EventAddRockyPayload(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RockyPayloadHandlers);
    return RockyPayloadHandlers;
}
#endif // EXPAND_ABILITIES