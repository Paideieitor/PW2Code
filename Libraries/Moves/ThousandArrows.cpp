#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

extern "C" void HandlerSmackDown(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);

// THOUSAND ARROWS
extern "C" void HandlerThousandArrowsFloatingFail(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}
extern "C" void HandlerThousandArrowsGrounded(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 targetIdx = 0; targetIdx < targetCount; ++targetIdx) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + targetIdx));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            bool knockedDown = 0;
            if (!Handler_IsMonInSkyDrop(serverFlow, targetSlot)) {

                if (Handler_CheckFloating(serverFlow,targetSlot)
                        && !BattleMon_CheckIfMoveCondition(targetMon, CONDITION_SKYDROP)) {

                    HandlerParam_AddCondition* addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
                    addCondition->pokeID = targetSlot;
                    addCondition->condition = CONDITION_KNOCKEDDOWN;
                    addCondition->condData = Condition_MakePermanent();
                    BattleHandler_StrSetup(&addCondition->exStr, 2u, 1128u);
                    BattleHandler_AddArg(&addCondition->exStr, targetSlot);
                    BattleHandler_PopWork(serverFlow, addCondition);
                    knockedDown = 1;
                    if (BattleMon_CheckIfMoveCondition(targetMon, CONDITION_FLOATING)) {

                        HandlerParam_CureCondition* cureFloatCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
                        cureFloatCondition->pokeCount = 1;
                        cureFloatCondition->pokeID[0] = targetSlot;
                        cureFloatCondition->msgDisable = 1;
                        cureFloatCondition->condition = CONDITION_FLOATING;
                        BattleHandler_PopWork(serverFlow, cureFloatCondition);
                    }
                    if (BattleMon_CheckIfMoveCondition(targetMon, CONDITION_TELEKINESIS)) {

                        HandlerParam_CureCondition* cureTelekinesisCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
                        cureTelekinesisCondition->pokeCount = 1;
                        cureTelekinesisCondition->pokeID[0] = targetSlot;
                        cureTelekinesisCondition->msgDisable = 1;
                        cureTelekinesisCondition->condition = CONDITION_TELEKINESIS;
                        BattleHandler_PopWork(serverFlow, cureTelekinesisCondition);
                    }
                }
                if (BattleMon_GetConditionFlag(targetMon, CONDITIONFLAG_FLY)) {

                    HandlerParam_HideTurnCancel* hideTurnCancel = (HandlerParam_HideTurnCancel*)BattleHandler_PushWork(serverFlow, EFFECT_CANCEL_SEMI_INVULN, pokemonSlot);
                    hideTurnCancel->pokeID = targetSlot;
                    hideTurnCancel->flag = 0x3;
                    if (!knockedDown) {

                        BattleHandler_StrSetup(&hideTurnCancel->exStr, 2u, 1128u);
                        BattleHandler_AddArg(&hideTurnCancel->exStr, targetSlot);
                    }
                    BattleHandler_PopWork(serverFlow, hideTurnCancel);
                }
            }
        }
    }
}

BattleEventHandlerTableEntry ThousandArrowsHandlers[]{
    {EVENT_CHECK_FLOATING, HandlerThousandArrowsFloatingFail},
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerThousandArrowsGrounded},
};
extern "C" BattleEventHandlerTableEntry* EventAddThousandArrows(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ThousandArrowsHandlers);
    return ThousandArrowsHandlers;
}

#endif // EXPAND_MOVES
