#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// VENOM DRENCH
extern "C" void HandlerVenomDrench(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (defendingSlot >= BATTLE_MAX_SLOTS)
            return;
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (BattleMon_GetStatus(defendingMon) != CONDITION_POISON) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry VenomDrenchHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerVenomDrench},
};
extern "C" BattleEventHandlerTableEntry* EventAddVenomDrench(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(VenomDrenchHandlers);
    return VenomDrenchHandlers;
}

#endif // EXPAND_MOVES
