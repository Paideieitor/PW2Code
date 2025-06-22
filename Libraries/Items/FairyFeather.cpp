#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

extern "C" void CommonTypeBoostingItem(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, POKE_TYPE type);

// FAIRY FEATHER
extern "C" void HandlerFairyFeather(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    // TODO: ADD FAIRY TYPE
    CommonTypeBoostingItem(item, serverFlow, pokemonSlot, TYPE_GRASS);
}
BattleEventHandlerTableEntry FairyFeatherHandlers[] = {
    {EVENT_MOVE_POWER, HandlerFairyFeather},
};
extern "C" BattleEventHandlerTableEntry* EventAddFairyFeather(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FairyFeatherHandlers);
    return FairyFeatherHandlers;
}
#endif // EXPAND_ITEMS