#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "abilities.h"
#include "server_flow.h"

// MAGNETIC FLUX
extern "C" void HandlerMagneticFlux(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (defendingSlot >= BATTLE_MAX_SLOTS)
            return;
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        ABILITY ability = BattleMon_GetValue(defendingMon, VALUE_ABILITY);
        if (ability != ABIL_PLUS && ability != ABIL_MINUS) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);
        }
    }
}
BattleEventHandlerTableEntry MagneticFluxHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerMagneticFlux},
};
extern "C" BattleEventHandlerTableEntry* EventAddMagneticFlux(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MagneticFluxHandlers);
    return MagneticFluxHandlers;
}

#endif // EXPAND_MOVES
