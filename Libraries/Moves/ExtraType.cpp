#include "settings.h"
#ifdef EXPAND_MOVES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

extern "C" void HandlerExtraType(ServerFlow* serverFlow, u32 pokemonSlot, POKE_TYPE addType)
{
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 targetCount = BattleEventVar_GetValue(VAR_TARGET_COUNT);
        for (u32 idx = 0; idx < targetCount; ++idx) {

            u32 targetSlot = BattleEventVar_GetValue((BattleEventVar)(VAR_TARGET_MON_ID + idx));
            BattleMon* targetMon = Handler_GetBattleMon(serverFlow, targetSlot);
            if (!BattleMon_IsFainted(targetMon) &&
                !BattleMon_HasType(targetMon, addType)) {

                BattleField_SetExtraType(targetSlot, addType);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, BATTLE_EXTRA_TYPE_MSGID);
                BattleHandler_AddArg(&message->str, addType);
                BattleHandler_AddArg(&message->str, targetSlot);
                BattleHandler_PopWork(serverFlow, message);
            }
        }
    }
}

// TRICK OR TREAT
extern "C" void HandlerTrickOrTreat(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    HandlerExtraType(serverFlow, pokemonSlot, TYPE_GHOST);
}
BattleEventHandlerTableEntry TrickOrTreatHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerTrickOrTreat},
};
extern "C" BattleEventHandlerTableEntry* EventAddTrickOrTreat(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TrickOrTreatHandlers);
    return TrickOrTreatHandlers;
}

// FOREST'S CURSE
extern "C" void HandlerForestsCurse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    HandlerExtraType(serverFlow, pokemonSlot, TYPE_GRASS);
}
BattleEventHandlerTableEntry ForestsCurseHandlers[]{
    {EVENT_UNCATEGORIZED_MOVE, HandlerForestsCurse},
};
extern "C" BattleEventHandlerTableEntry* EventAddForestsCurse(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(ForestsCurseHandlers);
    return ForestsCurseHandlers;
}

#endif // EXPAND_MOVES