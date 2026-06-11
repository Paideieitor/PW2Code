#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

extern "C" void HandlerFeintBreakProtect(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
extern "C" void HandlerFeintTextSet(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
extern "C" void HandlerFeintResetProtectCounter(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);

// HYPERSPACE FURY
extern "C" void HandlerHyperspaceFuryFail(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        // TODO: Add Hoopa Unbound
        if (attackingMon->species != PKM_NULL && attackingMon->form == 1) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER);
        }
    }
}

BattleEventHandlerTableEntry HyperspaceFuryHandlers[] {
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerHyperspaceFuryFail},
    {EVENT_CHECK_PROTECT_BREAK, HandlerFeintBreakProtect},
    {EVENT_DETERMINE_MOVE_DAMAGE, HandlerFeintTextSet},
    {EVENT_BREAK_OPPONENT_GUARD, HandlerFeintResetProtectCounter},
};
extern "C" BattleEventHandlerTableEntry* EventAddHyperspaceFury(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HyperspaceFuryHandlers);
    return HyperspaceFuryHandlers;
}

#endif // EXPAND_MOVES
