#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// SCREEN CLEANER
extern "C" void HandlerScreenCleaner(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_RemoveSideEffect* removeAllySideEffect;
        removeAllySideEffect = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeAllySideEffect->side = 0;
        removeAllySideEffect->flags[0] = 3;
        removeAllySideEffect->flags[1] |= 1u;
        removeAllySideEffect->flags[1] |= 2u;
        BattleHandler_PopWork(serverFlow, removeAllySideEffect);

        HandlerParam_RemoveSideEffect* removeEnemySideEffect;
        removeEnemySideEffect = (HandlerParam_RemoveSideEffect*)BattleHandler_PushWork(serverFlow, EFFECT_REMOVE_SIDE_EFFECT, pokemonSlot);
        removeEnemySideEffect->side = 1;
        removeEnemySideEffect->flags[0] = 3;
        removeEnemySideEffect->flags[1] |= 1u;
        removeEnemySideEffect->flags[1] |= 2u;
        BattleHandler_PopWork(serverFlow, removeEnemySideEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
BattleEventHandlerTableEntry ScreenCleanerHandlers[]{
    {EVENT_SWITCH_IN, HandlerScreenCleaner},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerScreenCleaner},
};
extern "C" BattleEventHandlerTableEntry * EventAddScreenCleaner(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ScreenCleanerHandlers);
    return ScreenCleanerHandlers;
}
#endif // EXPAND_ABILITIES