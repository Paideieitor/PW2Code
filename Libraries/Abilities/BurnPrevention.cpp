#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// WATER BUBBLE
extern "C" void HandlerWaterBubblePreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_BURN) {
        *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
    }
}
extern "C" void CommonStatusFailMessageHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot && *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, 68); // no effect default message
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
extern "C" void HandlerWaterBubbleHealBurn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_GetStatus(currentMon) == CONDITION_BURN) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_CureCondition* cureCondition;
        cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
        cureCondition->condition = CONDITION_BURN;
        cureCondition->pokeCount = 1;
        cureCondition->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, cureCondition);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerWaterBubbleResist(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FIRE) {
        BattleEventVar_MulValue(VAR_RATIO, 2048);
    }
}
extern "C" void HandlerWaterBubblePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_WATER) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 8192);
    }
}
BattleEventHandlerTableEntry WaterBubbleHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerWaterBubblePreventStatus},
    {EVENT_ADD_CONDITION_FAIL, CommonStatusFailMessageHandler},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerWaterBubbleHealBurn},
    {EVENT_ACTION_PROCESSING_END, HandlerWaterBubbleHealBurn},
    {EVENT_ATTACKER_POWER, HandlerWaterBubbleResist},
    {EVENT_MOVE_POWER, HandlerWaterBubblePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddWaterBubble(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WaterBubbleHandlers);
    return WaterBubbleHandlers;
}

// THERMAL EXCHANGE
extern "C" void HandlerThermalExchange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG) &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FIRE) {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = STATSTAGE_ATTACK;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry ThermalExchangeHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerWaterBubblePreventStatus},
    {EVENT_ADD_CONDITION_FAIL, CommonStatusFailMessageHandler},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerWaterBubbleHealBurn},
    {EVENT_ACTION_PROCESSING_END, HandlerWaterBubbleHealBurn},
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerThermalExchange},
};
extern "C" BattleEventHandlerTableEntry * EventAddThermalExchange(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ThermalExchangeHandlers);
    return ThermalExchangeHandlers;
}

// PURIFYING SALT
extern "C" void HandlerPurifyingSaltPreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        CONDITION condition = BattleEventVar_GetValue(VAR_CONDITION_ID);
        if (condition == CONDITION_PARALYSIS ||
            condition == CONDITION_SLEEP ||
            condition == CONDITION_FREEZE ||
            condition == CONDITION_BURN ||
            condition == CONDITION_POISON ||
            condition == CONDITION_CONFUSION ||
            condition == CONDITION_YAWN) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
extern "C" void HandlerPurifyingSaltGhostDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_GHOST) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry PurifyingSaltHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerPurifyingSaltPreventStatus},
    {EVENT_ADD_CONDITION_FAIL, CommonStatusFailMessageHandler},
    {EVENT_ATTACKER_POWER, HandlerPurifyingSaltGhostDamage},
};
extern "C" BattleEventHandlerTableEntry * EventAddPurifyingSalt(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PurifyingSaltHandlers);
    return PurifyingSaltHandlers;
}

// TODO: Add "CommonStatusFailMessageHandler" as an include put Purifying Salt in it's own file
#endif // EXPAND_ABILITIES