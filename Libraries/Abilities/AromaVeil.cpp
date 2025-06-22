#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// AROMA VEIL
extern "C" void HandlerAromaVeilPreventConditions(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) ||
        MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_HEALBLOCK ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_DISABLE ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_ATTRACT ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_TAUNT ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_ENCORE ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_TORMENT) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerAromaVeilFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if ((pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) || MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_AROMA_VEIL_MSGID);
        BattleHandler_AddArg(&message->str, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        BattleHandler_PopWork(serverFlow, message);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
BattleEventHandlerTableEntry AromaVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerAromaVeilPreventConditions},
    {EVENT_ADD_CONDITION_FAIL, HandlerAromaVeilFailMessage},
};
extern "C" BattleEventHandlerTableEntry * EventAddAromaVeil(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(AromaVeilHandlers);
    return AromaVeilHandlers;
}
#endif // EXPAND_ABILITIES