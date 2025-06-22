#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// DRAGON'S MAW
extern "C" void HandlerDragonsMaw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_DRAGON) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry DragonsMawHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerDragonsMaw},
};
extern "C" BattleEventHandlerTableEntry * EventAddDragonsMaw(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DragonsMawHandlers);
    return DragonsMawHandlers;
}
#endif // EXPAND_ABILITIES