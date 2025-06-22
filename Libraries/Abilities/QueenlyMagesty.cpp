#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/ActionOrderManager.h"

#include "server_flow.h"

// QUEENLY MAGESTY
extern "C" void HandlerQueenlyMagesty(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        defendingSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        ActionOrderWork* action_order = serverFlow->actionOrderWork;
        u16 orderIdx = 0;

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        for (; orderIdx < 6; ++orderIdx)
            if (action_order[orderIdx].battleMon == attackingMon)
                break;

        u32 priority = ACTION_ORDER_GET_PRIO(action_order, orderIdx);
        priority -= 7;
        // Special priority takes into account item & ability prio boosts (1 = no added prio)
        u32 special_priority = ACTION_ORDER_GET_SPECIAL_PRIO(action_order, orderIdx);
        special_priority -= 1;
        priority += special_priority;

        if (priority > 0) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry QueenlyMagestyHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerQueenlyMagesty},
};
extern "C" BattleEventHandlerTableEntry * EventAddQueenlyMagesty(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(QueenlyMagestyHandlers);
    return QueenlyMagestyHandlers;
}
#endif // EXPAND_ABILITIES