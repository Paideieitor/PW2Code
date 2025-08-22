#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/MoveExpansion.h"

#include "server_flow.h"

// MAT BLOCK
extern "C" void HandlerMatBlock(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u8 allies[3];
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        GetSideBattleSlots(serverFlow, BattleMon_GetID(currentMon), allies);

        for (u32 allyIdx = 0; allyIdx < 3; ++allyIdx) {
            u32 battleSlot = allies[allyIdx];
            if (battleSlot == BATTLE_MAX_SLOTS)
                break;

            HandlerParam_SetTurnFlag* setTurnFlag;
            setTurnFlag = (HandlerParam_SetTurnFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_TURN_FLAG, battleSlot);
            setTurnFlag->pokeID = battleSlot;
            setTurnFlag->flag = TURNFLAG_PROTECT;
            BattleHandler_PopWork(serverFlow, setTurnFlag);
        }

        IncrementProtectCounter(serverFlow, pokemonSlot, 0);
        
        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_MAT_BLOCK_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);
    }
}
BattleEventHandlerTableEntry MatBlockHandlers[]{
    {EVENT_MOVE_SEQUENCE_START, HandlerProtectStart},
    {EVENT_MOVE_EXECUTE_CHECK2, HandlerProtectCheckFail },
    {EVENT_MOVE_EXECUTE_FAIL, HandlerProtectResetCounter},
    {EVENT_UNCATEGORIZED_MOVE, HandlerMatBlock},
};
extern "C" BattleEventHandlerTableEntry * EventAddMatBlock(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MatBlockHandlers);
    return MatBlockHandlers;
}

#endif // EXPAND_MOVES