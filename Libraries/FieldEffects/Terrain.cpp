#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/include/Terrains.h"
#include "Patches/BattleUpgrade/include/BattleField.h"
#include "Patches/BattleUpgrade/include/ActionOrderManager.h"

#include "server_flow.h"

// TERRAIN FIELD
// STATUS PREVENTION TERRAIN EFFECTS (ELECTRIC & MISTY)
extern "C" void HandlerTerrainPreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    TERRAIN terrain = ServerEvent_GetTerrain(serverFlow);
    if (terrain != TERRAIN_ELECTRIC && terrain != TERRAIN_MISTY) {
        return;
    }

    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
    if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
        CONDITION condition = BattleEventVar_GetValue(VAR_CONDITION_ID);
        if (terrain == TERRAIN_ELECTRIC) {
            if (condition == CONDITION_SLEEP ||
                condition == CONDITION_YAWN) {
                *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
            }
        }
        else if (terrain == TERRAIN_MISTY) {
            if (condition == CONDITION_PARALYSIS ||
                condition == CONDITION_SLEEP ||
                condition == CONDITION_FREEZE ||
                condition == CONDITION_BURN ||
                condition == CONDITION_POISON ||
                condition == CONDITION_CONFUSION ||
                condition == CONDITION_YAWN)
            {
                *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
            }
        }
    }
}
extern "C" void HandlerTerrainStatusFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (*work) {
        TERRAIN terrain = ServerEvent_GetTerrain(serverFlow);
        if (terrain != TERRAIN_ELECTRIC && terrain != TERRAIN_MISTY) {
            return;
        }

        u16 msgID = BATTLE_ELECTRIC_TERRAIN_STATUS_MSGID;
        if (terrain == TERRAIN_MISTY)
            msgID = BATTLE_MISTY_TERRAIN_STATUS_MSGID;

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, BATTLE_MAX_SLOTS);
        BattleHandler_StrSetup(&message->str, 2u, msgID);
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);
    }
}
// ELECTRIC TERRAIN
extern "C" void CommonElectricTerrainCureStatus(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SLEEP)) {
        if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
            HandlerParam_CureCondition* cureCondition;
            cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            cureCondition->condition = CONDITION_SLEEP;
            cureCondition->pokeCount = 1;
            cureCondition->pokeID[0] = pokemonSlot;
            BattleHandler_PopWork(serverFlow, cureCondition);
        }
    }
}
extern "C" void HandlerElectricTerrainCheckSleep(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_ELECTRIC) {
        return;
    }

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
            BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
        }
    }
}
extern "C" void HandlerElectricTerrainTerrainChange(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(NEW_VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainSwitchIn(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainActProcEnd(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainFloatingChange(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(VAR_MON_ID));
}
extern "C" void HandlerElectricTerrainFloatingChangeNEW(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_ELECTRIC) {
        return;
    }

    CommonElectricTerrainCureStatus(serverFlow, BattleEventVar_GetValue(NEW_VAR_MON_ID));
}
// GRASSY TERRAIN
extern "C" void HandlerGrassyTerrainHeal(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work)
{
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_GRASSY) {
        return;
    }

    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (!ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
        if (!BattleMon_IsFullHP(currentMon)) {
            HandlerParam_RecoverHP* recoveHP;
            recoveHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, currentSlot);
            recoveHP->pokeID = currentSlot;
            recoveHP->recoverHP = DivideMaxHPZeroCheck(currentMon, 16);
            BattleHandler_StrSetup(&recoveHP->exStr, 2u, 387);
            BattleHandler_AddArg(&recoveHP->exStr, currentSlot);
            BattleHandler_PopWork(serverFlow, recoveHP);
        }
    }
}
extern "C" void HandlerGrassyTerrainQuakeMoves(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_GRASSY) {
        return;
    }

    MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
    if (moveID == MOVE_EARTHQUAKE ||
        moveID == MOVE_MAGNITUDE ||
        moveID == MOVE_BULLDOZE) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
            u32 power = BattleEventVar_GetValue(VAR_MOVE_POWER);
            BattleEventVar_RewriteValue(VAR_MOVE_POWER, power / 2);
        }
    }
}
// PSYCHIC TERRAIN
extern "C" void HandlerPsychicTerrainPreventPrio(BattleEventItem * a1, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (ServerEvent_GetTerrain(serverFlow) != TERRAIN_PSYCHIC) {
        return;
    }

    u32 currentSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (currentSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        return;
    }

    BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, currentSlot);
    if (!ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
        ActionOrderWork* actionOrder = serverFlow->actionOrderWork;
        u16 orderIdx = 0;

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        for (; orderIdx < 6; ++orderIdx) {
            if (actionOrder[orderIdx].battleMon == attackingMon) {
                break;
            }
        }

        u32 priority = ACTION_ORDER_GET_PRIO(actionOrder, orderIdx);
        priority -= 7;
        // Special priority takes into account item & ability prio boosts (1 = no added prio)
        u32 special_priority = ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, orderIdx);
        special_priority -= 1;
        priority += special_priority;

        if (priority > 0) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry FieldTerrainHandlers[] = {
    // STATUS PREVENTION TERRAIN EFFECTS (ELECTRIC & MISTY)
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerTerrainPreventStatus},
    {EVENT_ADD_CONDITION_FAIL, HandlerTerrainStatusFailMessage},
    // ELECTRIC TERRAIN
    {EVENT_CHECK_SLEEP, HandlerElectricTerrainCheckSleep},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerElectricTerrainTerrainChange},
    {EVENT_SWITCH_IN, HandlerElectricTerrainSwitchIn},
    {EVENT_ACTION_PROCESSING_END, HandlerElectricTerrainActProcEnd},
    {EVENT_ITEM_REWRITE_DONE, HandlerElectricTerrainFloatingChange},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerElectricTerrainFloatingChange},
    {EVENT_GROUNDED_BY_GRAVITY, HandlerElectricTerrainFloatingChangeNEW},
    // GRASSY TERRAIN
    {EVENT_TURN_CHECK_BEGIN, HandlerGrassyTerrainHeal},
    {EVENT_MOVE_BASE_POWER, HandlerGrassyTerrainQuakeMoves},
    // PSYCHIC TERRAIN
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerPsychicTerrainPreventPrio},
    // Terrain damage boost is aplied in [ServerEvent_CalcDamage]
};
BattleEventHandlerTableEntry* EventAddFieldTerrain(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FieldTerrainHandlers);
    return FieldTerrainHandlers;
}
#endif // EXPAND_FIELD_EFFECTS