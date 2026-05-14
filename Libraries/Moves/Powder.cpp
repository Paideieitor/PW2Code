#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/Contact.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS POWDER
extern "C" void HandlerPosPowder(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {

    POKE_TYPE type = BattleEventVar_GetValue(VAR_MOVE_TYPE);
    if (type == TYPE_FIRE) {
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        BattleMon* attackingMon = nullptr;

        DPRINTF("FIND -> SLOT: %d \n", attackingSlot);
        for (u32 idx = 0; idx < targetPos; ++idx) {
            DPRINTF("  TARGET: %d \n", work[idx]);
            if (work[idx] == attackingSlot) {
                DPRINT("  FOUND \n");
                attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
                break;
            }
        }
        if (!attackingMon) {
            return;
        }

        HandlerParam_Damage* damage;
        damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, attackingSlot);
        damage->pokeID = attackingSlot;
        damage->damage = DivideMaxHPZeroCheck(attackingMon, 4u);
        BattleHandler_StrSetup(&damage->exStr, 1u, BATTLE_POWDER_EXPLODE_MSGID);
        BattleHandler_AddArg(&damage->exStr, damage->pokeID);
        BattleHandler_PopWork(serverFlow, damage);

        BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
    }
}
BattleEventHandlerTableEntry PosPowderHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPosPowder},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosPowder(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PosPowderHandlers);
    return PosPowderHandlers;
}

// POWDER
extern "C" void HandlerPowder(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerParam_AddPosEffect* addEffectPos;
        addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
        addEffectPos->posEffect = POSEFF_POWDER;

        DPRINTF("ADD -> SLOT: %d \n", pokemonSlot);
        // Store all targets in the event work
        addEffectPos->workCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 idx = 0; idx < addEffectPos->workCount; ++idx) {
            addEffectPos->workToCopy[idx] = BattleEventVar_GetValue((BattleEventVar)((u32)VAR_TARGET_MON_ID + idx));
            DPRINTF("  TARGET: %d \n", addEffectPos->workToCopy[idx]);
        }
        // Easy access to the work count to events
        addEffectPos->targetPos = addEffectPos->workCount;

        BattleHandler_PopWork(serverFlow, addEffectPos);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_POWDER_MSGID);
        BattleHandler_AddArg(&message->str, work[0]);
        BattleHandler_PopWork(serverFlow, message);
    }
}
BattleEventHandlerTableEntry PowderHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerPowder},
};
extern "C" BattleEventHandlerTableEntry* EventAddPowder(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PowderHandlers);
    return PowderHandlers;
}

#endif // EXPAND_MOVES
