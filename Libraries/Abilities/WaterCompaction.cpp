#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// WATER COMPATION
extern "C" void HandlerWaterCompaction(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_DEFENSE, 2)) {
            if (!BattleMon_IsFainted(defendingMon)) {
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->moveAnimation = 1;
                statChange->stat = STATSTAGE_DEFENSE;
                statChange->volume = 2;
                BattleHandler_PopWork(serverFlow, statChange);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry WaterCompactionHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWaterCompaction},
};
extern "C" BattleEventHandlerTableEntry * EventAddWaterCompaction(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WaterCompactionHandlers);
    return WaterCompactionHandlers;
}
#endif // EXPAND_ABILITIES