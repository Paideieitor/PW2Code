#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// DISGUISE
extern "C" void HandlerDisguisePreventDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD MIMIKYU
    if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
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
#if GEN7_DISGUISE
            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = pokemonSlot;
            damage->damage = DivideMaxHPZeroCheck(currentMon, 8u);
            BattleHandler_PopWork(serverFlow, damage);
#endif
        }
    }
}
BattleEventHandlerTableEntry DisguiseHandlers[]{
    {EVENT_CHECK_DAMAGE_TO_RECOVER, HandlerDisguisePreventDamage},
};
extern "C" BattleEventHandlerTableEntry * EventAddDisguise(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DisguiseHandlers);
    return DisguiseHandlers;
}
#endif // EXPAND_ABILITIES