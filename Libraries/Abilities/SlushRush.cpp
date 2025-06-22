#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"
#include "server_events.h"

// SLUSH RUSH
extern "C" void HandlerSlushRush(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (ServerEvent_GetWeather(serverFlow) == WEATHER_HAIL) {
            BattleEventVar_MulValue(VAR_RATIO, 8192);
        }
    }
}
BattleEventHandlerTableEntry SlushRushHandlers[]{
    {EVENT_CALC_SPEED, HandlerSlushRush},
};
extern "C" BattleEventHandlerTableEntry * EventAddSlushRush(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SlushRushHandlers);
    return SlushRushHandlers;
}
#endif // EXPAND_ABILITIES