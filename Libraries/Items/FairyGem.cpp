#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

extern "C" void CommonGemDecide(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 type);
extern "C" void CommonGemPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 type);

// FAIRY GEM
extern "C" void HandlerFairyGemDecide(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
	// TODO: ADD FAIRY TYPE
	CommonGemDecide(item, serverFlow, pokemonSlot, work, TYPE_GRASS);
}
extern "C" void HandlerFairyGemPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
	// TODO: ADD FAIRY TYPE
	CommonGemPower(item, serverFlow, pokemonSlot, work, TYPE_GRASS);
}
extern "C" void HandlerGemEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work);
BattleEventHandlerTableEntry FairyGemHandlers[] = {
	{EVENT_DAMAGE_PROCESSING_START, HandlerFairyGemDecide},
	{EVENT_MOVE_POWER, HandlerFairyGemPower},
	{EVENT_DAMAGE_PROCESSING_END, HandlerGemEnd},
};
extern "C" BattleEventHandlerTableEntry* EventAddFairyGem(u32* handlerAmount) {
	*handlerAmount = ARRAY_COUNT(FairyGemHandlers);
	return FairyGemHandlers;
}
#endif // EXPAND_ITEMS