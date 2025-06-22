#include "settings.h"
#if EXPAND_ITEMS

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// ROOM SERVICE
extern "C" void HandlerRoomSeviceFieldCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (BattleField_CheckEffect(FLDEFF_TRICK_ROOM)) {
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(battleMon, STATSTAGE_SPEED, -1)) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void HandlerRoomSeviceUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && 
        BattleField_CheckEffect(FLDEFF_TRICK_ROOM)) {
        HandlerParam_ChangeStatStage* speedBoost;
        speedBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        speedBoost->pokeCount = 1;
        speedBoost->pokeID[0] = (u8)pokemonSlot;
        speedBoost->stat = STATSTAGE_SPEED;
        speedBoost->volume = -1;
        BattleHandler_PopWork(serverFlow, speedBoost);
    }
}
BattleEventHandlerTableEntry RoomSeviceHandlers[] = {
    {EVENT_MOVE_EXECUTE_END, HandlerRoomSeviceFieldCheck},
    {EVENT_SWITCH_IN, HandlerRoomSeviceFieldCheck},
    {EVENT_CHECK_ITEM_REACTION, HandlerRoomSeviceFieldCheck},
    {EVENT_USE_ITEM, HandlerRoomSeviceUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddRoomSevice(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(RoomSeviceHandlers);
    return RoomSeviceHandlers;
}
#endif // EXPAND_ITEMS