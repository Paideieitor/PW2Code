#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/Contact.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS SPIKY SHIELD
extern "C" void HandlerPosSpikyShield(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    u32 defendingSlot = Handler_PokePosToPokeID(serverFlow, targetPos);
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (defendingSlot == BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON) && 
            getMoveFlag(moveID, FLAG_CONTACT)) {
        u32 attackingSlot = BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON);

        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);

            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, defendingSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, 8u);
            BattleHandler_StrSetup(&damage->exStr, 2u, 430);
            BattleHandler_AddArg(&damage->exStr, damage->pokeID);
            BattleHandler_PopWork(serverFlow, damage);
        }
    }
}
BattleEventHandlerTableEntry PosSpikyShieldHandlers[]{
    {EVENT_PROTECT_SUCCESS, HandlerPosSpikyShield},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosSpikyShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PosSpikyShieldHandlers);
    return PosSpikyShieldHandlers;
}

// SPIKY SHIELD
extern "C" void HandlerSpikyShield(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerParam_AddPosEffect* addEffectPos;
        addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
        addEffectPos->posEffect = POSEFF_SPIKY_SHIELD;
        addEffectPos->targetPos = Handler_PokeIDToPokePos(serverFlow, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addEffectPos);

        HandlerProtectStart(item, serverFlow, pokemonSlot, work);
    }
}
BattleEventHandlerTableEntry SpikyShieldHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerSpikyShield},
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtectCheckFail},
    {EVENT_MOVE_EXECUTE_FAIL, HandlerProtectResetCounter},
    {EVENT_UNCATEGORIZED_MOVE, HandlerProtect},
};
extern "C" BattleEventHandlerTableEntry* EventAddSpikyShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SpikyShieldHandlers);
    return SpikyShieldHandlers;
}

#endif // EXPAND_MOVES
