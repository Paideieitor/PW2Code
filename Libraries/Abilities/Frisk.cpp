#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// FRISK
extern "C" void HandlerFriskUpdated(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u8 frontSlots[6];
        u16 items[6];

        u16 frontPos = Handler_GetExistFrontPokePos(serverFlow, pokemonSlot);
        u32 frontCount = Handler_ExpandPokeID(serverFlow, frontPos | 0x100, frontSlots);

        b32 friskActive = 0;
        for (u32 frontIdx = 0; frontIdx < frontCount; ++frontIdx) {
            BattleMon* frontMon = Handler_GetBattleMon(serverFlow, frontSlots[frontIdx]);
            items[frontIdx] = BattleMon_GetHeldItem(frontMon);
            if (items[frontIdx] != ITEM_NULL) {
                friskActive = 1;
            }
        }
        if (friskActive) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            for (u32 frontIdx = 0; frontIdx < frontCount; ++frontIdx) {
                if (items[frontIdx] != ITEM_NULL) {
                    HandlerParam_Message* message;
                    message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                    BattleHandler_StrSetup(&message->str, 2u, BATTLE_UPDATED_FRISK_MSGID);
                    BattleHandler_AddArg(&message->str, pokemonSlot);
                    BattleHandler_AddArg(&message->str, frontSlots[frontIdx]);
                    BattleHandler_AddArg(&message->str, items[frontIdx]);
                    BattleHandler_PopWork(serverFlow, message);
                }
            }
            
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry FriskHandlers[]{
    {EVENT_SWITCH_IN, HandlerFriskUpdated},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerFriskUpdated},
};
extern "C" BattleEventHandlerTableEntry * EventAddFriskUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FriskHandlers);
    return FriskHandlers;
}
#endif // EXPAND_ABILITIES