#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// FELL STINGER
extern "C" void HandlerFellStinger(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 idx = 0; idx < targetCount; ++idx)
        {
            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + idx));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon))
            {
                u32 statsToMax = BattleMon_GetStatStageVolumeToMax(attackingMon, STATSTAGE_ATTACK);
                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->stat = STATSTAGE_ATTACK;
                statChange->volume = statsToMax;
                BattleHandler_PopWork(serverFlow, statChange);

                return;
            }
        }
    }
}
BattleEventHandlerTableEntry FellStingerHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerFellStinger},
};
extern "C" BattleEventHandlerTableEntry* EventAddFellStinger(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FellStingerHandlers);
    return FellStingerHandlers;
}

#endif // EXPAND_MOVES