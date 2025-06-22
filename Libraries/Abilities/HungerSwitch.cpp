#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// HUNGER SWITCH
extern "C" void HandlerHungerSwitch(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD MORPEKO
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL) {
            u32 form = BattleMon_GetValue(currentMon, VALUE_FORM);

            if (form == 0) {
                form = 1;
            }
            else {
                form = 0;
            }

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = form;
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry HungerSwitchHandlers[]{
    {EVENT_TURN_CHECK_DONE, HandlerHungerSwitch},
};
extern "C" BattleEventHandlerTableEntry * EventAddHungerSwitch(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HungerSwitchHandlers);
    return HungerSwitchHandlers;
}
#endif // EXPAND_ABILITIES