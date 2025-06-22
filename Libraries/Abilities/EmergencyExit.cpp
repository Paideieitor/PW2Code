#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"
#include "Patches/BattleUpgrade/include/ActionOrderManager.h"

#include "server_flow.h"

// EMERGENCY EXIT
extern "C" void CommonEmergencyExitCheck(ServerFlow * serverFlow, u32 currentSlot) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
    u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

    u32 currentHP = BattleMon_GetValue(currentMon, VALUE_CURRENT_HP);
    u32 currentHPPercent = (currentHP * 100) / maxHP;

    u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE) - BattleField_GetSubstituteDamage(currentSlot);
    u32 beforeDmgHPPercent = (beforeDmgHP * 100) / maxHP;
#if EMERGENCY_EXIT_DEBUG
    DPRINTF("MAX HP: %d \n", maxHP);
    DPRINTF("CURRENT HP: %d \n", currentHP);
    DPRINTF("DAMAGE: %d \n", BattleEventVar_GetValue(VAR_DAMAGE));
    DPRINTF("SUBSTITUTE DAMAGE: %d \n", BattleField_GetSubstituteDamage(currentSlot));
    DPRINTF("BEFORE HP: %d \n", beforeDmgHP);
    DPRINTF("BEFORE HP PERCENT: %d \n", beforeDmgHPPercent);
    DPRINTF("CURRENT HP PERCENT: %d \n", currentHPPercent);
#endif
    if (beforeDmgHPPercent >= 50 &&
        currentHPPercent < 50) {
        BattleField_SetEmergencyExitFlag(currentSlot);
    }

    BattleField_SetSubstituteDamage(currentSlot, 0);
}
extern "C" void HandlerEmergencyExitDamageCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
    for (u32 target = 0; target < targetCount; ++target) {

        u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + target));
        if (pokemonSlot == targetSlot) {
            CommonEmergencyExitCheck(serverFlow, pokemonSlot);

            if (BattleField_CheckEmergencyExitFlag(pokemonSlot)) {
                BattleField_ResetEmergencyExitFlag(pokemonSlot);
#if EMERGENCY_EXIT_DEBUG
                DPRINTF("EE SWITCH -> SLOT: %d\n", pokemonSlot);
#endif
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_Switch* switchOut;
                switchOut = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
                switchOut->pokeID = pokemonSlot;
                BattleHandler_PopWork(serverFlow, switchOut);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
extern "C" void HandlerEmergencyExitSimpleCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
#if EMERGENCY_EXIT_DEBUG
        DPRINTF("EE SIMPLE -> SLOT: %d\n", pokemonSlot);
#endif
        CommonEmergencyExitCheck(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerEmergencyExitSwitchEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (BattleField_CheckEmergencyExitFlag(pokemonSlot)) {
        BattleField_ResetEmergencyExitFlag(pokemonSlot);
#if EMERGENCY_EXIT_DEBUG
        DPRINTF("EE SWITCH END -> SLOT: %d\n", pokemonSlot);
#endif
        if (Handler_GetFightEnableBenchPokeNum(serverFlow, pokemonSlot)
            && Handler_CheckReservedMemberChangeAction(serverFlow)) {

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_Switch* switchOut;
            switchOut = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
            switchOut->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, switchOut);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

            // Set the flag to switch after a turn has ended
            SetEndTurnSwitchFlag();
        }
    }
}
BattleEventHandlerTableEntry EmergencyExitHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_2, HandlerEmergencyExitDamageCheck},
    {EVENT_SIMPLE_DAMAGE_REACTION, HandlerEmergencyExitSimpleCheck},
    {EVENT_TURN_CHECK_END, HandlerEmergencyExitSwitchEnd},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmergencyExit(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(EmergencyExitHandlers);
    return EmergencyExitHandlers;
}
#endif // EXPAND_ABILITIES