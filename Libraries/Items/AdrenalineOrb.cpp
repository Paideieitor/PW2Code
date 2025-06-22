#include "settings.h"
#if EXPAND_ITEMS

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// ADRENALINE ORB
// work[0] -> stores the intimidate flag
extern "C" void HandlerAdrenalineOrbIntimCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && BattleEventVar_GetValue(VAR_INTIMIDATE_FLAG)) {
        work[0] = 1;
    }
}
extern "C" void HandlerAdrenalineOrbReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        work[0] != 0) {
        work[0] = 0;

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(currentMon, STATSTAGE_SPEED, 1)) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void HandlerAdrenalineOrbUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_ChangeStatStage* speedBoost;
        speedBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        speedBoost->pokeCount = 1;
        speedBoost->pokeID[0] = (u8)pokemonSlot;
        speedBoost->stat = STATSTAGE_SPEED;
        speedBoost->volume = 1;
        BattleHandler_PopWork(serverFlow, speedBoost);
    }
}
BattleEventHandlerTableEntry AdrenalineOrbHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerAdrenalineOrbIntimCheck},
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerAdrenalineOrbReaction},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerAdrenalineOrbReaction},
    {EVENT_USE_ITEM, HandlerAdrenalineOrbUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddAdrenalineOrb(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(AdrenalineOrbHandlers);
    return AdrenalineOrbHandlers;
}
#endif // EXPAND_ITEMS