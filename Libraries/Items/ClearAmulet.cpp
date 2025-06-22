#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// CLEAR AMULET
extern "C" void CommonStatDropGuardCheck(ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u32 moveEffect);
extern "C" void HandlerClearAmuletCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    CommonStatDropGuardCheck(serverFlow, pokemonSlot, work, 8);
}
extern "C" void HandlerClearAmuletMessage(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (*work) {
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_CLEARAMULET_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);
    }
    *work = 0;
}
BattleEventHandlerTableEntry ClearAmuletHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerClearAmuletCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerClearAmuletMessage},
};
extern "C" BattleEventHandlerTableEntry* EventAddClearAmulet(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(ClearAmuletHandlers);
    return ClearAmuletHandlers;
}
#endif // EXPAND_ITEMS