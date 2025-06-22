#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// SUPREME OVERLORD
// work[0] -> the amount to boost power
extern "C" u32 GetFaintedAlliesCount(ServerFlow * serverFlow, u32 pokemonSlot) {
    u32 count = 0;
    for (u8 i = 0; i < 24; ++i) {
        BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (affectedMon) {

            u32 affectedSlot = BattleMon_GetID(affectedMon);
            if (affectedSlot != pokemonSlot && MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                if (BattleMon_IsFainted(affectedMon)) {
                    // Add one to the count for each fainted ally
                    ++count;
                }
            }
        }
    }
    return count;
}
extern "C" void HandlerSupremeOverlordSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 faintedAllies = GetFaintedAlliesCount(serverFlow, pokemonSlot);
        u32 ratio = 0;
        for (u32 i = 0; i < faintedAllies; ++i) {
            ratio += 409;
        }
        work[0] = ratio;
    }
}
extern "C" void HandlerSupremeOverlordPower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) && work[0]) {
        BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4096 + work[0]);
    }
}
BattleEventHandlerTableEntry SupremeOverlordHandlers[]{
    {EVENT_SWITCH_IN, HandlerSupremeOverlordSwitchIn},
    {EVENT_MOVE_POWER, HandlerSupremeOverlordPower},
};
extern "C" BattleEventHandlerTableEntry * EventAddSupremeOverlord(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(SupremeOverlordHandlers);
    return SupremeOverlordHandlers;
}
#endif // EXPAND_ABILITIES