#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// BALL FETCH
extern "C" void HandlerBallFetch(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Has no item
        if (BattleMon_GetHeldItem(currentMon) == ITEM_NULL) {
            ActionOrderWork* actionOrderWork = serverFlow->actionOrderWork;
            for (u8 i = 0; i < 6; ++i) {
                u32 action = BattleAction_GetAction(&actionOrderWork[i].action);
                // Action is an item used by the player
                if (action == 2) {

                    BattleAction_Item* itemAction = (BattleAction_Item*)&actionOrderWork[i].action;
                    ITEM itemID = itemAction->itemID;
                    // Item is a PokéBall
                    if (ItemGetParam(itemID, ITSTAT_CLASS) == 4) {
                        HandlerParam_SetItem* setItem;
                        setItem = (HandlerParam_SetItem*)BattleHandler_PushWork(serverFlow, EFFECT_SET_HELD_ITEM, pokemonSlot);
                        setItem->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                        setItem->itemID = itemID;
                        setItem->pokeID = pokemonSlot;
                        setItem->clearConsume = 1;
                        BattleHandler_StrSetup(&setItem->exStr, 2u, BATTLE_BALL_FETCH_MSGID);
                        BattleHandler_AddArg(&setItem->exStr, pokemonSlot);
                        BattleHandler_AddArg(&setItem->exStr, itemID);
                        BattleHandler_PopWork(serverFlow, setItem);
                    }
                }
            }
        }
    }
}
BattleEventHandlerTableEntry HandlerBallHandlers[]{
    {EVENT_TURN_CHECK_DONE, HandlerBallFetch},
};
extern "C" BattleEventHandlerTableEntry * EventAddBallFetch(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HandlerBallHandlers);
    return HandlerBallHandlers;
}
#endif // EXPAND_ABILITIES