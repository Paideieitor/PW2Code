#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// ROTOTILLER
extern "C" void HandlerRototiller(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (defendingSlot >= BATTLE_MAX_SLOTS)
            return;
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (!BattleMon_HasType(defendingMon, TYPE_GRASS) || 
            ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry RototillerHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerRototiller},
};
extern "C" BattleEventHandlerTableEntry* EventAddRototiller(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(RototillerHandlers);
    return RototillerHandlers;
}

#endif // EXPAND_MOVES