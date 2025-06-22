#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// MEGA LAUNCHER
MOVE_ID PulseMoves[] = {
    MOVE_WATER_PULSE,
    MOVE_AURA_SPHERE,
    MOVE_DARK_PULSE,
    MOVE_DRAGON_PULSE,
    MOVE_HEAL_PULSE,
#if EXPAND_MOVES
    ORIGIN_PULSE,
    TERRAIN_PULSE,
#endif
};
extern "C" b32 IsPulseMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(PulseMoves); ++i) {
        if (moveID == PulseMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerMegaLauncherDamageBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (IsPulseMove(BattleEventVar_GetValue(VAR_MOVE_ID))) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
extern "C" void HandlerMegaLauncherHealBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (IsPulseMove(BattleEventVar_GetValue(VAR_MOVE_ID))) {
            BattleEventVar_RewriteValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry MegaLauncherHandlers[]{
    {EVENT_MOVE_POWER, HandlerMegaLauncherDamageBoost},
    {EVENT_RECOVER_HP, HandlerMegaLauncherHealBoost},
};
extern "C" BattleEventHandlerTableEntry * EventAddMegaLauncher(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MegaLauncherHandlers);
    return MegaLauncherHandlers;
}
#endif // EXPAND_ABILITIES