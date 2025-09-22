#include "settings.h"
#if EXPAND_MOVES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// MISTY TERRAIN
extern "C" void HandlerMistyTerrain(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerCommon_TerrainChange(serverFlow, pokemonSlot, TERRAIN_MISTY, 5, false, BATTLE_MISTY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry MistyTerrainHandlers[]{
    {EVENT_CALL_FIELD_EFFECT, HandlerMistyTerrain},
};
extern "C" BattleEventHandlerTableEntry* EventAddMistyTerrain(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MistyTerrainHandlers);
    return MistyTerrainHandlers;
}
#endif // EXPAND_MOVES