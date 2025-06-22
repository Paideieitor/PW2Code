#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// WIND RIDER
MOVE_ID WindMoves[] = {
    MOVE_GUST,
    MOVE_WHIRLWIND,
    MOVE_BLIZZARD,
    MOVE_AEROBLAST,
    MOVE_ICY_WIND,
    MOVE_TWISTER,
    MOVE_HEAT_WAVE,
    MOVE_AIR_CUTTER,
    MOVE_HURRICANE,
#if EXPAND_MOVES
    FAIRY_WIND,
    PETAL_BLIZZARD,
    BLEAKWIND_STORM,
    WILDBOLT_STORM,
    SANDSEAR_STORM,
    SPRINGTIDE_STORM,
#endif
};
extern "C" b32 IsWindMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(WindMoves); ++i) {
        if (moveID == WindMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void WindRiderBoost(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    HandlerParam_ChangeStatStage* statChange;
    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
    statChange->stat = STATSTAGE_ATTACK;
    statChange->volume = 1;
    statChange->pokeCount = 1;
    statChange->pokeID[0] = pokemonSlot;
    BattleHandler_PopWork(serverFlow, statChange);

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
}
extern "C" void HandlerWindRider(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && IsWindMove(moveID)
        && BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1)) {
        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerWindRiderTailwind(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleEventVar_GetValue(VAR_SIDE_EFFECT) == SIDEEFF_TAILWIND) {
        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerWindRiderSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        Handler_IsSideEffectActive(serverFlow, GetSideFromMonID(pokemonSlot), SIDEEFF_TAILWIND)) {
        WindRiderBoost(serverFlow, pokemonSlot);
    }
}
BattleEventHandlerTableEntry WindRiderHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerWindRider},
    {EVENT_CHECK_SIDE_EFFECT_PARAM, HandlerWindRiderTailwind},
    {EVENT_SWITCH_IN, HandlerWindRiderSwitchIn},
};
extern "C" BattleEventHandlerTableEntry * EventAddWindRider(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WindRiderHandlers);
    return WindRiderHandlers;
}

// WIND POWER
extern "C" void CommonChargeHandler(ServerFlow * serverFlow, u32 pokemonSlot, u32 * work, MOVE_ID moveID) {
    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    HandlerParam_Message* message;
    message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
    BattleHandler_StrSetup(&message->str, 1u, BATTLE_WIND_POWER_MSGID);
    BattleHandler_AddArg(&message->str, moveID);
    BattleHandler_AddArg(&message->str, pokemonSlot);
    BattleHandler_PopWork(serverFlow, message);

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

    *work = 1;
}
extern "C" void CommonChargePowerHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (*work > 0
        && pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 8192);
        *work = 0;
    }
}
extern "C" void HandlerWindPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && IsWindMove(moveID)) {
        CommonChargeHandler(serverFlow, pokemonSlot, work, moveID);
    }
}
extern "C" void HandlerWindPowerTailwind(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleEventVar_GetValue(VAR_SIDE_EFFECT) == SIDEEFF_TAILWIND) {
        CommonChargeHandler(serverFlow, pokemonSlot, work, MOVE_TAILWIND);
    }
}
BattleEventHandlerTableEntry WindPowerHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWindPower},
    {EVENT_CHECK_SIDE_EFFECT_PARAM, HandlerWindPowerTailwind},
    {EVENT_MOVE_POWER, CommonChargePowerHandler},
};
extern "C" BattleEventHandlerTableEntry * EventAddWindPower(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WindPowerHandlers);
    return WindPowerHandlers;
}

// ELECTROMORPHOSIS
extern "C" void HandlerElectromorphosis(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        CommonChargeHandler(serverFlow, pokemonSlot, work, moveID);
    }
}
BattleEventHandlerTableEntry ElectromorphosisHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerElectromorphosis},
    {EVENT_MOVE_POWER, CommonChargePowerHandler},
};
extern "C" BattleEventHandlerTableEntry * EventAddElectromorphosis(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ElectromorphosisHandlers);
    return ElectromorphosisHandlers;
}

// TODO: Add "CommonChargePowerHandler" as an include put Electromorphosis in it's own file
// TODO: Once moves have a functional Wind Flag put each ability in it's own file

#endif // EXPAND_ABILITIES