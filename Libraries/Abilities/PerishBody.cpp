#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// PERISH BODY
extern "C" void HandlerPerishBody(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_AddCondition* addConditionAtk;
            addConditionAtk = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addConditionAtk->pokeID = attackingSlot;
            addConditionAtk->condition = CONDITION_PERISHSONG;
            addConditionAtk->condData = Condition_MakeTurn(4);
            BattleHandler_PopWork(serverFlow, addConditionAtk);

            HandlerParam_AddCondition* addConditionDef;
            addConditionDef = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addConditionDef->pokeID = defendingSlot;
            addConditionDef->condition = CONDITION_PERISHSONG;
            addConditionDef->condData = Condition_MakeTurn(4);
            BattleHandler_PopWork(serverFlow, addConditionDef);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 1u, BATTLE_PERISH_BODY_MSGID);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry PerishBodyHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerPerishBody},
};
extern "C" BattleEventHandlerTableEntry * EventAddPerishBody(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PerishBodyHandlers);
    return PerishBodyHandlers;
}
#endif // EXPAND_ABILITIES