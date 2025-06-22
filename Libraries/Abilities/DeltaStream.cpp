#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"
#include "server_events.h"

// DELTA STREAM
extern "C" void HandlerDeltaStreamStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAddExtremeWeather(serverFlow, pokemonSlot, WEATHER_STRONGWINDS);
    }
}
extern "C" void HandlerDeltaStreamEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_STRONGWINDS);
    }
}
extern "C" void HandlerDeltaStreamEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_STRONGWINDS);
    }
}
extern "C" void HandlerDeltaStreamResistance(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetWeather(serverFlow) == WEATHER_STRONGWINDS && BattleField_GetExtremeWeatherMons()) {
        if (BattleEventVar_GetValue(VAR_POKE_TYPE) == TYPE_FLY &&
            GetTypeEffectiveness(BattleEventVar_GetValue(VAR_MOVE_TYPE), TYPE_FLY) == EFFECTIVENESS_2) {

            BattleEventVar_RewriteValue(VAR_SET_TYPE_EFFECTIVENESS, EFFECTIVENESS_1);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 1u, BATTLE_DELTA_STREAM_RESISTANCE_MSGID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_PopWork(serverFlow, message);
        }
    }
}
BattleEventHandlerTableEntry DeltaStreamHandlers[]{
    {EVENT_SWITCH_IN, HandlerDeltaStreamStart},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDeltaStreamStart},
    {EVENT_SWITCH_OUT_END, HandlerDeltaStreamEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerDeltaStreamEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerDeltaStreamEnd},
    {EVENT_NOTIFY_FAINTED, HandlerDeltaStreamEndFainted},
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerDeltaStreamResistance},
};
extern "C" BattleEventHandlerTableEntry * EventAddDeltaStream(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DeltaStreamHandlers);
    return DeltaStreamHandlers;
}
#endif // EXPAND_ABILITIES