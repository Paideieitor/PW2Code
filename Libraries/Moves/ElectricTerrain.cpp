#include "settings.h"
#if EXPAND_MOVES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// ELECTRIC TERRAIN
extern "C" void HandlerElectricTerrain(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerCommon_TerrainChange(serverFlow, pokemonSlot, TERRAIN_ELECTRIC, 5, false, BATTLE_ELECTRIC_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry ElectricTerrainHandlers[]{
    {EVENT_CALL_FIELD_EFFECT, HandlerElectricTerrain},
};
extern "C" BattleEventHandlerTableEntry* EventAddElectricTerrain(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ElectricTerrainHandlers);
    return ElectricTerrainHandlers;
}
#endif // EXPAND_MOVES