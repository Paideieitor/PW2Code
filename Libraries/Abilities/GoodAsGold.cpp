#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// GOOD AS GOLD
extern "C" void HandlerGoodAsGold(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry GoodAsGoldHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerGoodAsGold},
};
extern "C" BattleEventHandlerTableEntry * EventAddGoodAsGold(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GoodAsGoldHandlers);
    return GoodAsGoldHandlers;
}
#endif // EXPAND_ABILITIES