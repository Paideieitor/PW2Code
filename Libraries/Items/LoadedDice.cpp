#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// LOADED DICE
extern "C" void HandlerLoadedDice(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 hitCount = BattleEventVar_GetValue(VAR_MAX_HIT_COUNT);
        if (hitCount == 5) {
            hitCount = 5;
            if (BattleRandom(100u) < 50)
                --hitCount;
        }
        BattleEventVar_RewriteValue(VAR_HIT_COUNT, hitCount);
        BattleEventVar_RewriteValue(VAR_AVOID_FLAG, 0);
    }
}
BattleEventHandlerTableEntry LoadedDiceHandlers[] = {
    {EVENT_MOVE_HIT_COUNT, HandlerLoadedDice},
};
extern "C" BattleEventHandlerTableEntry* EventAddLoadedDice(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(LoadedDiceHandlers);
    return LoadedDiceHandlers;
}
#endif // EXPAND_ITEMS