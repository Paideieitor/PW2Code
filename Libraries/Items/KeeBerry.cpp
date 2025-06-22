#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// KEE BERRY
extern "C" void HandlerKeeBerryDamageReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_DEFENSE, 1)) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void HandlerKeeBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_ChangeStatStage* defenseBoost;
        defenseBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        defenseBoost->pokeCount = 1;
        defenseBoost->pokeID[0] = (u8)pokemonSlot;
        defenseBoost->stat = STATSTAGE_DEFENSE;
        defenseBoost->volume = 1;
        BattleHandler_PopWork(serverFlow, defenseBoost);
    }
}
BattleEventHandlerTableEntry KeeBerryHandlers[] = {
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerKeeBerryDamageReaction},
    {EVENT_USE_ITEM, HandlerKeeBerryUse},
    {EVENT_USE_ITEM_TEMP, HandlerKeeBerryUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddKeeBerry(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(KeeBerryHandlers);
    return KeeBerryHandlers;
}
#endif // EXPAND_ITEMS