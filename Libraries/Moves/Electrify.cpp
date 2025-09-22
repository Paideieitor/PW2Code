#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS ELECTRIFY
extern "C" void HandlerPosElectrify(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {
    u32 pokemonSlot = Handler_PokePosToPokeID(serverFlow, targetPos);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_ELEC);
    }
}
BattleEventHandlerTableEntry PosElectrifyHandlers[]{
    {EVENT_MOVE_PARAM, HandlerPosElectrify},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosElectrify(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PosElectrifyHandlers);
    return PosElectrifyHandlers;
}

// ELECTRIFY
extern "C" void HandlerElectrify(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_TARGET_COUNT) > 0) {

        u32 targetSlot = BattleEventVar_GetValue(VAR_TARGET_MON_ID);
        BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
        if (!BattleMon_GetTurnFlag(targetMon, TURNFLAG_MOVEPROCDONE)) {

            HandlerParam_AddPosEffect* addEffectPos;
            addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
            addEffectPos->posEffect = POSEFF_ELECTRIFY;
            addEffectPos->targetPos = Handler_PokeIDToPokePos(serverFlow, targetSlot);
            BattleHandler_PopWork(serverFlow, addEffectPos);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_ELECTRIFY_MSGID);
            BattleHandler_AddArg(&message->str, targetSlot);
            BattleHandler_PopWork(serverFlow, message);
        }
    }
}
BattleEventHandlerTableEntry ElectrifyHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerElectrify},
};
extern "C" BattleEventHandlerTableEntry* EventAddElectrify(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ElectrifyHandlers);
    return ElectrifyHandlers;
}

#endif // EXPAND_MOVES