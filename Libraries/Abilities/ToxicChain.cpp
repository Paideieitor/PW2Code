#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// TOXIC CHAIN
extern "C" void HandlerToxicChain(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !BattleEventVar_GetValue(VAR_SHIELD_DUST_FLAG)) {

        if (AbilityEvent_RollEffectChance(serverFlow, 30u)) {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            addCondition->pokeID = BattleEventVar_GetValue(VAR_DEFENDING_MON);
            addCondition->condition = CONDITION_POISON;
            addCondition->condData = Condition_MakeBadlyPoisoned();
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
BattleEventHandlerTableEntry ToxicChainHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerToxicChain},
};
extern "C" BattleEventHandlerTableEntry * EventAddToxicChain(u32 * handlerAmount) {
    *handlerAmount = GetNumHandlersWithPriority(EVENTPRI_ABILITY_POISON_TOUCH, ARRAY_COUNT(ToxicChainHandlers));
    return ToxicChainHandlers;
}
#endif // EXPAND_ABILITIES