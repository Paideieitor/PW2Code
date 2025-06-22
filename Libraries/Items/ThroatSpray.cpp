#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// THROAT SPRAY
extern "C" void HandlerThroatSpraySoundMoveCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag((MoveID)moveID, FLAG_SOUND)) {

            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (BattleMon_IsStatChangeValid(currentMon, STATSTAGE_SPECIAL_ATTACK, 1)) {
                ItemEvent_PushRun(item, serverFlow, pokemonSlot);
            }
        }
    }
}
extern "C" void HandlerThroatSprayUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_ChangeStatStage* spAttackBoost;
        spAttackBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        spAttackBoost->pokeCount = 1;
        spAttackBoost->pokeID[0] = (u8)pokemonSlot;
        spAttackBoost->stat = STATSTAGE_SPECIAL_ATTACK;
        spAttackBoost->volume = 1;
        BattleHandler_PopWork(serverFlow, spAttackBoost);
    }
}
BattleEventHandlerTableEntry ThroatSprayHandlers[] = {
    {EVENT_MOVE_EXECUTE_END, HandlerThroatSpraySoundMoveCheck},
    {EVENT_USE_ITEM, HandlerThroatSprayUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddThroatSpray(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(ThroatSprayHandlers);
    return ThroatSprayHandlers;
}
#endif // EXPAND_ITEMS