#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// RIPEN
extern "C" void HandlerRipen(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        ITEM itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (PML_ItemIsBerry(itemID)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
extern "C" void HandlerRipenNEW(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        ITEM itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (PML_ItemIsBerry(itemID)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry RipenHandlers[]{
    {EVENT_USE_ITEM, HandlerRipen},
    {EVENT_USE_TEMP_ITEM_BEFORE, HandlerRipenNEW},
};
extern "C" BattleEventHandlerTableEntry * EventAddRipen(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RipenHandlers);
    return RipenHandlers;
}
#endif // EXPAND_ABILITIES