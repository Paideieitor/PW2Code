#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// OVERCOAT
extern "C" void HandlerOvercoat(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerOvercoatPowderMoves(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_POWDER))
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
    }
}
BattleEventHandlerTableEntry OvercoatHandlers[]{
    {EVENT_WEATHER_REACTION, HandlerOvercoat},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerOvercoatPowderMoves},
};
extern "C" BattleEventHandlerTableEntry * EventAddOvercoatUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(OvercoatHandlers);
    return OvercoatHandlers;
}
#endif // EXPAND_ABILITIES