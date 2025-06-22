#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// QUICK DRAW
extern "C" void HandlerQuickDraw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (AbilityEvent_RollEffectChance(serverFlow, 30) &&
            BattleEventVar_RewriteValue(VAR_PRIORITY, 2)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_QUICK_DRAW_MSGID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_AddArg(&message->str, item->subID);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry QuickDrawHandlers[]{
    {EVENT_CHECK_SPECIAL_PRIORITY, HandlerQuickDraw},
};
extern "C" BattleEventHandlerTableEntry * EventAddQuickDraw(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(QuickDrawHandlers);
    return QuickDrawHandlers;
}
#endif // EXPAND_ABILITIES