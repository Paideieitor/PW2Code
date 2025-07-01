#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// TRACE
extern "C" void HandlerTrace(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry TraceHandlers[]{
    {EVENT_SWITCH_IN, HandlerTrace},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerTrace},
};
extern "C" BattleEventHandlerTableEntry * EventAddTraceUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TraceHandlers);
    return TraceHandlers;
}
#endif // EXPAND_ABILITIES