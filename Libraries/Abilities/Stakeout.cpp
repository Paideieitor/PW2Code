#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// STAKEOUT
extern "C" void HandlerStakeout(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);

        if (SwitchedInThisTurn(serverFlow, defendingMon)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 8192);
        }
    }
}
BattleEventHandlerTableEntry StakeoutHandlers[]{
    {EVENT_MOVE_POWER, HandlerStakeout},
};
extern "C" BattleEventHandlerTableEntry * EventAddStakeout(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(StakeoutHandlers);
    return StakeoutHandlers;
}
#endif // EXPAND_ABILITIES