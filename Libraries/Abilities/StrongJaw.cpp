#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// STRONG JAW
MOVE_ID BiteMoves[] = {
    MOVE_BITE,
    MOVE_HYPER_FANG,
    MOVE_CRUNCH,
    MOVE_POISON_FANG,
    MOVE_THUNDER_FANG,
    MOVE_ICE_FANG,
    MOVE_FIRE_FANG,
#if EXPAND_MOVES
    FISHIOUS_REND,
    JAW_LOCK,
    PSYCHIC_FANG,
#endif
};
extern "C" b32 IsBiteMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(BiteMoves); ++i) {
        if (moveID == BiteMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerStrongJaw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsBiteMove(moveID)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry StrongJawHandlers[]{
    {EVENT_MOVE_POWER, HandlerStrongJaw},
};
extern "C" BattleEventHandlerTableEntry * EventAddStrongJaw(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(StrongJawHandlers);
    return StrongJawHandlers;
}
#endif // EXPAND_ABILITIES