#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// COMPETITIVE
extern "C" void HandlerCompetitive(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot) && BattleEventVar_GetValue(VAR_VOLUME) < 0) {
            HandlerParam_ChangeStatStage* statStageChange;
            statStageChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statStageChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            statStageChange->stat = STATSTAGE_SPECIAL_ATTACK;
            statStageChange->volume = 2;
            statStageChange->moveAnimation = 1;
            statStageChange->pokeCount = 1;
            statStageChange->pokeID[0] = pokemonSlot;
            BattleHandler_PopWork(serverFlow, statStageChange);
        }
    }
}
BattleEventHandlerTableEntry CompetitiveHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerCompetitive},
};
extern "C" BattleEventHandlerTableEntry * EventAddCompetitive(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CompetitiveHandlers);
    return CompetitiveHandlers;
}
#endif // EXPAND_ABILITIES