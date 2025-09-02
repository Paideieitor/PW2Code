#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// PARTING SHOT
// work[0] -> True if Parting Shot should switch the user
extern "C" void HandlerPartingShotCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleEventVar_RewriteValue(VAR_PARTING_SHOT_FLAG, 1);
    }
}
extern "C" void HandlerPartingShotApply(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) ||
        pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        work[0] = 1;
    }
}
extern "C" void HandlerPartingShotFail(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
      if (BattleEventVar_GetValue(VAR_PARTING_SHOT_FLAG) && 
        BattleEventVar_GetValue(VAR_MIRROR_ARMOR_FLAG)) {
        work[0] = 1;
    }
}
extern "C" void HandlerPartingShot(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        work[0] == 1) {
        if (Handler_GetFightEnableBenchPokeNum(serverFlow, pokemonSlot)
            && Handler_CheckReservedMemberChangeAction(serverFlow)) {
            
            HandlerParam_Switch* switchOut;
            switchOut = (HandlerParam_Switch*)BattleHandler_PushWork(serverFlow, EFFECT_SWITCH, pokemonSlot);
            switchOut->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, switchOut);
        }
    }
}
BattleEventHandlerTableEntry PartingShotHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerPartingShotCheck},
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerPartingShotApply},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerPartingShotFail},
    {EVENT_MOVE_SEQUENCE_END, HandlerPartingShot},
};
extern "C" BattleEventHandlerTableEntry* EventAddPartingShot(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PartingShotHandlers);
    return PartingShotHandlers;
}

#endif // EXPAND_MOVES