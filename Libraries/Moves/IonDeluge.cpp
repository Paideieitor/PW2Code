#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// POS ION DELUGE
extern "C" void HandlerPosIonDeluge(BattleEventItem* item, ServerFlow* serverFlow, u32 targetPos, u32* work) {
    if (IS_NOT_NEW_EVENT) { // Safety layer to make sure it's not used if a new event uses the same event ID
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_ELEC);
        }
    }
}
BattleEventHandlerTableEntry PosIonDelugeHandlers[]{
    {EVENT_MOVE_PARAM, HandlerPosIonDeluge},
    {EVENT_TURN_CHECK_DONE, HandlerPosTurnCheckDone},
};
extern "C" BattleEventHandlerTableEntry* EventAddPosIonDeluge(u32* handlerAmount) {
    *handlerAmount = PosEffect_SetPriority(EVENTPRI_ABILITY_STALL, ARRAY_COUNT(PosIonDelugeHandlers));
    return PosIonDelugeHandlers;
}

// ION DELUGE
extern "C" void HandlerIonDeluge(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerParam_AddPosEffect* addEffectPos;
        addEffectPos = (HandlerParam_AddPosEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_POS_EFFECT, pokemonSlot);
        addEffectPos->posEffect = POSEFF_ION_DELUGE;
        addEffectPos->targetPos = 6;
        BattleHandler_PopWork(serverFlow, addEffectPos);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, BATTLE_ION_DELUGE_MSGID);
        BattleHandler_PopWork(serverFlow, message);
    }
}
BattleEventHandlerTableEntry IonDelugeHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerIonDeluge},
};
extern "C" BattleEventHandlerTableEntry* EventAddIonDeluge(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(IonDelugeHandlers);
    return IonDelugeHandlers;
}

// PLASMA FIST
BattleEventHandlerTableEntry PlasmaFistHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerIonDeluge},
};
extern "C" BattleEventHandlerTableEntry* EventAddPlasmaFist(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PlasmaFistHandlers);
    return PlasmaFistHandlers;
}

#endif // EXPAND_MOVES