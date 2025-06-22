#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SAND SPIT
extern "C" void HandlerSandSpit(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        HandlerParam_ChangeWeather* changeWeather;
        changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
        changeWeather->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        changeWeather->weather = WEATHER_SANDSTORM;
        changeWeather->turn = WEATHER_ABILITY_TURNS;
        BattleHandler_PopWork(serverFlow, changeWeather);
    }
}
BattleEventHandlerTableEntry SandSpitHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSandSpit},
};
extern "C" BattleEventHandlerTableEntry * EventAddSandSpit(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SandSpitHandlers);
    return SandSpitHandlers;
}
#endif // EXPAND_ABILITIES