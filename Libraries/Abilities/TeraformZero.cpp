#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// TERAFORM ZERO
extern "C" void HandlerTeraformZero(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_ChangeWeather* changeWeather;
        changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
        changeWeather->weather = WEATHER_NULL;
        BattleHandler_PopWork(serverFlow, changeWeather);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = FLDEFF_TERRAIN;
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
BattleEventHandlerTableEntry TeraformZeroHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerTeraformZero},
};
extern "C" BattleEventHandlerTableEntry * EventAddTeraformZero(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TeraformZeroHandlers);
    return TeraformZeroHandlers;
}
#endif // EXPAND_ABILITIES