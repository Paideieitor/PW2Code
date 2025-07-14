#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// SURGE SURFER
extern "C" void HandlerSurgeSurfer(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC) {
        BattleEventVar_MulValue(VAR_RATIO, 8192);
    }
}
BattleEventHandlerTableEntry SurgeSurferHandlers[]{
    {EVENT_CALC_SPEED, HandlerSurgeSurfer},
};
extern "C" BattleEventHandlerTableEntry * EventAddSurgeSurfer(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SurgeSurferHandlers);
    return SurgeSurferHandlers;
}
#endif // EXPAND_ABILITIES