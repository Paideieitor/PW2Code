#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// BATTLE BOND
extern "C" void HandlerBattleBond(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    // TODO: ADD GRENINJA
    if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
        pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        !BattleField_CheckBattleBondFlag(pokemonSlot)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u8 i = 0; i < targetCount; ++i) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + i));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon) &&
                BattleMon_GetValue(currentMon, VALUE_FORM) != 1) {
                BattleField_SetBattleBondFlag(pokemonSlot);
#if GEN9_BATTLE_BOND
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_ChangeStatStage* statChangeAttack;
                statChangeAttack = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChangeAttack->pokeCount = 1;
                statChangeAttack->pokeID[0] = pokemonSlot;
                statChangeAttack->stat = STATSTAGE_ATTACK;
                statChangeAttack->volume = 1;
                BattleHandler_PopWork(serverFlow, statChangeAttack);

                HandlerParam_ChangeStatStage* statChangeSpAttack;
                statChangeSpAttack = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChangeSpAttack->pokeCount = 1;
                statChangeSpAttack->pokeID[0] = pokemonSlot;
                statChangeSpAttack->stat = STATSTAGE_SPECIAL_ATTACK;
                statChangeSpAttack->volume = 1;
                BattleHandler_PopWork(serverFlow, statChangeSpAttack);

                HandlerParam_ChangeStatStage* statChangeSpeed;
                statChangeSpeed = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChangeSpeed->pokeCount = 1;
                statChangeSpeed->pokeID[0] = pokemonSlot;
                statChangeSpeed->stat = STATSTAGE_SPEED;
                statChangeSpeed->volume = 1;
                BattleHandler_PopWork(serverFlow, statChangeSpeed);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, BATTLE_BATTLE_BOND_MSGID);
                BattleHandler_AddArg(&message->str, pokemonSlot);
                BattleHandler_PopWork(serverFlow, message);
#else
                HandlerParam_ChangeForm* changeForm;
                changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
                changeForm->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                changeForm->pokeID = pokemonSlot;
                changeForm->newForm = 1;
                BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_BATTLE_BOND_MSGID);
                BattleHandler_PopWork(serverFlow, changeForm);
#endif
            }
        }
    }
}
BattleEventHandlerTableEntry BattleBondHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerBattleBond},
};
extern "C" BattleEventHandlerTableEntry * EventAddBattleBond(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BattleBondHandlers);
    return BattleBondHandlers;
}
#endif // EXPAND_ABILITIES