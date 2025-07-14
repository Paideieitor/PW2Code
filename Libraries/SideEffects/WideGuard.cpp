#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

// WIDE GUARD
extern "C" void HandlerSideWideGuard(BattleEventItem * item, ServerFlow * serverFlow, u32 side, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (side == GetSideFromMonID(defendingSlot)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        MoveTarget targetType = (MoveTarget)PML_MoveGetParam(moveID, MVDATA_TARGET);
        if (PML_MoveIsDamaging(moveID) && targetType - TARGET_OTHER_ALL <= TARGET_ALLY_USER_SELECT) {

            if (CheckUnseenFist)
                return;

            if (BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1)) {
                HandlerParam_StrParams* strParam;
                strParam = (HandlerParam_StrParams*)BattleEventVar_GetValue(VAR_WORK_ADDRESS);
                BattleHandler_StrSetup(strParam, 2u, 797);
                BattleHandler_AddArg(strParam, defendingSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry SideWideGuardHandlers[]{
    {EVENT_NOEFFECT_CHECK, HandlerSideWideGuard},
};
extern "C" BattleEventHandlerTableEntry * EventAddSideWideGuardUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SideWideGuardHandlers);
    return SideWideGuardHandlers;
}
#endif // EXPAND_FIELD_EFFECTS