#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// WELL-BAKED BODY
extern "C" void HandlerWellBakedBody(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (CommonDamageRecoverCheck(serverFlow, pokemonSlot, TYPE_FIRE)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = STATSTAGE_DEFENSE;
        statChange->volume = 2;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, statChange);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
BattleEventHandlerTableEntry WellBakedBodyHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerWellBakedBody},
};
extern "C" BattleEventHandlerTableEntry * EventAddWellBakedBody(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WellBakedBodyHandlers);
    return WellBakedBodyHandlers;
}
#endif // EXPAND_ABILITIES