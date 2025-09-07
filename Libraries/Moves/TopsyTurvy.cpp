#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// TOPSY-TURVY
extern "C" bool ReverseStatStage(u8* statStage) {
    char stat = (char)(*statStage) - 6;
    if (stat == 0) {
        return false;
    }
    *statStage = (u8)((stat * -1) + 6);
    return true;
}
extern "C" bool ReverseStatStages(BattleMon* battleMon) {
    bool changed = false;
    if (ReverseStatStage(&(battleMon->statStageParam.AttackStage))) {
        changed = true;
    }
    if (ReverseStatStage(&(battleMon->statStageParam.DefenseStage))) {
        changed = true;
    }
    if (ReverseStatStage(&(battleMon->statStageParam.SpeedStage))) {
        changed = true;
    }
    if (ReverseStatStage(&(battleMon->statStageParam.SpAttackStage))) {
        changed = true;
    }
    if (ReverseStatStage(&(battleMon->statStageParam.SpDefenseStage))) {
        changed = true;
    }
    if (ReverseStatStage(&(battleMon->statStageParam.AccuracyStage))) {
        changed = true;
    }
    if (ReverseStatStage(&(battleMon->statStageParam.EvasionStage))) {
        changed = true;
    }
    return changed;
}
extern "C" void HandlerTopsyTurvy(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 idx = 0; idx < targetCount; ++idx) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + idx));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (!BattleMon_IsFainted(targetMon) &&
                ReverseStatStages(targetMon)) {

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, BATTLE_TOPSY_TURVY_MSGID);
                BattleHandler_PopWork(serverFlow, message);
            }
        }
    }
}
BattleEventHandlerTableEntry TopsyTurvyHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerTopsyTurvy},
};
extern "C" BattleEventHandlerTableEntry* EventAddTopsyTurvy(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TopsyTurvyHandlers);
    return TopsyTurvyHandlers;
}

#endif // EXPAND_MOVES