#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// GALE WINGS
extern "C" void HandlerGaleWings(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
#if GEN6_GALE_WINGS
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFullHP(attackingMon))
#endif
        {
            MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            if (moveID) {
                if (PML_MoveGetType(moveID) == TYPE_FLY) {
                    BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 1);
                }

                // This code makes Gale Wings work with moves that turn Flying Type like Hidden Power
                /*
                MoveParam params;
                if (MoveEvent_AddItem(attackingMon, moveID, 0)) {
                    ServerEvent_GetMoveParam(serverFlow, moveID, attackingMon, &params);
                    MoveEvent_RemoveItem(attackingMon, moveID);
                }

                if (params.moveType == TYPE_FLY) {
                    BattleEventVar_RewriteValue(VAR_MOVE_PRIORITY, BattleEventVar_GetValue(VAR_MOVE_PRIORITY) + 1);
                }
                */
            }
        }
    }
}
BattleEventHandlerTableEntry GaleWingsHandlers[]{
    {EVENT_GET_MOVE_PRIORITY, HandlerGaleWings},
};
extern "C" BattleEventHandlerTableEntry * EventAddGaleWings(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(GaleWingsHandlers);
    return GaleWingsHandlers;
}
#endif // EXPAND_ABILITIES