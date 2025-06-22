#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// CHEEK POUCH
extern "C" void HandlerCheekPouch(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ITEM itemID = BattleEventVar_GetValue(VAR_ITEM);
        if (itemID && PML_ItemIsBerry(itemID) && !BattleMon_CheckIfMoveCondition(currentMon, CONDITION_HEALBLOCK)) {
            if (!BattleMon_IsFullHP(currentMon)) {

                HandlerParam_RecoverHP* recoveHP;
                recoveHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
                recoveHP->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                recoveHP->pokeID = pokemonSlot;
                recoveHP->recoverHP = DivideMaxHPZeroCheck(currentMon, 3);
                BattleHandler_StrSetup(&recoveHP->exStr, 2u, 387);
                BattleHandler_AddArg(&recoveHP->exStr, pokemonSlot);
                BattleHandler_PopWork(serverFlow, recoveHP);
            }
        }
    }
}
BattleEventHandlerTableEntry CheekPouchHandlers[]{
    {EVENT_CONSUME_ITEM, HandlerCheekPouch},
    {EVENT_USE_TEMP_ITEM_AFTER, HandlerCheekPouch},
};
extern "C" BattleEventHandlerTableEntry * EventAddCheekPouch(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(CheekPouchHandlers);
    return CheekPouchHandlers;
}
#endif // EXPAND_ABILITIES