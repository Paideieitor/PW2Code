#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// PRANKSTER
extern "C" void HandlerPranksterUpdated(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        work[0] = 0;
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (!PML_MoveGetCategory(moveID)) {
            u8 priority = BattleEventVar_GetValue(VAR_MOVE_PRIORITY);
            BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, priority + 1);
            work[0] = 1;
        }
    }
}
extern "C" void HandlerPranksterImmune(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[0] == 1 &&
        pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        work[0] = 0;

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (BattleMon_HasType(defendingMon, TYPE_DARK)) {

            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry PranksterHandlers[]{
    {EVENT_GET_MOVE_PRIORITY, HandlerPranksterUpdated},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPranksterImmune},
};
extern "C" BattleEventHandlerTableEntry * EventAddPranksterUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PranksterHandlers);
    return PranksterHandlers;
}
#endif // EXPAND_ABILITIES