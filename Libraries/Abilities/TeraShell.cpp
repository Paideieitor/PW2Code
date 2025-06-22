#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// TERA SHELL
extern "C" void HandlerTeraShell(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFullHP(currentMon)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            BattleEventVar_RewriteValue(VAR_SET_TYPE_EFFECTIVENESS, 2);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry TeraShellHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerTeraShell},
};
extern "C" BattleEventHandlerTableEntry * EventAddTeraShell(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TeraShellHandlers);
    return TeraShellHandlers;
}
#endif // EXPAND_ABILITIES