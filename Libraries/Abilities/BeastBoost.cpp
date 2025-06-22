#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// BEAST BOOST
extern "C" StatStage GetHighestStat(BattleMon * battleMon) {
    u16 highest = battleMon->attack;
    StatStage output = STATSTAGE_ATTACK;

    if (highest < battleMon->defense) {
        highest = battleMon->defense;
        output = STATSTAGE_DEFENSE;
    }
    if (highest < battleMon->specialAttack) {
        highest = battleMon->specialAttack;
        output = STATSTAGE_SPECIAL_ATTACK;
    }
    if (highest < battleMon->specialDefense) {
        highest = battleMon->specialDefense;
        output = STATSTAGE_SPECIAL_DEFENSE;
    }
    if (highest < battleMon->speed) {
        highest = battleMon->speed;
        output = STATSTAGE_SPEED;
    }

    return output;
}
extern "C" void HandlerBeastBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 target = 0; target < targetCount; ++target) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + target));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (BattleMon_IsFainted(targetMon)) {
                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->stat = GetHighestStat(attackingMon);
                statChange->volume = 1;
                BattleHandler_PopWork(serverFlow, statChange);
            }
        }
    }
}
BattleEventHandlerTableEntry BeastBoostHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerBeastBoost},
};
extern "C" BattleEventHandlerTableEntry * EventAddBeastBoost(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BeastBoostHandlers);
    return BeastBoostHandlers;
}
#endif // EXPAND_ABILITIES