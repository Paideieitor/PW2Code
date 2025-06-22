#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// STAMINA
extern "C" void HandlerStamina(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(defendingMon, STATSTAGE_DEFENSE, 1)) {
            if (!BattleMon_IsFainted(defendingMon)) {
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_ChangeStatStage* statChange;
                statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statChange->pokeCount = 1;
                statChange->pokeID[0] = pokemonSlot;
                statChange->moveAnimation = 1;
                statChange->stat = STATSTAGE_DEFENSE;
                statChange->volume = 1;
                BattleHandler_PopWork(serverFlow, statChange);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry StaminaHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerStamina},
};
extern "C" BattleEventHandlerTableEntry * EventAddStamina(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StaminaHandlers);
    return StaminaHandlers;
}
#endif // EXPAND_ABILITIES