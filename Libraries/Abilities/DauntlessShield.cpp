#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// DAUNTLESS SHIELD
extern "C" void HandlerDauntlessShield(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
#if GEN9_DAUNTLESS_SHIELD
        if (BattleField_CheckDauntlessShieldFlag(pokemonSlot)) {
            return;
        }
        BattleField_SetDauntlessShieldFlag(pokemonSlot);
#endif
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_DEFENSE;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry DauntlessShieldHandlers[]{
    {EVENT_SWITCH_IN, HandlerDauntlessShield},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDauntlessShield},
};
extern "C" BattleEventHandlerTableEntry * EventAddDauntlessShield(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DauntlessShieldHandlers);
    return DauntlessShieldHandlers;
}
#endif // EXPAND_ABILITIES