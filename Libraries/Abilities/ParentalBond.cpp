#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// PARENTAL BOND
extern "C" void HandlerParentalBondPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && BattleField_CheckParentalBondFlag()) {
        *work += 1;
        u32 power = BattleEventVar_GetValue(VAR_MOVE_POWER);

        if (*work == 2) {
            *work = 0;
#if GEN6_PARENTAL_BOND
            power /= 2;
#else
            power /= 4;
#endif
        }

        BattleEventVar_RewriteValue(VAR_MOVE_POWER, power);
    }
}
BattleEventHandlerTableEntry ParentalBondHandlers[]{
    {EVENT_MOVE_POWER, HandlerParentalBondPower},
    // Double hit implementation in [ServerControl_DamageRoot]
};
extern "C" BattleEventHandlerTableEntry * EventAddParentalBond(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ParentalBondHandlers);
    return ParentalBondHandlers;
}
#endif // EXPAND_ABILITIES