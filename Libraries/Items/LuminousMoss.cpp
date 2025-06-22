#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// LUMINOUS MOSS
extern "C" void HandlerLuminousMossDamageReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_SPECIAL_DEFENSE, 1))
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerLuminousMossUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_ChangeStatStage* spDefenseBoost;
        spDefenseBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spDefenseBoost->pokeCount = 1;
        spDefenseBoost->pokeID[0] = (u8)pokemonSlot;
        spDefenseBoost->stat = STATSTAGE_SPECIAL_DEFENSE;
        spDefenseBoost->volume = 1;
        BattleHandler_PopWork(serverFlow, spDefenseBoost);
    }
}
BattleEventHandlerTableEntry LuminousMossHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerLuminousMossDamageReaction},
    {EVENT_USE_ITEM, HandlerLuminousMossUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddLuminousMoss(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(LuminousMossHandlers);
    return LuminousMossHandlers;
}
#endif // EXPAND_ITEMS