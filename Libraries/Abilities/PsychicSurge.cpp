#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Terrains.h"

#include "server_flow.h"

// PSYCHIC SURGE
extern "C" void HandlerPsychicSurge(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_PSYCHIC, BATTLE_PSYCHIC_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry PsychicSurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerPsychicSurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerPsychicSurge},
};
extern "C" BattleEventHandlerTableEntry * EventAddPsychicSurge(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PsychicSurgeHandlers);
    return PsychicSurgeHandlers;
}
#endif // EXPAND_ABILITIES