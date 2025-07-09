#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// SAFETY GOGGLES
extern "C" void HandlerOvercoat(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerSafetyGogglesPowderMoves(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        pokemonSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_POWDER))
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
    }
}
BattleEventHandlerTableEntry SafetyGogglesHandlers[] = {
    {EVENT_WEATHER_REACTION, HandlerOvercoat},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerSafetyGogglesPowderMoves}, // rage powder not working is here -> HandlerRagePowderBaitTarget
};
extern "C" BattleEventHandlerTableEntry* EventAddSafetyGoggles(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SafetyGogglesHandlers);
    return SafetyGogglesHandlers;
}
#endif // EXPAND_ITEMS