#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SPICY SPRAY
extern "C" void HandlerSpicySpray(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);

        HandlerParam_AddCondition* addCondition;
        addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
        addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        addCondition->condition = CONDITION_BURN;
        addCondition->pokeID = attackingSlot;
        addCondition->condData = MakeBasicStatus(CONDITION_BURN);
        BattleHandler_PopWork(serverFlow, addCondition);
    }
}
BattleEventHandlerTableEntry SpicySprayHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSpicySpray},
};
extern "C" BattleEventHandlerTableEntry * EventAddSpicySpray(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SpicySprayHandlers);
    return SpicySprayHandlers;
}
#endif // EXPAND_ABILITIES
