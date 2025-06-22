#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// POWER CONSTRUCT
extern "C" void HandlerPowerConstruct(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD ZYGARDE
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
            BattleMon_GetValue(currentMon, VALUE_FORM) != 2) {
            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);

            if (currentHP <= DivideMaxHPZeroCheck(currentMon, 2u)) {
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = 2;
                BattleHandler_StrSetup(&changeForm->exStr, 1u, BATTLE_POWER_CONSTRUCT_MSGID);
                BattleHandler_PopWork(serverFlow, changeForm);
            }
        }
    }
}
BattleEventHandlerTableEntry PowerConstructHandlers[]{
    {EVENT_TURN_CHECK_DONE, HandlerPowerConstruct},
};
extern "C" BattleEventHandlerTableEntry * EventAddPowerConstruct(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(PowerConstructHandlers);
    return PowerConstructHandlers;
}
#endif // EXPAND_ABILITIES