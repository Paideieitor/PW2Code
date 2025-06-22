#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Terrains.h"

#include "server_flow.h"

// GRASSY SURGE
extern "C" void HandlerGrassySurge(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_GRASSY, BATTLE_GRASSY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry GrassySurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerGrassySurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerGrassySurge},
};
extern "C" BattleEventHandlerTableEntry * EventAddGrassySurge(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GrassySurgeHandlers);
    return GrassySurgeHandlers;
}
#endif // EXPAND_ABILITIES