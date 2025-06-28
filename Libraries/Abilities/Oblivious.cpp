#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/AbilityExpansion.h"

#include "server_flow.h"

// OBLIVIOUS
extern "C" void HandlerOblivious(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerAddStatusFailedCommon(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerObliviousCureStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerObliviousNoEffectCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerObliviousActionEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry ObliviousHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerOblivious},
    {EVENT_ADD_CONDITION_FAIL, HandlerAddStatusFailedCommon},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerObliviousCureStatus},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerObliviousNoEffectCheck},
    {EVENT_ACTION_PROCESSING_END, HandlerObliviousActionEnd},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerIgnoreIntimidate},
};
extern "C" BattleEventHandlerTableEntry * EventAddObliviousUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ObliviousHandlers);
    return ObliviousHandlers;
}
#endif // EXPAND_ABILITIES