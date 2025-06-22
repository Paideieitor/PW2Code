#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// STANCE CHANGE
extern "C" void HandlerStanceChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD AEGISLASH
    if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 currentForm = BattleMon_GetValue(currentMon, VALUE_FORM);

        MoveCategory category = PML_MoveGetCategory(moveID);
        HandlerParam_ChangeForm* changeForm;
        if (currentForm == 0 &&
            (category == CATEGORY_PHYSICAL || category == CATEGORY_SPECIAL)) {

            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 1;
            BattleHandler_StrSetup(&changeForm->exStr, 1u, BATTLE_BLADE_FORME_MSGID);
            BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
        // TODO: ADD KING'S SHIELD 
        else if (moveID == MOVE_NULL) {
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 0;
            BattleHandler_StrSetup(&changeForm->exStr, 1u, BATTLE_SHIELD_FORME_MSGID);
            BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
BattleEventHandlerTableEntry StanceChangeHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerStanceChange},
};
extern "C" BattleEventHandlerTableEntry * EventAddStanceChange(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StanceChangeHandlers);
    return StanceChangeHandlers;
}
#endif // EXPAND_ABILITIES