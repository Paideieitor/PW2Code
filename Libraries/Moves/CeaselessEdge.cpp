#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// CEASELESS EDGE
extern "C" void HandlerCeaselessEdge(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        ConditionData condData = Condition_MakePermanent();
        u8 opposingSide = GetSideFromOpposingMonID(pokemonSlot);

        HandlerParam_AddSideEffect* addSideEffect;
        addSideEffect = (HandlerParam_AddSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_SIDE_EFFECT, pokemonSlot);
        addSideEffect->header.flags |= SIDE_HANDLER_MOVE_POPUP_FLAG;
        addSideEffect->sideEffect = SIDEEFF_SPIKES;
        addSideEffect->side = opposingSide;
        addSideEffect->condData = condData;
        BattleHandler_StrSetup(&addSideEffect->exStr, 1u, 148u);
        BattleHandler_AddArg(&addSideEffect->exStr, opposingSide);
        BattleHandler_PopWork(serverFlow, addSideEffect);
    }
}
BattleEventHandlerTableEntry CeaselessEdgeHandlers[]{
    {EVENT_DAMAGE_PROCESSING_END_HIT_REAL, HandlerCeaselessEdge},
};
extern "C" BattleEventHandlerTableEntry * EventAddCeaselessEdge(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CeaselessEdgeHandlers);
    return CeaselessEdgeHandlers;
}

#endif // EXPAND_MOVES