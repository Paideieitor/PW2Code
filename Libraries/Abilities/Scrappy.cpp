#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/AbilityExpansion.h"

#include "server_flow.h"

// SCRAPPY
extern "C" void HandlerScrappy(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry ScrappyHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerScrappy},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerIgnoreIntimidate},
};
extern "C" BattleEventHandlerTableEntry * EventAddScrappyUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ScrappyHandlers);
    return ScrappyHandlers;
}
#endif // EXPAND_ABILITIES