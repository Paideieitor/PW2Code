#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// MERCILESS
extern "C" void HandlerMerciless(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (BattleMon_GetStatus(defendingMon) == CONDITION_POISON) {
            BattleEventVar_RewriteValue(VAR_CRIT_STAGE, 4);
        }
    }
}
BattleEventHandlerTableEntry MercilessHandlers[]{
    {EVENT_CRITICAL_CHECK, HandlerMerciless},
};
extern "C" BattleEventHandlerTableEntry * EventAddMerciless(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MercilessHandlers);
    return MercilessHandlers;
}
#endif // EXPAND_ABILITIES