#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// RAPID SPIN
extern "C" void HandlerRapidSpin(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_LEECHSEED)) {
            HandlerParam_CureCondition* cureLeechSeed;
            cureLeechSeed = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            cureLeechSeed->pokeCount = 1;
            cureLeechSeed->pokeID[0] = pokemonSlot;
            cureLeechSeed->condition = CONDITION_LEECHSEED;
            BattleHandler_PopWork(serverFlow, cureLeechSeed);
        }

        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_BIND)) {
            HandlerParam_CureCondition* cureBind;
            cureBind = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            cureBind->pokeCount = 1;
            cureBind->pokeID[0] = pokemonSlot;
            cureBind->condition = CONDITION_BIND;
            BattleHandler_PopWork(serverFlow, cureBind);
        }

        HandlerParam_RemoveSideEffect* removeSideEffect;
        removeSideEffect = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeSideEffect->side = GetSideFromMonID(pokemonSlot);
        REMOVE_SIDE_EFFECT_SETUP(removeSideEffect->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_SPIKES, removeSideEffect->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_TOXIC_SPIKES, removeSideEffect->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_STEALTH_ROCK, removeSideEffect->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_STICKY_WEB, removeSideEffect->flags);
        BattleHandler_PopWork(serverFlow, removeSideEffect);
    }
}
BattleEventHandlerTableEntry RapidSpinHandlers[]{
    {EVENT_MOVE_DAMAGE_SIDE_AFTER, HandlerRapidSpin},
};
extern "C" BattleEventHandlerTableEntry* EventAddRapidSpinUpdated(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RapidSpinHandlers);
    return RapidSpinHandlers;
}

#endif // EXPAND_MOVES