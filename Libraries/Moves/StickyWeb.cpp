#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// STICKY WEB
extern "C" void HandlerStickyWeb(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    ConditionData condData = Condition_MakePermanent();
    u8 opposingSide = GetSideFromOpposingMonID(pokemonSlot);
    CommonCreateSideEffect(item, serverFlow, pokemonSlot, work, opposingSide, SIDEEFF_STICKY_WEB, 
        condData, BATTLE_STICKY_WEB_USE_MSGID + opposingSide);
}
BattleEventHandlerTableEntry StickyWebHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE_NO_TARGET, HandlerStickyWeb}, 
};
extern "C" BattleEventHandlerTableEntry* EventAddStickyWeb(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(StickyWebHandlers);
    return StickyWebHandlers;
}

#endif // EXPAND_MOVES