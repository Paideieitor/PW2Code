#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"
#include "server_events.h"

// SHORE UP
extern "C" void HandlerShoreUp(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 ratio = 2048;
        WEATHER weather = Handler_CheckWeather(serverFlow, pokemonSlot, BATTLE_MAX_SLOTS, ServerEvent_GetWeather(serverFlow));
        if (weather == WEATHER_SANDSTORM) {
            ratio = 2732;
        }
        BattleEventVar_RewriteValue(VAR_RATIO, ratio);
    }
}
BattleEventHandlerTableEntry ShoreUpHandlers[]{
    {EVENT_RECOVER_HP, HandlerShoreUp},
};
extern "C" BattleEventHandlerTableEntry* EventAddShoreUp(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ShoreUpHandlers);
    return ShoreUpHandlers;
}

#endif // EXPAND_MOVES
