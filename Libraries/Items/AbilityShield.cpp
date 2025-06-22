#include "settings.h"
#if EXPAND_ITEMS

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// ABILITY SHIELD
extern "C" void HandlerAbilityShieldAbilityChangeFail(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}
BattleEventHandlerTableEntry AbilityShieldHandlers[] = {
    {EVENT_ABILITY_CHANGE_CHECK_FAIL, HandlerAbilityShieldAbilityChangeFail},
};
extern "C" BattleEventHandlerTableEntry* EventAddAbilityShield(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AbilityShieldHandlers);
    return AbilityShieldHandlers;
}
#endif // EXPAND_ITEMS