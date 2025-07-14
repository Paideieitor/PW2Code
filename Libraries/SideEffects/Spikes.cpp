#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_events.h"

// SPIKES
extern "C" void HandlerSideSpikesUpdated(BattleEventItem * item, ServerFlow * serverFlow, u32 side, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (side == GetSideFromMonID(currentSlot) &&
        currentMon &&
        !ServerEvent_CheckFloating(serverFlow, currentMon, 1)) {
        if (CheckDisableEntryHazards(currentMon))
            return;

        u32 damageFraction = 8;
        switch (BattleSideStatus_GetCountFromBattleEventItem(item, side))
        {
        case 2:
            damageFraction = 6;
            break;
        case 3:
            damageFraction = 4;
            break;
        }

        HandlerParam_Damage* damage;
        damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, BATTLE_MAX_SLOTS);
        damage->pokeID = currentSlot;
        damage->damage = DivideMaxHPZeroCheck(currentMon, damageFraction);
        BattleHandler_StrSetup(&damage->exStr, 2u, 851);
        BattleHandler_AddArg(&damage->exStr, currentSlot);
        BattleHandler_PopWork(serverFlow, damage);
    }
}
BattleEventHandlerTableEntry SideSpikeHandlers[] {
    {EVENT_SWITCH_IN, HandlerSideSpikesUpdated},
};
extern "C" BattleEventHandlerTableEntry * EventAddSideSpikesUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SideSpikeHandlers);
    return SideSpikeHandlers;
}
#endif // EXPAND_FIELD_EFFECTS