#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// MEGA LAUNCHER
extern "C" void HandlerMegaLauncherDamageBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_PULSE)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 6144);
        }
    }
}
extern "C" void HandlerMegaLauncherHealBoost(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_PULSE)) {
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