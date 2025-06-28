#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// RATTLED
extern "C" void HandlerRattled(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerRattledIntimidateCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && BattleEventVar_GetValue(VAR_INTIMIDATE_FLAG)) {
        work[0] = 1;
    }
}
extern "C" void HandlerRattledIntimidate(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && work[0]) {

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_SPEED;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry RattledHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerRattled},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerRattledIntimidateCheck},
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerRattledIntimidate},
};
extern "C" BattleEventHandlerTableEntry * EventAddRattledUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RattledHandlers);
    return RattledHandlers;
}
#endif // EXPAND_ABILITIES