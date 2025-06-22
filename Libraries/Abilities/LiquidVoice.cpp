#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// LIQUID VOICE
extern "C" void HandlerLiquidVoice(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_SOUND)) {
        BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_WATER);
    }
}
BattleEventHandlerTableEntry LiquidVoiceHandlers[]{
    {EVENT_MOVE_PARAM, HandlerLiquidVoice},
};
extern "C" BattleEventHandlerTableEntry * EventAddLiquidVoice(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(LiquidVoiceHandlers);
    return LiquidVoiceHandlers;
}
#endif // EXPAND_ABILITIES