#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// UNSEEN FIST
// work[0] -> 1 if the attack went through protect, 0 otherwise
extern "C" void HandlerUnseenFistBreakProtect(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
#if CHAMPIONS_UNSEEN_FIST
    work[0] = 0;
#endif
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (IsContact(serverFlow, moveID, pokemonSlot)) {
            BattleEventVar_RewriteValue(VAR_GENERAL_USE_FLAG, 2); // Value of 2 ingnores protect without breaking it
#if CHAMPIONS_UNSEEN_FIST
            if (BattleEventVar_GetValue(VAR_NO_EFFECT_FLAG)) {
                work[0] = 1;
            }
#endif
        }
    }
}
#if CHAMPIONS_UNSEEN_FIST
extern "C" void HandlerUnseenFistDamage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[0] == 1 && pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        work[0] = 0;
        BattleEventVar_MulValue(VAR_RATIO, 1024);
    }
}
#endif
BattleEventHandlerTableEntry UnseenFistHandlers[]{
    {EVENT_CHECK_PROTECT_BREAK, HandlerUnseenFistBreakProtect},
#if CHAMPIONS_UNSEEN_FIST
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerUnseenFistDamage},
#endif
};
extern "C" BattleEventHandlerTableEntry * EventAddUnseenFist(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(UnseenFistHandlers);
    return UnseenFistHandlers;
}
#endif // EXPAND_ABILITIES
