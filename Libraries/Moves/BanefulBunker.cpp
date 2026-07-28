#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/Contact.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS BANEFUL BUNKER
extern "C" void HandlerPosBanefulBunker(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    u32 defendingSlot = Handler_PokePosToPokeID(serverFlow, targetPos);
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (defendingSlot == BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON) && 
            getMoveFlag(moveID, FLAG_CONTACT)) {
        u32 attackingSlot = BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON);

        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {

            HandlerParam_AddCondition* addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, defendingSlot);
            addCondition->pokeID = attackingSlot;
            addCondition->condition = CONDITION_POISON;
            addCondition->condData = MakeBasicStatus(CONDITION_POISON);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
BattleEventHandlerTableEntry PosBanefulBunkerHandlers[]{
    {EVENT_PROTECT_SUCCESS, HandlerPosBanefulBunker},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosBanefulBunker(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PosBanefulBunkerHandlers);
    return PosBanefulBunkerHandlers;
}

// BANEFUL BUNKER
extern "C" void HandlerBanefulBunker(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerParam_AddPosEffect* addEffectPos;
        addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
        addEffectPos->posEffect = POSEFF_BANEFUL_BUNKER;
        addEffectPos->targetPos = Handler_PokeIDToPokePos(serverFlow, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addEffectPos);

        HandlerProtectStart(item, serverFlow, pokemonSlot, work);
    }
}
BattleEventHandlerTableEntry BanefulBunkerHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerBanefulBunker},
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtectCheckFail},
    {EVENT_MOVE_EXECUTE_FAIL, HandlerProtectResetCounter},
    {EVENT_UNCATEGORIZED_MOVE, HandlerProtect},
};
extern "C" BattleEventHandlerTableEntry* EventAddBanefulBunker(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BanefulBunkerHandlers);
    return BanefulBunkerHandlers;
}

#endif // EXPAND_MOVES
