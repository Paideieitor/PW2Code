#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Terrains.h"

#include "server_flow.h"

// GRASS PELT
extern "C" void HandlerGrassPelt(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        if (ServerEvent_GetTerrain(serverFlow) == TERRAIN_GRASSY) {
            if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {
                BattleEventVar_MulValue(VAR_RATIO, 6144);
            }
        }
    }
}
BattleEventHandlerTableEntry GrassPeltHandlers[]{
    {EVENT_DEFENDER_GUARD, HandlerGrassPelt},
};
extern "C" BattleEventHandlerTableEntry * EventAddGrassPelt(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(GrassPeltHandlers);
    return GrassPeltHandlers;
}
#endif // EXPAND_ABILITIES