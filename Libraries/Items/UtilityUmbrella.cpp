#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"
#include "server_events.h"

// UTILITY UMBRELLA
extern "C" void HandlerUtilityUmbrellaChange(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    ServerEvent_AfterWeatherChange(serverFlow, ServerEvent_GetWeather(serverFlow));
}
BattleEventHandlerTableEntry UtilityUmbrellaHandlers[] = {
    {EVENT_HELD_ITEM_DECIDE, HandlerUtilityUmbrellaChange},
    {EVENT_ITEM_REWRITE_DONE, HandlerUtilityUmbrellaChange},
};
extern "C" BattleEventHandlerTableEntry* EventAddUtilityUmbrella(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(UtilityUmbrellaHandlers);
    return UtilityUmbrellaHandlers;
}
#endif // EXPAND_ITEMS