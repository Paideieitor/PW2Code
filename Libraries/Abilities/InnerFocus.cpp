#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/AbilityExpansion.h"

#include "server_flow.h"

// INNER FOCUS
extern "C" void HandlerInnerFocus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry InnerFocusHandlers[]{
    {EVENT_FLINCH_CHECK, HandlerInnerFocus},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerIgnoreIntimidate},
};
extern "C" BattleEventHandlerTableEntry * EventAddInnerFocusUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(InnerFocusHandlers);
    return InnerFocusHandlers;
}
#endif // EXPAND_ABILITIES