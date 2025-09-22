#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// KING'S SHIELD
extern "C" void HandlerKingsShield(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    DPRINTF("KINGS SHIELD - %d \n", pokemonSlot);
    if (IS_NOT_NEW_EVENT)
        return;

    DPRINTF("ATK: %d | DEF: %d \n", BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON), BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON));
    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON)) {
        u32 defendingSlot = BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->pokeCount = 1;
        statChange->pokeID[0] = defendingSlot;
        statChange->stat = STATSTAGE_DEFENSE;
#if !GEN6_KINGS_SHIELD
        statChange->volume = 1;
#else
        statChange->volume = 2;
#endif
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry KingsShieldHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerProtectStart},
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtectCheckFail},
    {EVENT_MOVE_EXECUTE_FAIL, HandlerProtectResetCounter},
    {EVENT_UNCATEGORIZED_MOVE, HandlerProtect},
    {EVENT_PROTECT_SUCCESS, HandlerKingsShield},
};
extern "C" BattleEventHandlerTableEntry* EventAddKingsShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(KingsShieldHandlers);
    return KingsShieldHandlers;
}

#endif // EXPAND_MOVES