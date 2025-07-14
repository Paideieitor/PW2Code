#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

// QUICK GUARD
extern "C" void HandlerSideQuickGuard(BattleEventItem * item, ServerFlow * serverFlow, u32 side, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (side == GetSideFromMonID(defendingSlot)
        && defendingSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        ActionOrderWork* actionOrder = serverFlow->actionOrderWork;
        u16 orderIdx = 0;

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        for (; orderIdx < 6; ++orderIdx)
            if (actionOrder[orderIdx].battleMon == attackingMon)
                break;

        u32 priority = ACTION_ORDER_GET_PRIO(actionOrder, orderIdx);
        priority -= 7;
        // Special priority takes into account item & ability prio boosts (1 = no added prio)
        u32 specialPriority = ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, orderIdx);
        specialPriority -= 1;
        priority += specialPriority;

        if (priority > 0) {
            if (CheckUnseenFist)
                return;

            if (BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1)) {
                HandlerParam_StrParams* strParam;
                strParam = (HandlerParam_StrParams*)BattleEventVar_GetValue(VAR_WORK_ADDRESS);
                BattleHandler_StrSetup(strParam, 2u, 800);
                BattleHandler_AddArg(strParam, defendingSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry SideQuickGuardHandlers[]{
    {EVENT_NOEFFECT_CHECK, HandlerSideQuickGuard},
};
extern "C" BattleEventHandlerTableEntry * EventAddSideQuickGuardUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SideQuickGuardHandlers);
    return SideQuickGuardHandlers;
}
#endif // EXPAND_FIELD_EFFECTS