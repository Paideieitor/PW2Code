#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// TOUGH CLAWS
extern "C" void HandlerToughClaws(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5461);
        }
    }
}
BattleEventHandlerTableEntry ToughClawsHandlers[]{
    {EVENT_MOVE_POWER, HandlerToughClaws},
};
extern "C" BattleEventHandlerTableEntry * EventAddToughClaws(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(ToughClawsHandlers);
    return ToughClawsHandlers;
}
#endif // EXPAND_ABILITIES