#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SHIELDS DOWN
extern "C" void HandlerShieldsDown(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // TODO: ADD MINIOR 
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL) {
            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);

            u8 form = 1;
            u16 msgID = BATTLE_SHIELDS_DOWN_INACTIVE_MSGID;
            if (currentHP <= DivideMaxHPZeroCheck(currentMon, 2u)) {
                form = 0;
                msgID = BATTLE_SHIELDS_DOWN_ACTIVE_MSGID;
            }

            if (form != BattleMon_GetValue(currentMon, VALUE_FORM)) {
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = form;
                BattleHandler_StrSetup(&changeForm->exStr, 1u, msgID);
                BattleHandler_PopWork(serverFlow, changeForm);
            }
        }
    }
}
BattleEventHandlerTableEntry ShieldsDownHandlers[]{
    {EVENT_TURN_CHECK_DONE, HandlerShieldsDown},
};
extern "C" BattleEventHandlerTableEntry * EventAddShieldsDown(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ShieldsDownHandlers);
    return ShieldsDownHandlers;
}
#endif // EXPAND_ABILITIES