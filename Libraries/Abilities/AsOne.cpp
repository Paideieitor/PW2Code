#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

extern "C" void NeighBoost(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 target = 0; target < targetCount; ++target) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + target));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon)) {
                BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
                if (BattleMon_IsStatChangeValid(currentMon, stat, 1)) {

                    HandlerParam_Header* header = BattleHandler_PushWork(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
                    // This lets the handler know what ability should be
                    BattleField_SetAsOnePopUpFlags(AS_ONE_SECOND_ABILITY_FLAG);
                    BattleHandler_PopWork(serverFlow, header);

                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    statChange->stat = stat;
                    statChange->volume = 1;
                    BattleHandler_PopWork(serverFlow, statChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }
        }
    }
}

// CHILLING NEIGH
extern "C" void HandlerChillingNeigh(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    NeighBoost(serverFlow, pokemonSlot, STATSTAGE_ATTACK);
}
BattleEventHandlerTableEntry ChillingNeighHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerChillingNeigh},
};
extern "C" BattleEventHandlerTableEntry * EventAddChillingNeigh(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ChillingNeighHandlers);
    return ChillingNeighHandlers;
}

// GRIM NEIGH
extern "C" void HandlerGrimNeigh(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    NeighBoost(serverFlow, pokemonSlot, STATSTAGE_SPECIAL_ATTACK);
}
BattleEventHandlerTableEntry GrimNeighHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerGrimNeigh},
};
extern "C" BattleEventHandlerTableEntry * EventAddGrimNeigh(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GrimNeighHandlers);
    return GrimNeighHandlers;
}

// AS ONE
extern "C" void HandlerAsOneUnnerve(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (!*work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_AS_ONE_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        HandlerParam_Header* header = BattleHandler_PushWork(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        // This lets the handler know what ability should be
        BattleField_SetAsOnePopUpFlags(AS_ONE_FIRST_ABILITY_FLAG);
        BattleHandler_PopWork(serverFlow, header);

        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, 176u);
        BattleHandler_AddArg(&message->str, GetSideFromMonID(pokemonSlot) == 0);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        BattleEventItem_AttachSkipCheckHandler(item, HandlerUnnerveSkipCheck);
        *work = 1;
    }
}
extern "C" void HandlerAsOneUnnerveSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerAsOneUnnerve(item, serverFlow, pokemonSlot, work);
    }
}
extern "C" void HandlerAsOneNeigh(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        ABILITY ability = BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY);
        if (ability == ABIL_AS_ONE_ICE) {
            NeighBoost(serverFlow, pokemonSlot, STATSTAGE_ATTACK);
        }
        else if (ability == ABIL_AS_ONE_SHADOW) {
            NeighBoost(serverFlow, pokemonSlot, STATSTAGE_SPECIAL_ATTACK);
        }
    }
}
BattleEventHandlerTableEntry AsOneHandlers[]{
    {EVENT_SWITCH_IN, HandlerAsOneUnnerveSwitchIn},
    {EVENT_CHECK_ACTIVATION, HandlerAsOneUnnerve},
    {EVENT_SWITCH_IN_PREVIOUS, HandlerAsOneUnnerve},
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerAsOneNeigh},
};
extern "C" BattleEventHandlerTableEntry * EventAddAsOne(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AsOneHandlers);
    return AsOneHandlers;
}
#endif // EXPAND_ABILITIES