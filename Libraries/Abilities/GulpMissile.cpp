#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// GULP MISSILE
extern "C" void HandlerGulpMissileCatch(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == currentSlot &&
        (moveID == MOVE_SURF || moveID == MOVE_DIVE)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        // TODO: ADD CRAMORANT
        if (BattleMon_GetSpecies(currentMon) == PKM_NULL &&
            BattleMon_GetValue(currentMon, VALUE_FORM) == 0) {

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;

            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
            if (currentHP >= DivideMaxHPZeroCheck(currentMon, 2u)) {
                changeForm->newForm = 1;
            }
            else {
                changeForm->newForm = 2;
            }

            BattleHandler_PopWork(serverFlow, changeForm);
        }
    }
}
extern "C" void HandlerGulpMissileDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        u32 prevForm = BattleMon_GetValue(defendingMon, VALUE_FORM);
        // TODO: ADD CRAMORANT
        if (BattleMon_GetSpecies(defendingMon) == PKM_NULL &&
            prevForm != 0) {
            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = 0;
            BattleHandler_PopWork(serverFlow, changeForm);

            u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);

            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, 4u);
            BattleHandler_PopWork(serverFlow, damage);

            if (prevForm == 1) {
                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = attackingSlot;
                statChange->stat = STATSTAGE_DEFENSE;
                statChange->volume = -1;
                BattleHandler_PopWork(serverFlow, statChange);
            }
            else if (prevForm == 2) {
                HandlerParam_AddCondition* addCondition;
                addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
                addCondition->condition = CONDITION_PARALYSIS;
                addCondition->condData = MakeBasicStatus(CONDITION_PARALYSIS);
                addCondition->almost = 0;
                addCondition->pokeID = attackingSlot;
                BattleHandler_PopWork(serverFlow, addCondition);
            }
        }
    }
}
BattleEventHandlerTableEntry GulpMissileHandlers[]{
    {EVENT_MOVE_EXECUTE_START, HandlerGulpMissileCatch},
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerGulpMissileDamage},
};
extern "C" BattleEventHandlerTableEntry * EventAddGulpMissile(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GulpMissileHandlers);
    return GulpMissileHandlers;
}
#endif // EXPAND_ABILITIES