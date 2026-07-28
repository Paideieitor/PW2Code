#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// SPARKLING ARIA
extern "C" void HandlerSparklingAria(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT); 

        for (u32 targetIdx = 0; targetIdx < targetCount; ++targetIdx) {
            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + targetIdx));

            if (targetSlot != BATTLE_MAX_SLOTS) {
                BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);

                if (BattleMon_CheckIfMoveCondition(targetMon, CONDITION_BURN)) {

                    HandlerParam_CureCondition* cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
                    cureCondition->pokeID[0] = targetSlot;
                    cureCondition->pokeCount = 1;
                    cureCondition->condition = CONDITION_BURN;
                    BattleHandler_PopWork(serverFlow, cureCondition);
                }
            }
        }
    }
}

BattleEventHandlerTableEntry SparklingAriaHandlers[] {
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerSparklingAria},
};

extern "C" BattleEventHandlerTableEntry* EventAddSparklingAria(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SparklingAriaHandlers);
    return SparklingAriaHandlers;
}

#endif // EXPAND_MOVES
