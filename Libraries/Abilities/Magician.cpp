#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// MAGICIAN
extern "C" void HandlerMagician(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_GetHeldItem(currentMon) == ITEM_NULL && !*work) {
            u32 targetSlot = BattleEventVar_GetValue(VAR_TARGET_MON_ID);
            if (targetSlot != BATTLE_MAX_SLOTS) {
                BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
                if (BattleMon_GetHeldItem(targetMon)) {
                    if (!HandlerCommon_CheckIfCanStealPokeItem(serverFlow, pokemonSlot, targetSlot)) {
                        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                        HandlerParam_SwapItem* swapItem;
                        swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                        swapItem->pokeID = targetSlot;
                        BattleHandler_StrSetup(&swapItem->exStr, 2u, 1057);
                        BattleHandler_AddArg(&swapItem->exStr, pokemonSlot);
                        BattleHandler_AddArg(&swapItem->exStr, targetSlot);
                        ITEM heldItem = BattleMon_GetHeldItem(targetMon);
                        BattleHandler_AddArg(&swapItem->exStr, heldItem);
                        BattleHandler_PopWork(serverFlow, swapItem);

                        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                    }
                }
            }
        }
    }
}
BattleEventHandlerTableEntry MagicianHandlers[]{
    {EVENT_DAMAGE_PROCESSING_START, HandlerThiefStart},
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerMagician},
};
extern "C" BattleEventHandlerTableEntry * EventAddMagician(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MagicianHandlers);
    return MagicianHandlers;
}
#endif // EXPAND_ABILITIES