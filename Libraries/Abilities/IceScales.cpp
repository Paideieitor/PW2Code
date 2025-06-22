#include "settings.h"
#if EXPAND_ABILITIES
#endif // EXPAND_ABILITIES
#include "server_flow.h"

// ICE SCALES
extern "C" void HandlerIceScales(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry IceScalesHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerIceScales},
};
extern "C" BattleEventHandlerTableEntry * EventAddIceScales(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(IceScalesHandlers);
    return IceScalesHandlers;
}