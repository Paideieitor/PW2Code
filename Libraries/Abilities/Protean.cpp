#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"
#include "server_events.h"

// PROTEAN
extern "C" void HandlerProtean(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
#if GEN9_PROTEAN
    if (*work != 0) {
        return;
    }
#endif
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

        MoveParam params;
        ServerEvent_GetMoveParam(serverFlow, BattleEventVar_GetValue(VAR_MOVE_ID), currentMon, &params);

        if (params.moveType != NULL_TYPE &&
            params.moveType != BattleMon_GetPokeType(currentMon)) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ChangeType* changeType;
            changeType = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
            changeType->pokeType = PokeTypePair_MakeMonotype(params.moveType);
            changeType->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, changeType);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
#if GEN9_PROTEAN
            * work = 1;
#endif
        }
    }
}
BattleEventHandlerTableEntry ProteanHandlers[]{
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtean},
};
extern "C" BattleEventHandlerTableEntry * EventAddProtean(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(ProteanHandlers);
    return ProteanHandlers;
}
#endif // EXPAND_ABILITIES