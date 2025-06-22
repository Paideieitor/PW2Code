#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// PRIMORDIAL SEA
extern "C" void HandlerPrimordialSeaStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAddExtremeWeather(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}
extern "C" void HandlerPrimordialSeaEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}
extern "C" void HandlerPrimordialSeaEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        HandlerRemoveExtremeWeather(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}
extern "C" void HandlerPrimordialSeaImmunity(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    HandlerExtremeWeatherImmunity(serverFlow, pokemonSlot, WEATHER_RAIN, TYPE_FIRE, BATTLE_PRIMORDIAL_SEA_IMMUNE_MSGID);
}
BattleEventHandlerTableEntry PrimordialSeaHandlers[]{
    {EVENT_SWITCH_IN, HandlerPrimordialSeaStart},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerPrimordialSeaStart},
    {EVENT_SWITCH_OUT_END, HandlerPrimordialSeaEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerPrimordialSeaEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerPrimordialSeaEnd},
    {EVENT_NOTIFY_FAINTED, HandlerPrimordialSeaEndFainted},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPrimordialSeaImmunity},
};
extern "C" BattleEventHandlerTableEntry * EventAddPrimordialSea(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PrimordialSeaHandlers);
    return PrimordialSeaHandlers;
}
#endif // EXPAND_ABILITIES