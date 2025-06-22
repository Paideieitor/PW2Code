#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// WEAKNESS POLICY
extern "C" void HandlerWeaknessPolicyDamageReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    TypeEffectiveness typeEffectiveness = (TypeEffectiveness)BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS);
    
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && typeEffectiveness >= EFFECTIVENESS_2) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_ATTACK, 1) ||
            BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_SPECIAL_ATTACK, 1)) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void HandlerWeaknessPolicyUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {

        HandlerParam_ChangeStatStage* attackBoost;
        attackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        attackBoost->pokeCount = 1;
        attackBoost->pokeID[0] = (u8)pokemonSlot;
        attackBoost->stat = STATSTAGE_ATTACK;
        attackBoost->volume = 2;
        BattleHandler_PopWork(serverFlow, attackBoost);

        HandlerParam_ChangeStatStage* spAttackBoost;
        spAttackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spAttackBoost->pokeCount = 1;
        spAttackBoost->pokeID[0] = (u8)pokemonSlot;
        spAttackBoost->stat = STATSTAGE_SPECIAL_ATTACK;
        spAttackBoost->volume = 2;
        BattleHandler_PopWork(serverFlow, spAttackBoost);
    }
}
BattleEventHandlerTableEntry WeaknessPolicyHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWeaknessPolicyDamageReaction},
    {EVENT_USE_ITEM, HandlerWeaknessPolicyUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddWeaknessPolicy(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(WeaknessPolicyHandlers);
    return WeaknessPolicyHandlers;
}
#endif // EXPAND_ITEMS