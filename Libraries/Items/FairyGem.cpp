#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

extern "C" void CommonGemDecide(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 type);
extern "C" void CommonGemPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 type);

// FAIRY GEM
extern "C" void HandlerFairyGemDecide(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
	CommonGemDecide(item, serverFlow, pokemonSlot, work, TYPE_FAIRY);
}
extern "C" void HandlerFairyGemPower(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
	CommonGemPower(item, serverFlow, pokemonSlot, work, TYPE_FAIRY);
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