#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// ZERO TO HERO
extern "C" void HandlerZeroToHero(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_TARGET_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD PALAFIN
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
            BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry ZeroToHeroHandlers[]{
    {EVENT_SWITCH_OUT_INTERRUPT, HandlerZeroToHero},
};
extern "C" BattleEventHandlerTableEntry * EventAddZeroToHero(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ZeroToHeroHandlers);
    return ZeroToHeroHandlers;
}
#endif // EXPAND_ABILITIES