#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_events.h"

// TOXIC SPIKES
extern "C" void HandlerSideToxicSpikesUpdated(BattleEventItem * item, ServerFlow * serverFlow, u32 side, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (side == GetSideFromMonID(currentSlot) &&
        currentMon &&
        !ServerEvent_CheckFloating(serverFlow, currentMon, 1)) {
        if (BattleMon_HasType(currentMon, TYPE_POIS)) {
            HandlerParam_RemoveSideEffect* removeSideEffect;
            removeSideEffect = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, currentSlot);
            removeSideEffect->side = side;
            REMOVE_SIDE_EFFECT_SETUP(removeSideEffect->flags);
            SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_TOXIC_SPIKES, removeSideEffect->flags);
            BattleHandler_PopWork(serverFlow, removeSideEffect);
        }
        else {
            if (CheckDisableEntryHazards(currentMon))
                return;

            ConditionData condData;
            switch (BattleSideStatus_GetCountFromBattleEventItem(item, side))
            {
            case 1:
                condData = Condition_MakePermanent();
                break;
            case 2:
                condData = Condition_MakeBadlyPoisoned();
                break;
            default:
                return;
            }

            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, BATTLE_MAX_SLOTS);
            addCondition->condition = CONDITION_POISON;
            addCondition->condData = condData;
            addCondition->pokeID = currentSlot;
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
BattleEventHandlerTableEntry SideToxicSpikeHandlers[]{
    {EVENT_SWITCH_IN, HandlerSideToxicSpikesUpdated},
};
extern "C" BattleEventHandlerTableEntry * EventAddSideToxicSpikesUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SideToxicSpikeHandlers);
    return SideToxicSpikeHandlers;
}
#endif // EXPAND_FIELD_EFFECTS