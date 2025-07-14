#include "settings.h"
#if EXPAND_ITEMS

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// TERRAIN EXTENDER
extern "C" void HandlerTerrainExtender(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON) && 
        BattleEventVar_GetValue(VAR_WEATHER) != TERRAIN_NULL) {
        u32 turnExtension = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        BattleEventVar_RewriteValue(VAR_EFFECT_TURN_COUNT, turnExtension);
    }
}
BattleEventHandlerTableEntry TerrainExtenderHandlers[] = {
    {EVENT_MOVE_TERRAIN_TURN_COUNT, HandlerTerrainExtender},
};
extern "C" BattleEventHandlerTableEntry * EventAddTerrainExtender(u32 * handleAmount) {
    *handleAmount = ARRAY_COUNT(TerrainExtenderHandlers);
    return TerrainExtenderHandlers;
}
#endif // EXPAND_ITEMS