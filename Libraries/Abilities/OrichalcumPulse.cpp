#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"
#include "server_events.h"

// ORICHALCUM PULSE
extern "C" void HandlerOrichalcumPulseSun(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeWeather* changeWeather;
        changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
        changeWeather->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        changeWeather->weather = WEATHER_SUN;
        changeWeather->turn = WEATHER_ABILITY_TURNS;
        BattleHandler_PopWork(serverFlow, changeWeather);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_ORICHALCUM_PULSE_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerOrichalcumPulseAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL &&
        ServerEvent_GetWeather(serverFlow) == WEATHER_SUN) {

        BattleEventVar_MulValue(VAR_RATIO, 5461);
    }
}
BattleEventHandlerTableEntry OrichalcumPulseHandlers[]{
    {EVENT_SWITCH_IN, HandlerOrichalcumPulseSun},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerOrichalcumPulseSun},
    {EVENT_ATTACKER_POWER, HandlerOrichalcumPulseAttack},
};
extern "C" BattleEventHandlerTableEntry * EventAddOrichalcumPulse(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(OrichalcumPulseHandlers);
    return OrichalcumPulseHandlers;
}
#endif // EXPAND_ABILITIES