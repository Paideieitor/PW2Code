#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// EJECT PACK
// work[0] -> stores if the Pokémon has had its stats dropped
extern "C" void HandlerEjectPackStatCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleEventVar_GetValue(VAR_VOLUME) < 0) {
        work[0] = 1;
    }
}
extern "C" void HandlerEjectPackActionEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        work[0] == 1) {
        work[0] = 0;
        if (Handler_GetFightEnableBenchPokeNum(serverFlow, pokemonSlot)
            && Handler_CheckReservedMemberChangeAction(serverFlow)) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void HandlerEjectPackUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_Switch* switchOut;
        switchOut = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
        switchOut->pokeID = pokemonSlot;
        BattleHandler_PopWork(serverFlow, switchOut);
    }
}
BattleEventHandlerTableEntry EjectPackHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerEjectPackStatCheck},
    {EVENT_ACTION_PROCESSING_END, HandlerEjectPackActionEnd},
    {EVENT_USE_ITEM, HandlerEjectPackUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddEjectPack(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(EjectPackHandlers);
    return EjectPackHandlers;
}
#endif // EXPAND_ITEMS