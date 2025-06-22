#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// INTREPID SWORD
extern "C" void HandlerIntrepidSword(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
#if GEN9_INTREPID_SWORD
        if (BattleField_CheckIntrepidSwordFlag(pokemonSlot)) {
            return;
        }
        BattleField_SetIntrepidSwordFlag(pokemonSlot);
#endif
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_ATTACK;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry IntrepidSwordHandlers[]{
    {EVENT_SWITCH_IN, HandlerIntrepidSword},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerIntrepidSword},
};
extern "C" BattleEventHandlerTableEntry * EventAddIntrepidSword(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(IntrepidSwordHandlers);
    return IntrepidSwordHandlers;
}
#endif // EXPAND_ABILITIES