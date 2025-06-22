#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// GORILLA TACTICS
extern "C" void HandlerChoiceItemCommonMoveLock(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerChoiceBandPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerChoiceItemCommonItemChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
BattleEventHandlerTableEntry GorillaTacticsHandlers[]{
    {EVENT_CHOOSE_MOVE, HandlerChoiceItemCommonMoveLock},
    {EVENT_ATTACKER_POWER, HandlerChoiceBandPower},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerChoiceItemCommonItemChange},
    {EVENT_ABILITY_NULLIFIED, HandlerChoiceItemCommonItemChange},
};
extern "C" BattleEventHandlerTableEntry * EventAddGorillaTactics(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GorillaTacticsHandlers);
    return GorillaTacticsHandlers;
}
#endif // EXPAND_ABILITIES