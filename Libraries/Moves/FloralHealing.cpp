#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"
#include "server_events.h"

// SHORE UP
extern "C" void HandlerFloralHealing(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 ratio = 2048;
        TERRAIN terrain = ServerEvent_GetTerrain(serverFlow);

        if (terrain == TERRAIN_GRASSY) {
            ratio = 2732;
        }
        BattleEventVar_RewriteValue(VAR_RATIO, ratio);
    }
}
BattleEventHandlerTableEntry FloralHealingHandlers[]{
    {EVENT_RECOVER_HP, HandlerFloralHealing},
};
extern "C" BattleEventHandlerTableEntry* EventAddFloralHealing(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FloralHealingHandlers);
    return FloralHealingHandlers;
}

#endif // EXPAND_MOVES
