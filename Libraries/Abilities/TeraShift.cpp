#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// TERA SHIFT
extern "C" void HandlerTeraShift(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD TERAPAGOS
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
            BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry TeraShiftHandlers[]{
    {EVENT_SWITCH_IN, HandlerTeraShift},
};
extern "C" BattleEventHandlerTableEntry * EventAddTeraShift(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TeraShiftHandlers);
    return TeraShiftHandlers;
}
#endif // EXPAND_ABILITIES