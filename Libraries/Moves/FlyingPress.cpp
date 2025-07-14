#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// FLYING PRESS
extern "C" void HandlerFlyingPress(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        
        TypeEffectiveness effectiveness = GetTypeEffectiveness(TYPE_FLY, BattleEventVar_GetValue(VAR_POKE_TYPE));
        BattleEventVar_RewriteValue(VAR_SET_TYPE_EFFECTIVENESS, COMPOUND_EFFECTIVENESS + effectiveness);
    }
}
BattleEventHandlerTableEntry FlyingPressHandlers[]{
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerFlyingPress},
};
extern "C" BattleEventHandlerTableEntry * EventAddFlyingPress(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FlyingPressHandlers);
    return FlyingPressHandlers;
}

#endif // EXPAND_MOVES