#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// BULLET PROOF
extern "C" void HandlerBulletProof(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (getMoveFlag(moveID, FLAG_BULLET)) {
            if (BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1)) {
                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, 210);
                BattleHandler_AddArg(&message->str, pokemonSlot);
                BattleHandler_PopWork(serverFlow, message);

                BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            }
        }
    }
}
BattleEventHandlerTableEntry BulletProofHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerBulletProof},
};
extern "C" BattleEventHandlerTableEntry * EventAddBulletProof(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(BulletProofHandlers);
    return BulletProofHandlers;
}
#endif // EXPAND_ABILITIES