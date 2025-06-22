#include "settings.h"
#if EXPAND_ITEMS

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// BLUNDER POLICY
extern "C" void HandlerBlunderPolicyCheckMiss(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON)) {
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(battleMon, STATSTAGE_SPEED, 1)) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void HandlerBlunderPolicyUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_ChangeStatStage* speedBoost;
        speedBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        speedBoost->pokeCount = 1;
        speedBoost->pokeID[0] = (u8)pokemonSlot;
        speedBoost->stat = STATSTAGE_SPEED;
        speedBoost->volume = 2;
        BattleHandler_PopWork(serverFlow, speedBoost);
    }
}
BattleEventHandlerTableEntry BlunderPolicyHandlers[] = {
    {EVENT_ACCURACY_MISS, HandlerBlunderPolicyCheckMiss},
    {EVENT_USE_ITEM, HandlerBlunderPolicyUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddBlunderPolicy(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(BlunderPolicyHandlers);
    return BlunderPolicyHandlers;
}
#endif // EXPAND_ITEMS