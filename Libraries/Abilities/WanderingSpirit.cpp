#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// WANDERING SPIRIT
extern "C" void HandlerWanderingSpirit(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);

            PokeSet targetSet;
            targetSet.battleMon[0] = defendingMon;
            targetSet.count = 1;
            ServerDisplay_SkillSwap(serverFlow, attackingMon, &targetSet);
        }
    }
}
BattleEventHandlerTableEntry WanderingSpiritHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerWanderingSpirit},
};
extern "C" BattleEventHandlerTableEntry * EventAddWanderingSpirit(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(WanderingSpiritHandlers);
    return WanderingSpiritHandlers;
}
#endif // EXPAND_ABILITIES