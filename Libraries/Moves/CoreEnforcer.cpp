#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// CORE ENFORCER
extern "C" void HandlerCoreEnforcer(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT); 

        for (u32 targetIdx = 0; targetIdx < targetCount; ++targetIdx) {
            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + targetIdx));

            if (targetSlot != BATTLE_MAX_SLOTS) {
                BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);

                if (BattleMon_GetTurnFlag(targetMon, TURNFLAG_MOVEPROCDONE)) {

                    HandlerParam_AddCondition *addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
                    addCondition->pokeID = targetSlot;
                    addCondition->condition = CONDITION_GASTROACID;
                    addCondition->condData = Condition_MakePermanent();
                    BattleHandler_StrSetup(&addCondition->exStr, 2u, 565);
                    BattleHandler_AddArg(&addCondition->exStr, targetSlot);
                    BattleHandler_PopWork(serverFlow, addCondition);
                }
            }
        }
    }
}

BattleEventHandlerTableEntry CoreEnforcerHandlers[] {
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerCoreEnforcer},
};

extern "C" BattleEventHandlerTableEntry* EventAddCoreEnforcer(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CoreEnforcerHandlers);
    return CoreEnforcerHandlers;
}

#endif // EXPAND_MOVES
