#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SWEET VEIL
extern "C" void HandlerSweetVeilPreventSleep(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) ||
        MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_SLEEP) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerSweetVeilFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if ((pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) || MainModule_IsAllyMonID(pokemonSlot, BattleEventVar_GetValue(VAR_DEFENDING_MON))) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_SWEET_VEIL_MSGID);
        BattleHandler_AddArg(&message->str, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
BattleEventHandlerTableEntry SweetVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerSweetVeilPreventSleep},
    {EVENT_ADD_CONDITION_FAIL, HandlerSweetVeilFailMessage},
    // Rest prevention in [HandlerRestCheckFail]
};
extern "C" BattleEventHandlerTableEntry * EventAddSweetVeil(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SweetVeilHandlers);
    return SweetVeilHandlers;
}
#endif // EXPAND_ABILITIES