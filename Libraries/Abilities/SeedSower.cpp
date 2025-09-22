#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// SEED SOWER
extern "C" void HandlerSeedSower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        HandlerCommon_TerrainChange(serverFlow, pokemonSlot, TERRAIN_GRASSY, TERRAIN_ABILITY_TURNS, true, BATTLE_GRASSY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry SeedSowerHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerSeedSower},
};
extern "C" BattleEventHandlerTableEntry * EventAddSeedSower(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SeedSowerHandlers);
    return SeedSowerHandlers;
}
#endif // EXPAND_ABILITIES