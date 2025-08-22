#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// TOXIC DEBRIS
extern "C" void HandlerToxicDebris(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {
        ConditionData condData = Condition_MakePermanent();
        u8 opposingSide = GetSideFromOpposingMonID(pokemonSlot);

        HandlerParam_AddSideEffect* addSideEffect;
        addSideEffect = (HandlerParam_AddSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_SIDE_EFFECT, pokemonSlot);
        addSideEffect->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        addSideEffect->sideEffect = SIDEEFF_TOXIC_SPIKES;
        addSideEffect->side = opposingSide;
        addSideEffect->condData = condData;
        BattleHandler_StrSetup(&addSideEffect->exStr, 1u, 152u);
        BattleHandler_AddArg(&addSideEffect->exStr, opposingSide);
        BattleHandler_PopWork(serverFlow, addSideEffect);
    }
}
BattleEventHandlerTableEntry ToxicDebrisHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerToxicDebris},
};
extern "C" BattleEventHandlerTableEntry * EventAddToxicDebris(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ToxicDebrisHandlers);
    return ToxicDebrisHandlers;
}
#endif // EXPAND_ABILITIES