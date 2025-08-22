#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

// STICKY WEB
extern "C" void HandlerSideStickyWeb(BattleEventItem* item, ServerFlow* serverFlow, u32 side, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (side == GetSideFromMonID(currentSlot)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (CheckDisableEntryHazards(currentMon) ||
            ServerControl_CheckFloating(serverFlow, currentMon, 1))
            return;

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, currentSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_STICKY_WEB_EFFECT_MSGID);
        BattleHandler_AddArg(&message->str, currentSlot);
        BattleHandler_PopWork(serverFlow, message);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, currentSlot);
        statChange->pokeCount = 1;
        statChange->pokeID[0] = currentSlot;
        statChange->moveAnimation = 1;
        statChange->stat = STATSTAGE_SPEED;
        statChange->volume = -1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry SideStickyWebHandlers[]{
    {EVENT_SWITCH_IN, HandlerSideStickyWeb},
};
extern "C" BattleEventHandlerTableEntry* EventAddSideStickyWeb(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SideStickyWebHandlers);
    return SideStickyWebHandlers;
}
#endif // EXPAND_FIELD_EFFECTS