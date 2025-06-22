#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// SNOWBALL
extern "C" void HandlerSnowballDamageReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ICE) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_ATTACK, 1))
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerSnowballUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_ChangeStatStage* attackBoost;
        attackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        attackBoost->pokeCount = 1;
        attackBoost->pokeID[0] = (u8)pokemonSlot;
        attackBoost->stat = STATSTAGE_ATTACK;
        attackBoost->volume = 1;
        BattleHandler_PopWork(serverFlow, attackBoost);
    }
}
BattleEventHandlerTableEntry SnowballHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSnowballDamageReaction},
    {EVENT_USE_ITEM, HandlerSnowballUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddSnowball(u32* handleAmount) {
    *handleAmount = ARRAY_COUNT(SnowballHandlers);
    return SnowballHandlers;
}
#endif // EXPAND_ITEMS