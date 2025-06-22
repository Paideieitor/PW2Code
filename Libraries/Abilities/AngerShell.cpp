#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// ANGER SHELL
extern "C" void HandlerAngerShell(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
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
#if ANGER_SHELL_DEBUG
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

                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_DEFENSE;
                    statChange->volume = -1;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_SPECIAL_DEFENSE;
                    statChange->volume = -1;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_ATTACK;
                    statChange->volume = 2;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_SPECIAL_ATTACK;
                    statChange->volume = 2;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->stat = STATSTAGE_SPEED;
                    statChange->volume = 2;
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    BattleHandler_PopWork(serverFlow, statChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }

            BattleField_SetSubstituteDamage(pokemonSlot, 0);
        }
    }
}
BattleEventHandlerTableEntry AngerShellHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_2, HandlerAngerShell},
};
extern "C" BattleEventHandlerTableEntry * EventAddAngerShell(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AngerShellHandlers);
    return AngerShellHandlers;
}
#endif // EXPAND_ABILITIES