#include "settings.h"
#if EXPAND_MOVES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// GRASSY TERRAIN
extern "C" void HandlerGrassyTerrain(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerCommon_TerrainChange(serverFlow, pokemonSlot, TERRAIN_GRASSY, 5, false, BATTLE_GRASSY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry GrassyTerrainHandlers[]{
    {EVENT_CALL_FIELD_EFFECT, HandlerGrassyTerrain},
};
extern "C" BattleEventHandlerTableEntry* EventAddGrassyTerrain(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GrassyTerrainHandlers);
    return GrassyTerrainHandlers;
}
#endif // EXPAND_MOVES