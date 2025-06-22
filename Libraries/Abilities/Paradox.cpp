#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"
#include "server_events.h"

extern "C" ITEM BattleMon_GetUsableItem(BattleMon * battleMon);

// PARADOX ABILITIES
// work[0] -> the stat it is boosting
// work[1] -> Booster Energy flag
// work[2] -> delay activation check flag
extern "C" StatStage GetHighestStatWithBoosts(BattleMon * battleMon) {
    u16 highest = BattleMon_GetValue(battleMon, VALUE_ATTACK_STAT);
    StatStage output = STATSTAGE_ATTACK;

    if (u16 defense = BattleMon_GetValue(battleMon, VALUE_DEFENSE_STAT) > highest) {
        highest = defense;
        output = STATSTAGE_DEFENSE;
    }
    if (u16 specialAttack = BattleMon_GetValue(battleMon, VALUE_SPECIAL_ATTACK_STAGE) > highest) {
        highest = specialAttack;
        output = STATSTAGE_SPECIAL_ATTACK;
    }
    if (u16 specialDefense = BattleMon_GetValue(battleMon, VALUE_SPECIAL_DEFENSE_STAGE) > highest) {
        highest = specialDefense;
        output = STATSTAGE_SPECIAL_DEFENSE;
    }
    if (u16 speed = BattleMon_GetValue(battleMon, VALUE_SPEED_STAT) > highest) {
        highest = speed;
        output = STATSTAGE_SPEED;
    }

    return output;
}
extern "C" void CommonParadoxActivationHandler(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work, b32 fieldCondition) {
    if (work[0] == STATSTAGE_NULL) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
#if PARADOX_DEBUG
        DPRINTF("    PARADOX %d -> ITEM: %d \n", pokemonSlot, BattleMon_GetUsableItem(currentMon));
#endif
        if (!fieldCondition && BattleMon_GetUsableItem(currentMon) == ITEM_BOOSTER_ENERGY) {
            work[1] = 1;
        }

#if PARADOX_DEBUG
        DPRINTF("    PARADOX %d -> FLAG: %d \n", pokemonSlot, BattleField_CheckParadoxAbilityFlag(pokemonSlot));
#endif
        if (work[1] == 1 || fieldCondition) {
            work[0] = GetHighestStatWithBoosts(currentMon);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            // Activate the Energy Booster message and consumition
            if (work[1] == 1) {
#if PARADOX_DEBUG
                DPRINTF("    BOOSTER ENERGY %d \n", pokemonSlot);
#endif
                ItemEvent_PushRun(item, serverFlow, pokemonSlot);
            }

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_PARADOX_BOOST_MSGID + ((work[0] - 1) * 3));
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
        else {
            work[0] = STATSTAGE_NULL;
        }
    }
}
extern "C" void CommonParadoxAttackHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat, b32 fieldCondition) {
    if (fieldCondition) {

        if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
            (stat == STATSTAGE_ATTACK || stat == STATSTAGE_SPECIAL_ATTACK)) {

            MoveCategory category = CATEGORY_PHYSICAL;
            if (stat == STATSTAGE_SPECIAL_ATTACK) {
                MoveCategory category = CATEGORY_SPECIAL;
            }
            if (category == BattleEventVar_GetValue(VAR_MOVE_CATEGORY)) {
                BattleEventVar_MulValue(VAR_RATIO, 5324);
            }
        }
    }
}
extern "C" void CommonParadoxDefenseHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat, b32 fieldCondition) {
    if (fieldCondition) {

        if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
            (stat == STATSTAGE_DEFENSE || stat == STATSTAGE_SPECIAL_DEFENSE)) {
            MoveCategory category = CATEGORY_PHYSICAL;
            if (stat == STATSTAGE_SPECIAL_DEFENSE) {
                MoveCategory category = CATEGORY_SPECIAL;
            }

            if (category == BattleEventVar_GetValue(VAR_MOVE_CATEGORY)) {
                BattleEventVar_MulValue(VAR_RATIO, 5324);
            }
        }
    }
}
extern "C" void CommonParadoxSpeedHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat, b32 fieldCondition) {
    if (fieldCondition) {

        if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
            stat == STATSTAGE_SPEED) {
            BattleEventVar_MulValue(VAR_RATIO, 6144);
        }
    }
}
extern "C" void HandlerBoosterEnergyUse(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[1] == 1) {
        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_BOOSTER_ENERGY_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_AddArg(&message->str, ITEM_BOOSTER_ENERGY);
        BattleHandler_AddArg(&message->str, item->subID);
        BattleHandler_PopWork(serverFlow, message);
    }
}

// PROTOSYNTHESIS
extern "C" void HandlerProtosynthesisSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // Wait for weather/terrain effects to activate when switching in the first turn
    if (serverFlow->turnCount == 0 && serverFlow->numActOrder == 0) {
        work[2] = 1;
        return;
    }

#if PARADOX_DEBUG
    DPRINTF("PROTO SWITCH %d -> MON: %d \n", pokemonSlot, BattleEventVar_GetValue(VAR_MON_ID));
#endif
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        // Check the weather directly to avoid Cloud Nine or Air Lock
        CommonParadoxActivationHandler(item, serverFlow, pokemonSlot, work, BattleField_GetWeather() == WEATHER_SUN);
    }
}
extern "C" void HandlerProtosynthesisAfterLastSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // Check for activation if it was delayed
    if (work[2] != 0) {
#if PARADOX_DEBUG
        DPRINTF("PROTO DELAY %d \n", pokemonSlot);
#endif
        CommonParadoxActivationHandler(item, serverFlow, pokemonSlot, work, BattleField_GetWeather() == WEATHER_SUN);
        work[2] = 0;
    }
}
extern "C" void HandlerProtosynthesisWeather(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
#if PARADOX_DEBUG
    DPRINTF("PROTO WEATHER %d \n", pokemonSlot);
#endif
    CommonParadoxActivationHandler(item, serverFlow, pokemonSlot, work, BattleField_GetWeather() == WEATHER_SUN);
}
extern "C" void HandlerProtosynthesisAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxAttackHandler(serverFlow, pokemonSlot, (StatStage)work[0], 
        work[1] == 1 || ServerEvent_GetWeather(serverFlow) == WEATHER_SUN);
}
extern "C" void HandlerProtosynthesisDefense(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxDefenseHandler(serverFlow, pokemonSlot, (StatStage)work[0], 
        work[1] == 1 || ServerEvent_GetWeather(serverFlow) == WEATHER_SUN);
}
extern "C" void HandlerProtosynthesisSpeed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxSpeedHandler(serverFlow, pokemonSlot, (StatStage)work[0], 
        work[1] == 1 || ServerEvent_GetWeather(serverFlow) == WEATHER_SUN);
}
BattleEventHandlerTableEntry ProtosynthesisHandlers[]{
    {EVENT_SWITCH_IN, HandlerProtosynthesisSwitchIn},
    {EVENT_AFTER_LAST_SWITCHIN, HandlerProtosynthesisAfterLastSwitchIn},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerProtosynthesisSwitchIn},
    {EVENT_CHECK_ITEM_REACTION, HandlerProtosynthesisSwitchIn},
    {EVENT_AFTER_WEATHER_CHANGE, HandlerProtosynthesisWeather},
    {EVENT_ATTACKER_POWER, HandlerProtosynthesisAttack},
    {EVENT_DEFENDER_GUARD, HandlerProtosynthesisDefense},
    {EVENT_CALC_SPEED, HandlerProtosynthesisSpeed},
    {EVENT_USE_ITEM, HandlerBoosterEnergyUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddProtosynthesis(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ProtosynthesisHandlers);
    return ProtosynthesisHandlers;
}

// QUARK DRIVE
extern "C" void HandlerQuarkDriveSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // Wait for weather/terrain effects to activate when switching in the first turn
    if (serverFlow->turnCount == 0 && serverFlow->numActOrder == 0) {
        work[2] = 1;
        return;
    }

#if PARADOX_DEBUG
    DPRINTF("QUARK SWITCH %d -> MON: %d \n", pokemonSlot, BattleEventVar_GetValue(VAR_MON_ID));
#endif
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        // Check the terrain directly to avoid terrain disabling
        CommonParadoxActivationHandler(item, serverFlow, pokemonSlot, work, ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
    }
}
extern "C" void HandlerQuarkDriveAfterLastSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    // Check for activation if it was delayed
    if (work[2] != 0) {
#if PARADOX_DEBUG
        DPRINTF("QUARK DELAY %d \n", pokemonSlot);
#endif
        CommonParadoxActivationHandler(item, serverFlow, pokemonSlot, work, ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
        work[2] = 0;
    }
}
extern "C" void HandlerQuarkDriveTerrain(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;
    
#if PARADOX_DEBUG
    DPRINTF("QUARK TERRAIN %d \n", pokemonSlot);
#endif
    CommonParadoxActivationHandler(item, serverFlow, pokemonSlot, work, ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
extern "C" void HandlerQuarkDriveAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxAttackHandler(serverFlow, pokemonSlot, (StatStage)work[0],
        work[1] == 1 || ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
extern "C" void HandlerQuarkDriveDefense(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxDefenseHandler(serverFlow, pokemonSlot, (StatStage)work[0],
        work[1] == 1 || ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
extern "C" void HandlerQuarkDriveSpeed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonParadoxSpeedHandler(serverFlow, pokemonSlot, (StatStage)work[0],
        work[1] == 1 || ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC);
}
BattleEventHandlerTableEntry QuarkDriveHandlers[]{
    {EVENT_SWITCH_IN, HandlerQuarkDriveSwitchIn},
    {EVENT_AFTER_LAST_SWITCHIN, HandlerQuarkDriveAfterLastSwitchIn},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerQuarkDriveSwitchIn},
    {EVENT_CHECK_ITEM_REACTION, HandlerQuarkDriveSwitchIn},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerQuarkDriveTerrain},
    {EVENT_ATTACKER_POWER, HandlerQuarkDriveAttack},
    {EVENT_DEFENDER_GUARD, HandlerQuarkDriveDefense},
    {EVENT_CALC_SPEED, HandlerQuarkDriveSpeed},
    {EVENT_USE_ITEM, HandlerBoosterEnergyUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddQuarkDrive(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(QuarkDriveHandlers);
    return QuarkDriveHandlers;
}
#endif // EXPAND_ABILITIES