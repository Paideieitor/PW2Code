#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// MISTY SURGE
extern "C" void HandlerMistySurge(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerCommon_TerrainChange(serverFlow, pokemonSlot, TERRAIN_MISTY, TERRAIN_ABILITY_TURNS, true, BATTLE_MISTY_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry MistySurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerMistySurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerMistySurge},
};
extern "C" BattleEventHandlerTableEntry * EventAddMistySurge(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MistySurgeHandlers);
    return MistySurgeHandlers;
}
#endif // EXPAND_ABILITIES