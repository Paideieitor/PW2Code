#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// GUARD DOG
extern "C" void HandlerGuardDog(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && BattleEventVar_GetValue(VAR_INTIMIDATE_FLAG)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = STATSTAGE_ATTACK;
        statChange->volume = 1;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, statChange);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerSuctionCups(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry GuardDogHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerGuardDog},
    {EVENT_CHECK_FORCE_SWITCH, HandlerSuctionCups},
};
extern "C" BattleEventHandlerTableEntry * EventAddGuardDog(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GuardDogHandlers);
    return GuardDogHandlers;
}
#endif // EXPAND_ABILITIES