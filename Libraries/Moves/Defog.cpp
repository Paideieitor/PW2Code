#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// DEFOG
extern "C" void HandlerDefog(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON))
    {
        u32 targetSlot = BattleEventVar_GetValue(VAR_TARGET_MON_ID);
        BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
        if (!BattleMon_IsSubstituteActive(targetMon))
        {
            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->stat = STATSTAGE_EVASION;
            statChange->volume = -1;
            statChange->moveAnimation = 1;
            statChange->pokeCount = 1;
            statChange->pokeID[0] = targetSlot;
            BattleHandler_PopWork(serverFlow, statChange);
        }

        HandlerParam_RemoveSideEffect* removeAllySide;
        removeAllySide = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeAllySide->side = GetSideFromMonID(pokemonSlot);
        REMOVE_SIDE_EFFECT_SETUP(removeAllySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_SPIKES, removeAllySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_TOXIC_SPIKES, removeAllySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_STEALTH_ROCK, removeAllySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_STICKY_WEB, removeAllySide->flags);
        BattleHandler_PopWork(serverFlow, removeAllySide);

        HandlerParam_RemoveSideEffect* removeEnemySide;
        removeEnemySide = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeEnemySide->side = GetSideFromMonID(targetSlot);
        REMOVE_SIDE_EFFECT_SETUP(removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_REFLECT, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_LIGHT_SCREEN, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_SAFEGUARD, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_MIST, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_SPIKES, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_TOXIC_SPIKES, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_STEALTH_ROCK, removeEnemySide->flags);
        SET_REMOVE_SIDE_EFFEC_FLAG(SIDEEFF_STICKY_WEB, removeEnemySide->flags);
        BattleHandler_PopWork(serverFlow, removeEnemySide);

        HandlerParam_RemoveFieldEffect* removeFieldEffect;
        removeFieldEffect = (HandlerParam_RemoveFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_FIELD_EFFECT, pokemonSlot);
        removeFieldEffect->effect = FLDEFF_TERRAIN;
        BattleHandler_PopWork(serverFlow, removeFieldEffect);
    }
}
extern "C" void HandlerBypassSubstitute(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
BattleEventHandlerTableEntry DefogHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerDefog},
    {EVENT_BYPASS_SUBSTITUTE, HandlerBypassSubstitute},
};
extern "C" BattleEventHandlerTableEntry* EventAddDefogUpdated(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DefogHandlers);
    return DefogHandlers;
}

#endif // EXPAND_MOVES