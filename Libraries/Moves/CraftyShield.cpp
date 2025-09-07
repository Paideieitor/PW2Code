#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS CRAFTY SHIELD
extern "C" void HandlerPosCraftyShield(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {
    u32 pokemonSlot = Handler_PokePosToPokeID(serverFlow, targetPos);
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (defendingSlot != attackingSlot &&
        !MainModule_IsAllyMonID(attackingSlot, defendingSlot) &&
        MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_STATUS) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
            HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_CRAFTY_SHIELD_EFFECT_MSGID);
            BattleHandler_AddArg(&message->str, defendingSlot);
            BattleHandler_PopWork(serverFlow, message);
        }
    }
}
BattleEventHandlerTableEntry PosCraftyShieldHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPosCraftyShield},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
    {EVENT_PROTECT_BROKEN, HandlerPosProtectBroken},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosCraftyShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PosCraftyShieldHandlers);
    return PosCraftyShieldHandlers;
}

// CRAFTY SHIELD
extern "C" void HandlerCraftyShield(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerParam_AddPosEffect* addEffectPos;
        addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
        addEffectPos->posEffect = POSEFF_CRAFTY_SHIELD;
        addEffectPos->targetPos = pokemonSlot;
        BattleHandler_PopWork(serverFlow, addEffectPos);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, BATTLE_CRAFTY_SHIELD_USE_MSGID + GetSideFromMonID(pokemonSlot));
        BattleHandler_PopWork(serverFlow, message);
    }
}
BattleEventHandlerTableEntry CraftyShieldHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerCraftyShield},
};
extern "C" BattleEventHandlerTableEntry* EventAddCraftyShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CraftyShieldHandlers);
    return CraftyShieldHandlers;
}

#endif // EXPAND_MOVES