#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// STEAM ENGINE
extern "C" void HandlerSteamEngine(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER || BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FIRE)) {

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_SPEED;
        statChange->volume = 6;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry SteamEngineHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSteamEngine},
};
extern "C" BattleEventHandlerTableEntry * EventAddSteamEngine(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SteamEngineHandlers);
    return SteamEngineHandlers;
}
#endif // EXPAND_ABILITIES