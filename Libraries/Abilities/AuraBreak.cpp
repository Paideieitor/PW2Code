#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// AURA BREAK
extern "C" void HandlerAuraBreakAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_AddAuraBreakMon();

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_AURA_BREAK_MSGID);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerAuraBreakRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_RemoveAuraBreakMon();
    }
}
extern "C" void HandlerAuraBreakRemoveFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        BattleField_RemoveAuraBreakMon();
    }
}
BattleEventHandlerTableEntry AuraBreakHandlers[]{
    {EVENT_SWITCH_IN, HandlerAuraBreakAdd},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerAuraBreakAdd},
    {EVENT_SWITCH_OUT_END, HandlerAuraBreakRemove},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerAuraBreakRemove},
    {EVENT_ABILITY_NULLIFIED, HandlerAuraBreakRemove},
    {EVENT_NOTIFY_FAINTED, HandlerAuraBreakRemoveFainted},
};
extern "C" BattleEventHandlerTableEntry * EventAddAuraBreak(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(AuraBreakHandlers);
    return AuraBreakHandlers;
}
#endif // EXPAND_ABILITIES