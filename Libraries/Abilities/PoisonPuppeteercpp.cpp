#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// POISON PUPPETEER
extern "C" void HandlerPoisonPuppeteer(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON) {
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);

        HandlerParam_AddCondition* addCondition;
        addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
        addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        addCondition->condition = CONDITION_CONFUSION;
        addCondition->pokeID = defendingSlot;
        MakeCondition(addCondition->condition, Handler_GetBattleMon(serverFlow, addCondition->pokeID), &addCondition->condData);
        BattleHandler_PopWork(serverFlow, addCondition);
    }
}
BattleEventHandlerTableEntry PoisonPuppeteerHandlers[]{
    {EVENT_ADD_BASIC_STATUS, HandlerPoisonPuppeteer},
};
extern "C" BattleEventHandlerTableEntry * EventAddPoisonPuppeteer(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PoisonPuppeteerHandlers);
    return PoisonPuppeteerHandlers;
}
#endif // EXPAND_ABILITIES