#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// FLUFFY
extern "C" void HandlerFluffy(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        u32 ratio = 4096;

        POKE_TYPE moveType = BattleEventVar_GetValue(VAR_MOVE_TYPE);
        if (moveType == TYPE_FIRE) {
            ratio *= 2;
        }

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, pokemonSlot)) {
            ratio /= 2;
        }

        BattleEventVar_MulValue(VAR_RATIO, ratio);
    }
}
BattleEventHandlerTableEntry FluffyHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerFluffy},
};
extern "C" BattleEventHandlerTableEntry * EventAddFluffy(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FluffyHandlers);
    return FluffyHandlers;
}
#endif // EXPAND_ABILITIES