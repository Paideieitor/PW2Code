#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"
#include "server_events.h"

// ICE FACE
extern "C" void IceFaceRevertForm(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD EISCUE
    if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
        BattleMon_GetValue(currentMon, VALUE_FORM) == 1) {

        if (ServerEvent_GetWeather(serverFlow) == WEATHER_HAIL) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 0;
            BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_DISGUISE_MSGID);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
extern "C" void HandlerIceFacePreventDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD EISCUE
    if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {
        if (BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            // General Flag set to 2 [ServerControl_ApplyDamageToRecover]
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 2);

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_DISGUISE_MSGID);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
extern "C" void HandlerIceFaceHailCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    IceFaceRevertForm(serverFlow, pokemonSlot);
}
extern "C" void HandlerIceFaceSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        IceFaceRevertForm(serverFlow, pokemonSlot);
    }
}
BattleEventHandlerTableEntry IceFaceHandlers[]{
    {EVENT_CHECK_DAMAGE_TO_RECOVER, HandlerIceFacePreventDamage},
    {EVENT_AFTER_WEATHER_CHANGE, HandlerIceFaceHailCheck},
    {EVENT_SWITCH_IN, HandlerIceFaceSwitchIn},
};
extern "C" BattleEventHandlerTableEntry * EventAddIceFace(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(IceFaceHandlers);
    return IceFaceHandlers;
}
#endif // EXPAND_ABILITIES