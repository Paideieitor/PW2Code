#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SCHOOLING
extern "C" void HandlerSchooling(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD WISHIWASHI
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
            currentMon->level >= 20) {
            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);

            u8 form = 0;
            u16 msgID = BATTLE_SCHOOLING_INACTIVE_MSGID;
            if (currentHP > DivideMaxHPZeroCheck(currentMon, 4u)) {
                form = 1;
                msgID = BATTLE_SCHOOLING_ACTIVE_MSGID;
            }

            if (form != BattleMon_GetValue(currentMon, VALUE_FORM)) {
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = form;
                BattleHandler_StrSetup(&changeForm->exStr, 2u, msgID);
                BattleHandler_PopWork(serverFlow, changeForm);
            }
        }
    }
}
BattleEventHandlerTableEntry SchoolingHandlers[]{
    {EVENT_SWITCH_IN, HandlerSchooling},
    {EVENT_TURN_CHECK_DONE, HandlerSchooling},
};
extern "C" BattleEventHandlerTableEntry * EventAddSchooling(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SchoolingHandlers);
    return SchoolingHandlers;
}
#endif // EXPAND_ABILITIES