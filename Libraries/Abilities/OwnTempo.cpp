#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/AbilityExpansion.h"

#include "server_flow.h"

// OWN TEMPO
extern "C" void HandlerOwnTempoStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerOwnTempoAddStatusFailed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerOwnTempoCureStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerOwnTempoActionEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry OwnTempoHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerOwnTempoStatus},
    {EVENT_ADD_CONDITION_FAIL, HandlerOwnTempoAddStatusFailed},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerOwnTempoCureStatus},
    {EVENT_ACTION_PROCESSING_END, HandlerOwnTempoActionEnd},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerIgnoreIntimidate},
};
extern "C" BattleEventHandlerTableEntry * EventAddOwnTempoUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(OwnTempoHandlers);
    return OwnTempoHandlers;
}
#endif // EXPAND_ABILITIES