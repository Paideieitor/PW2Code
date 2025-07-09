#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

#include "types.h"

extern "C" void CommonResistBerry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 pokeType, b32 skipEffectivenessCheck);
extern "C" void HandlerCommonResistBerryDamageAfter(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);

// ROSELI BERRY
extern "C" void HandlerRoseliBerry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work)
{
    CommonResistBerry(item, serverFlow, pokemonSlot, work, TYPE_FAIRY, 0);
}
BattleEventHandlerTableEntry RoseliBerryHandlers[] = {
{EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerRoseliBerry},
{EVENT_AFTER_DAMAGE_REACTION, HandlerCommonResistBerryDamageAfter},
};
extern "C" BattleEventHandlerTableEntry* EventAddRoseliBerry(u32* handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(RoseliBerryHandlers);
    return RoseliBerryHandlers;
}
#endif // EXPAND_ITEMS