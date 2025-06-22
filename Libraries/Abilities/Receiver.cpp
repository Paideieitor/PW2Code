#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

#include "abilities.h"

// RECEIVER
extern "C" void HandlerReceiver(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot != currentSlot &&
        MainModule_IsAllyMonID(pokemonSlot, currentSlot) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, currentSlot))) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        ABILITY ability = BattleMon_GetValue(currentMon, VALUE_ABILITY);
        if (!j_j_IsRolePlayFailAbility(ability)) {
            HandlerParam_ChangeAbility* changeAbility;
            changeAbility = (HandlerParam_ChangeAbility*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_ABILITY, pokemonSlot);
            changeAbility->pokeID = pokemonSlot;
            changeAbility->ability = ability;
            BattleHandler_StrSetup(&changeAbility->exStr, 2u, 619);
            BattleHandler_AddArg(&changeAbility->exStr, pokemonSlot);
            BattleHandler_AddArg(&changeAbility->exStr, currentSlot);
            BattleHandler_AddArg(&changeAbility->exStr, changeAbility->ability);
            BattleHandler_PopWork(serverFlow, changeAbility);
        }
    }
}
BattleEventHandlerTableEntry ReceiverHandlers[]{
    {EVENT_NOTIFY_FAINTED, HandlerReceiver},
};
extern "C" BattleEventHandlerTableEntry * EventAddReceiver(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ReceiverHandlers);
    return ReceiverHandlers;
}
#endif // EXPAND_ABILITIES