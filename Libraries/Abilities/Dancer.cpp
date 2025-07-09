#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/ActionOrderManager.h"

#include "server_flow.h"

// DANCER
extern "C" void HandlerDancerCheckMove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (!CheckExtraActionFlag()) {
        u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (pokemonSlot != currentSlot && getMoveFlag(moveID, FLAG_DANCE)) {

            BattleMon* dancerMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            // Dancer fails if the Dancer Pokémon is in a semi-invulnerable state
            if (BattleMon_CheckIfMoveCondition(dancerMon, CONDITION_SKYDROP) ||
                BattleMon_CheckIfMoveCondition(dancerMon, CONDITION_CHARGELOCK)) {
                return;
            }

            // The slowest Dancer Pokémon must move first, 
            // so we move all extra actions back and add the newest one to the start
            ShiftExtraActionOrders();

            ActionOrderWork* extraActionOrder = GetExtraActionOrder(0);
            extraActionOrder->battleMon = dancerMon;

            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
            for (u8 orderIdx = 0; orderIdx < 6; ++orderIdx) {
                if (serverFlow->actionOrderWork[orderIdx].battleMon == currentMon) {
                    extraActionOrder->action = serverFlow->actionOrderWork[orderIdx].action;

                    BattleAction_Fight* fight = (BattleAction_Fight*)&(extraActionOrder->action);

                    MoveTarget moveTarget = (MoveTarget)PML_MoveGetParam((MOVE_ID)fight->moveID, MVDATA_TARGET);
                    // The target position varies depending on the move and if the Pokémon is an ally or not
                    switch (moveTarget)
                    {
                    case TARGET_OTHER_SELECT:
                    case TARGET_ENEMY_SELECT:
                    case TARGET_ENEMY_RANDOM:
                        if (!MainModule_IsAllyMonID(pokemonSlot, currentSlot)) {
                            fight->targetPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
                        }
                        break;
                    case TARGET_ALLY_USER_SELECT:
                        fight->targetPos = Handler_PokeIDToPokePos(serverFlow, pokemonSlot);
                        break;
                    case TARGET_ALLY_SELECT:
                        fight->targetPos = CommonGetAllyPos(serverFlow, Handler_PokeIDToPokePos(serverFlow, pokemonSlot));
                        break;
                    default:
                        fight->targetPos = 6;
                    }

                    extraActionOrder->speed = serverFlow->actionOrderWork[orderIdx].speed;
                    extraActionOrder->partyID = serverFlow->actionOrderWork[orderIdx].partyID;
                    break;
                }
            }

            extraActionOrder->field_E = 0;
            extraActionOrder->field_F = 0;
        }
    }
}
extern "C" void HandlerDancerPopUp(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (CheckExtraActionFlag() && 
        pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerDancerMoveFail(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (CheckExtraActionFlag() && 
        pokemonSlot == currentSlot) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);

        // If the Dancer Pokémon is encored it tries to copy the move,
        // but if it's not the encored move it fails 
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_ENCORE)) {
            u8 actionIdx = 0;
            for (; actionIdx < 6; ++actionIdx) {
                if (GetExtraActionOrder(actionIdx)->battleMon == currentMon) {
                    break;
                }
            }
            BattleAction_Fight* fight = (BattleAction_Fight*)&(GetExtraActionOrder(actionIdx)->action);

            if (fight->moveID != moveID) {
                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_MOVELOCK);
                return;
            }
        }

        // If the Dancer Pokémon is locked to a move by a choice item or by a move 
        // like Petal Dance it tries to copy the move but it fails
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_MOVELOCK) ||
            BattleMon_CheckIfMoveCondition(currentMon, CONDITION_CHOICELOCK)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_MOVELOCK);
            return;
        }
    }
}
BattleEventHandlerTableEntry DancerHandlers[]{
    {EVENT_MOVE_EXECUTE_EFFECTIVE, HandlerDancerCheckMove},
    {EVENT_MOVE_EXECUTE_NOEFFECT, HandlerDancerCheckMove},
    {EVENT_MOVE_SEQUENCE_START, HandlerDancerPopUp},
    {EVENT_MOVE_EXECUTE_CHECK1, HandlerDancerMoveFail}
};
extern "C" BattleEventHandlerTableEntry * EventAddDancer(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DancerHandlers);
    return DancerHandlers;
}
#endif // EXPAND_ABILITIES