#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// BERSERK
extern "C" void HandlerBerserk(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
    for (u8 i = 0; i < targetCount; ++i) {
        u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + i));
        if (pokemonSlot == targetSlot) {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

            u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
            u32 currentHPPercent = (currentHP * 100) / maxHP;

            u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE) - BattleField_GetSubstituteDamage(pokemonSlot);
            u32 beforeDmgHPPercent = (beforeDmgHP * 100) / maxHP;
#if BERSERK_DEBUG
            DPRINTF("MAX HP: %d\n", maxHP);
            DPRINTF("CURRENT HP: %d\n", currentHP);
            DPRINTF("DAMAGE: %d\n", BattleEventVar_GetValue(VAR_DAMAGE));
            DPRINTF("SUBSTITUTE DAMAGE: %d\n", BattleField_GetSubstituteDamage(pokemonSlot));
            DPRINTF("BEFORE HP: %d\n", beforeDmgHP);
            DPRINTF("BEFORE HP PERCENT: %d\n", beforeDmgHPPercent);
            DPRINTF("CURRENT HP PERCENT: %d\n", currentHPPercent);
#endif
            if (beforeDmgHPPercent >= 50)
            {
                if (currentHPPercent < 50)
                {
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_ChangeStatStage* stageChange;
                    stageChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    stageChange->pokeCount = 1;
                    stageChange->pokeID[0] = pokemonSlot;
                    stageChange->moveAnimation = 1;
                    stageChange->stat = STATSTAGE_SPECIAL_ATTACK;
                    stageChange->volume = 1;
                    BattleHandler_PopWork(serverFlow, stageChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }

            BattleField_SetSubstituteDamage(pokemonSlot, 0);
        }
    }
}
BattleEventHandlerTableEntry BerserkHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_2, HandlerBerserk},
};
extern "C" BattleEventHandlerTableEntry * EventAddBerserk(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BerserkHandlers);
    return BerserkHandlers;
}
#endif // EXPAND_ABILITIES