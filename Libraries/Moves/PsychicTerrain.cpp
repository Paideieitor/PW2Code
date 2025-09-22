#include "settings.h"
#if EXPAND_MOVES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// PSYCHIC TERRAIN
extern "C" void HandlerPsychicTerrain(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        HandlerCommon_TerrainChange(serverFlow, pokemonSlot, TERRAIN_PSYCHIC, 5, false, BATTLE_PSYCHIC_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry PsychicTerrainHandlers[]{
    {EVENT_CALL_FIELD_EFFECT, HandlerPsychicTerrain},
};
extern "C" BattleEventHandlerTableEntry* EventAddPsychicTerrain(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PsychicTerrainHandlers);
    return PsychicTerrainHandlers;
}
#endif // EXPAND_MOVES