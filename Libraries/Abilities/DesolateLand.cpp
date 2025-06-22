#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// DESOLATE LAND
extern "C" void HandlerDesolateLandStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAddExtremeWeather(serverFlow, pokemonSlot, WEATHER_SUN);
    }
}
extern "C" void HandlerDesolateLandEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_SUN);
    }
}
extern "C" void HandlerDesolateLandEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_SUN);
    }
}
extern "C" void HandlerDesolateLandImmunity(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    HandlerExtremeWeatherImmunity(serverFlow, pokemonSlot, WEATHER_SUN, TYPE_WATER, BATTLE_DESOLATE_LAND_IMMUNE_MSGID);
}
BattleEventHandlerTableEntry DesolateLandHandlers[]{
    {EVENT_SWITCH_IN, HandlerDesolateLandStart},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDesolateLandStart},
    {EVENT_SWITCH_OUT_END, HandlerDesolateLandEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerDesolateLandEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerDesolateLandEnd},
    {EVENT_NOTIFY_FAINTED, HandlerDesolateLandEndFainted},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerDesolateLandImmunity},
};
extern "C" BattleEventHandlerTableEntry * EventAddDesolateLand(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DesolateLandHandlers);
    return DesolateLandHandlers;
}
#endif // EXPAND_ABILITIES