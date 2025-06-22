#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// SHARPNESS
MOVE_ID sharpMoves[]{
    MOVE_CUT,
    MOVE_RAZOR_LEAF,
    MOVE_SLASH,
    MOVE_FURY_CUTTER,
    MOVE_AIR_CUTTER,
    MOVE_AERIAL_ACE,
    MOVE_LEAF_BLADE,
    MOVE_NIGHT_SLASH,
    MOVE_AIR_SLASH,
    MOVE_X_SCISSOR,
    MOVE_PSYCHO_CUT,
    MOVE_CROSS_POISON,
    MOVE_SACRED_SWORD,
    MOVE_RAZOR_SHELL,
    MOVE_SECRET_SWORD,
#if EXPAND_MOVES
    AQUA_CUTTER,
    CEASELESS_EDGE,
    SOLAR_BLADE,
    STONE_AXE,
    BEHEMOTH_BLADE,
    BITTER_BLADE,
    KOWTOW_CLEAVE,
    MIGHTY_CLEAVE,
    POPULATION_BOMB,
    PSYBLADE,
    TACHYON_CUTTER,
#endif
};
extern "C" b32 IsSharpMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(sharpMoves); ++i) {
        if (moveID == sharpMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerSharpness(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsSharpMove(moveID)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
BattleEventHandlerTableEntry SharpnessHandlers[]{
    {EVENT_MOVE_POWER, HandlerSharpness},
};
extern "C" BattleEventHandlerTableEntry * EventAddSharpness(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SharpnessHandlers);
    return SharpnessHandlers;
}
#endif // EXPAND_ABILITIES