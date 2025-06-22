#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// COVERT CLOAK
extern "C" b32 IsAffectedBySheerForce(MOVE_ID moveID);
extern "C" void HandlerCovertCloakConditionCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON) && pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsAffectedBySheerForce(moveID)) {
            BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
extern "C" void HandlerInnerFocus(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
BattleEventHandlerTableEntry CovertCloakHandlers[] = {
    {EVENT_ADD_CONDITION, HandlerCovertCloakConditionCheck},
    {EVENT_ADD_STAT_STAGE_CHANGE_TARGET, HandlerCovertCloakConditionCheck},
    {EVENT_FLINCH_CHECK, HandlerInnerFocus},
};
extern "C" BattleEventHandlerTableEntry* EventAddCovertCloak(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CovertCloakHandlers);
    return CovertCloakHandlers;
}
#endif // EXPAND_ITEMS