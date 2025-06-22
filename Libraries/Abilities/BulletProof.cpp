#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// BULLET PROOF
MOVE_ID BulletMoves[] = {
    MOVE_EGG_BOMB,
    MOVE_BARRAGE,
    MOVE_SLUDGE_BOMB,
    MOVE_OCTAZOOKA,
    MOVE_ZAP_CANNON,
    MOVE_SHADOW_BALL,
    MOVE_MIST_BALL,
    MOVE_ICE_BALL,
    MOVE_WEATHER_BALL,
    MOVE_BULLET_SEED,
    MOVE_ROCK_BLAST,
    MOVE_GYRO_BALL,
    MOVE_AURA_SPHERE,
    MOVE_SEED_BOMB,
    MOVE_FOCUS_BLAST,
    MOVE_ENERGY_BALL,
    MOVE_MUD_BOMB,
    MOVE_ROCK_WRECKER,
    MOVE_MAGNET_BOMB,
    MOVE_ELECTRO_BALL,
    MOVE_ACID_SPRAY,
    MOVE_SEARING_SHOT,
#if EXPAND_MOVES
    POLLEN_PUFF,
    BEAK_BLAST,
    PYRO_BALL,
    SYRUP_BOMB,
#endif
};
extern "C" b32 IsBulletMove(MOVE_ID moveID) {
    for (u16 i = 0; i < ARRAY_COUNT(BulletMoves); ++i) {
        if (moveID == BulletMoves[i]) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerBulletProof(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (IsBulletMove(moveID)) {
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