#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// CUD CHEW
// work[0] -> berry that has been consumed
// work[1] -> turn count until consuming the berry again
extern "C" void HandlerCudChew(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ITEM itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (itemID && PML_ItemIsBerry(itemID)) {

            work[0] = itemID;
            work[1] = 0;
        }
    }
}
extern "C" void HandlerCudChewConsume(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[0] && pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        ++work[1];

        if (work[1] == 2) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ActivateItem* forceUseItem;
            forceUseItem = (HandlerParam_ActivateItem*)BattleHandler_PushWork(serverFlow, EFFECT_FORCE_USE_HELD_ITEM, pokemonSlot);
            forceUseItem->header.flags |= 0x1000000u;
            forceUseItem->itemID = work[0];
            forceUseItem->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, forceUseItem);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

            work[0] = 0;
            work[1] = 0;
        }
    }
}
BattleEventHandlerTableEntry CudChewHandlers[]{
    {EVENT_CONSUME_ITEM, HandlerCudChew},
    {EVENT_USE_TEMP_ITEM_AFTER, HandlerCudChew},
    {EVENT_TURN_CHECK_END, HandlerCudChewConsume},
};
extern "C" BattleEventHandlerTableEntry * EventAddCudChew(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(CudChewHandlers);
    return CudChewHandlers;
}
#endif // EXPAND_ABILITIES