#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Terrains.h"

#include "server_flow.h"

// ELECTRIC SURGE
extern "C" void HandlerElectricSurge(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        CommonTerrainChangeAbility(serverFlow, pokemonSlot, TERRAIN_ELECTRIC, BATTLE_ELECTRIC_TERRAIN_MSGID);
    }
}
BattleEventHandlerTableEntry ElectricSurgeHandlers[]{
    {EVENT_SWITCH_IN, HandlerElectricSurge},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerElectricSurge},
};
extern "C" BattleEventHandlerTableEntry * EventAddElectricSurge(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ElectricSurgeHandlers);
    return ElectricSurgeHandlers;
}
#endif // EXPAND_ABILITIES