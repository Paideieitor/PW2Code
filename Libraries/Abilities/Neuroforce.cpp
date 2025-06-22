#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// NEUROFORCE
extern "C" void HandlerNeuroforce(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 effectiveness = BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS);
        if (effectiveness > EFFECTIVENESS_1) {
            BattleEventVar_MulValue(VAR_RATIO, 5120);
        }
    }
}
BattleEventHandlerTableEntry NeuroforceHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerNeuroforce},
};
extern "C" BattleEventHandlerTableEntry * EventAddNeuroforce(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(NeuroforceHandlers);
    return NeuroforceHandlers;
}
#endif // EXPAND_ABILITIES