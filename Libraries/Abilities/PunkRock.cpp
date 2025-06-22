#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// PUNK ROCK
extern "C" void HandlerPunkRockPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_SOUND)) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5461);
        }
    }
}
extern "C" void HandlerPunkRockDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(BattleEventVar_GetValue(VAR_MOVE_ID), FLAG_SOUND)) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry PunkRockHandlers[]{
    {EVENT_MOVE_POWER, HandlerPunkRockPower},
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerPunkRockDamage},
};
extern "C" BattleEventHandlerTableEntry * EventAddPunkRock(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(PunkRockHandlers);
    return PunkRockHandlers;
}
#endif // EXPAND_ABILITIES