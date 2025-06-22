#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// PASTEL VEIL
extern "C" void HandlerPastelVeilPreventPoison(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerPastelVeilFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, 210u);
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
extern "C" void HandlerPastelVeilCurePoisonAlly(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot == currentSlot) {
        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedSlot = BattleMon_GetID(affectedMon);
                if (MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS &&
                        !BattleMon_IsFainted(affectedMon)) {

                        if (BattleMon_CheckIfMoveCondition(affectedMon, CONDITION_POISON)) {
                            HandlerParam_CureCondition* cureCondition;
                            cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
                            cureCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                            cureCondition->condition = CONDITION_POISON;
                            cureCondition->pokeID[0] = affectedSlot;
                            cureCondition->pokeCount = 1;
                            BattleHandler_PopWork(serverFlow, cureCondition);
                        }
                    }
                }
            }
        }
    }
}
extern "C" void HandlerPastelVeilCurePoisonSelf(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_POISON)) {
        HandlerParam_CureCondition* cureCondition;
        cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
        cureCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        cureCondition->condition = CONDITION_POISON;
        cureCondition->pokeID[0] = pokemonSlot;
        cureCondition->pokeCount = 1;
        BattleHandler_PopWork(serverFlow, cureCondition);
    }
}
BattleEventHandlerTableEntry PastelVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerPastelVeilPreventPoison},
    {EVENT_ADD_CONDITION_FAIL, HandlerPastelVeilFailMessage},
    {EVENT_SWITCH_IN, HandlerPastelVeilCurePoisonAlly},
    {EVENT_ACTION_PROCESSING_END, HandlerPastelVeilCurePoisonSelf},
};
extern "C" BattleEventHandlerTableEntry * EventAddPastelVeil(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PastelVeilHandlers);
    return PastelVeilHandlers;
}
#endif // EXPAND_ABILITIES