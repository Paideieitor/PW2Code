#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS KING'S SHIELD
extern "C" void HandlerPosKingsShield(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    u32 pokemonSlot = Handler_PokePosToPokeID(serverFlow, targetPos);
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON) && 
        PML_MoveGetCategory(moveID) == CATEGORY_PHYSICAL) {
        u32 attackingSlot = BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->pokeCount = 1;
        statChange->pokeID[0] = attackingSlot;
        statChange->stat = STATSTAGE_DEFENSE;
#if !GEN6_KINGS_SHIELD
        statChange->volume = -1;
#else
        statChange->volume = -2;
#endif
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry PosKingsShieldHandlers[]{
    {EVENT_PROTECT_SUCCESS, HandlerPosKingsShield},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosKingsShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PosKingsShieldHandlers);
    return PosKingsShieldHandlers;
}

// KING'S SHIELD
extern "C" void HandlerKingsShield(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        HandlerParam_AddPosEffect* addEffectPos;
        addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
        addEffectPos->posEffect = POSEFF_KINGS_SHIELD;
        addEffectPos->targetPos = Handler_PokeIDToPokePos(serverFlow, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addEffectPos);

        HandlerProtectStart(item, serverFlow, pokemonSlot, work);
    }
}
BattleEventHandlerTableEntry KingsShieldHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerKingsShield},
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtectCheckFail},
    {EVENT_MOVE_EXECUTE_FAIL, HandlerProtectResetCounter},
    {EVENT_UNCATEGORIZED_MOVE, HandlerProtect},
    
};
extern "C" BattleEventHandlerTableEntry* EventAddKingsShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(KingsShieldHandlers);
    return KingsShieldHandlers;
}

#endif // EXPAND_MOVES