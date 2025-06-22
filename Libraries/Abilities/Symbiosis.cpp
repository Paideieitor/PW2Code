#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SYMBIOSIS
ITEM dmgReduceBerries[] = {
    ITEM_OCCA_BERRY,
    ITEM_PASSHO_BERRY,
    ITEM_WACAN_BERRY,
    ITEM_RINDO_BERRY,
    ITEM_YACHE_BERRY,
    ITEM_CHOPLE_BERRY,
    ITEM_KEBIA_BERRY,
    ITEM_SHUCA_BERRY,
    ITEM_COBA_BERRY,
    ITEM_PAYAPA_BERRY,
    ITEM_TANGA_BERRY,
    ITEM_CHARTI_BERRY,
    ITEM_KASIB_BERRY,
    ITEM_HABAN_BERRY,
    ITEM_COLBUR_BERRY,
    ITEM_BABIRI_BERRY,
    ITEM_CHILAN_BERRY,
#if EXPAND_ITEMS
    ITEM_ROSELI_BERRY,
#endif
};
extern "C" b32 IsDmgReduceBerry(ITEM itemID) {
    for (u16 i = 0; i < ARRAY_COUNT(dmgReduceBerries); ++i) {
        if (itemID == dmgReduceBerries[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void SymbiosisChangeItem(ServerFlow * serverFlow, u32 pokemonSlot, u32 receiveSlot) {
#if SYMBIOSIS_DEBUG
    DPRINTF("SYMBIOSIS -> SLOT: %d | RECEIVE SLOT: %d\n", pokemonSlot, receiveSlot);
#endif
    if (pokemonSlot != receiveSlot && MainModule_IsAllyMonID(pokemonSlot, receiveSlot)) {
        BattleMon* receiveMon = Handler_GetBattleMon(serverFlow, receiveSlot);
        ITEM allyItem = BattleMon_GetHeldItem(receiveMon);
#if SYMBIOSIS_DEBUG
        DPRINTF("SYMBIOSIS -> ALLY ITEM: %d\n", allyItem);
#endif
        if (!allyItem) {
            BattleMon* giveMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            int giveItem = BattleMon_GetHeldItem(giveMon);
#if SYMBIOSIS_DEBUG
            DPRINTF("SYMBIOSIS -> ITEM TO GIVE: %d\n", giveItem);
#endif
            if (giveItem) {
                if (!PML_ItemIsMail(giveItem)
                    && !HandlerCommon_IsUnremovableItem(BattleMon_GetSpecies(giveMon), giveItem)
                    && !HandlerCommon_IsUnremovableItem(BattleMon_GetSpecies(receiveMon), giveItem)) {
#if SYMBIOSIS_DEBUG
                    DPRINT("SYMBIOSIS -> SUCCESS\n");
#endif
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_SwapItem* swapItem;
                    swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                    swapItem->pokeID = receiveSlot;
                    BattleHandler_StrSetup(&swapItem->exStr, 2u, BATTLE_SYMBIOSIS_MSGID);
                    BattleHandler_AddArg(&swapItem->exStr, pokemonSlot);
                    BattleHandler_StrSetup(&swapItem->exSubStr1, 2u, 685);
                    BattleHandler_AddArg(&swapItem->exSubStr1, receiveSlot);
                    BattleHandler_AddArg(&swapItem->exSubStr1, giveItem);
                    BattleHandler_PopWork(serverFlow, swapItem);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }
        }
    }
}
extern "C" void HandlerSymbiosis(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    u32 receiveSlot = BattleEventVar_GetValue(NEW_VAR_MON_ID);
    ITEM itemID = BattleEventVar_GetValue(VAR_ITEM);

    // We initially set the delayed slot as null
    *work = BATTLE_MAX_SLOTS;

    // DELAY CHECK
    if (IsDmgReduceBerry(itemID)
#if GEN7_SYMBIOSIS_GEMS
        || IsGem(itemID)
#endif
        )
    {
        // store the delayed slot if needed
        *work = receiveSlot;
#if SYMBIOSIS_DEBUG
        DPRINTF("CHECK DELAY -> DELAY SLOT: %d\n", receiveSlot);
#endif
        return;
    }

    SymbiosisChangeItem(serverFlow, pokemonSlot, receiveSlot);
}
extern "C" void HandlerSymbiosisDelayed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // If a we have a delayed slot stored
    if (*work != BATTLE_MAX_SLOTS) {
#if SYMBIOSIS_DEBUG
        DPRINTF("SYMBIOSIS DELAY -> DELAY SLOT: %d\n", *work);
#endif
        // Trigger the delayed swap
        SymbiosisChangeItem(serverFlow, pokemonSlot, *work);
    }
    // Reset the event variable
    *work = BATTLE_MAX_SLOTS;
}
BattleEventHandlerTableEntry SymbiosisHandlers[]{
    {EVENT_CONSUME_ITEM, HandlerSymbiosis}, // swap item or delay if it's a damage affecting item
    {EVENT_USE_TEMP_ITEM_AFTER, HandlerSymbiosis}, // swap when consuming temp item
    {EVENT_MOVE_DAMAGE_SIDE_AFTER, HandlerSymbiosisDelayed}, // swap item if we are delaying
};
extern "C" BattleEventHandlerTableEntry * EventAddSymbiosis(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SymbiosisHandlers);
    return SymbiosisHandlers;
}
#endif // EXPAND_ABILITIES