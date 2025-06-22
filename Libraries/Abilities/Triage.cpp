#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// TRIAGE
MOVE_ID healingMoves[] = {
    MOVE_ABSORB,
    MOVE_MEGA_DRAIN,
    MOVE_RECOVER,
    MOVE_SOFT_BOILED,
    MOVE_DREAM_EATER,
    MOVE_LEECH_LIFE,
    MOVE_REST,
    MOVE_GIGA_DRAIN,
    MOVE_MILK_DRINK,
    MOVE_MORNING_SUN,
    MOVE_SYNTHESIS,
    MOVE_MOONLIGHT,
    MOVE_SWALLOW,
    MOVE_WISH,
    MOVE_SLACK_OFF,
    MOVE_ROOST,
    MOVE_HEALING_WISH,
    MOVE_DRAIN_PUNCH,
    MOVE_HEAL_ORDER,
    MOVE_LUNAR_DANCE,
    MOVE_HEAL_PULSE,
    MOVE_HORN_LEECH,
#if EXPAND_MOVES
    STRENGTH_SAP,
    SHORE_UP,
    REVIVAL_BLESSING,
    PURIFY,
    PARABOLIC_CHARGE,
    OBLIVION_WING,
    MATCHA_GOTCHA,
    LUNAR_BLESSING,
    BITTER_BLADE,
    DRAINING_KISS,
    FLORAL_HEALING,
#endif

};
extern "C" b32 IsHealingMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(healingMoves); ++i) {
        if (moveID == healingMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerTriage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        int moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsHealingMove(moveID)) {
            BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 3);
        }
    }
}
BattleEventHandlerTableEntry TriageHandlers[]{
    {EVENT_GET_MOVE_PRIORITY, HandlerTriage},
};
extern "C" BattleEventHandlerTableEntry * EventAddTriage(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TriageHandlers);
    return TriageHandlers;
}
#endif // EXPAND_ABILITIES