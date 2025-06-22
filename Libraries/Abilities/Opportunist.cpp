#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// OPPORTUNIST
// work[0] -> flag to apply the stat change
extern "C" void HandlerOpportunistCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    int volume = BattleEventVar_GetValue(VAR_VOLUME);
    if (!MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_MON_ID))
        && volume > 0
        && !BattleEventVar_GetValue(VAR_OPPORTUNIST_FLAG)) {

        work[0] = 1;
    }
}
extern "C" void HandlerOpportunist(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[0]) {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = (StatStage)BattleEventVar_GetValue(VAR_MOVE_EFFECT);
        statChange->volume = BattleEventVar_GetValue(VAR_VOLUME);
        statChange->pad = STAT_CHANGE_OPPORTUNIST_FLAG;
        BattleHandler_PopWork(serverFlow, statChange);

        work[0] = 0;
    }

}
BattleEventHandlerTableEntry OpportunistHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerOpportunistCheck},
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerOpportunist},
};
extern "C" BattleEventHandlerTableEntry * EventAddOpportunist(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(OpportunistHandlers);
    return OpportunistHandlers;
}
#endif // EXPAND_ABILITIES