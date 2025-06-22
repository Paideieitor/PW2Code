#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// FLOWER VEIL
extern "C" void HandlerFlowerVeilPreventStatus(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, defendingSlot), TYPE_GRASS)) {
        if (BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_PARALYSIS ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_SLEEP ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_FREEZE ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_BURN ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_POISON ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_CONFUSION ||
            BattleEventVar_GetValue(VAR_CONDITION_ID) == CONDITION_YAWN) {
            *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, FORCE_FAIL_MESSAGE);
        }
    }
}
extern "C" void HandlerFlowerVeilStatusFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if ((pokemonSlot == defendingSlot || MainModule_IsAllyMonID(pokemonSlot, defendingSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, defendingSlot), TYPE_GRASS) &&
        *work) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);
        HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, BATTLE_FLOWER_VEIL_MSGID);
        BattleHandler_AddArg(&message->str, defendingSlot);
        BattleHandler_PopWork(serverFlow, message);
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        *work = 0;
    }
}
extern "C" void HandlerFlowerVeilStatCheck(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);

    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, currentSlot), TYPE_GRASS) &&
        currentSlot != BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_VOLUME) <= 0) {
        *work = BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);
    }
}
extern "C" void HandlerFlowerVeilStatFailMessage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);

    if ((pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) &&
        BattleMon_HasType(Handler_GetBattleMon(serverFlow, currentSlot), TYPE_GRASS) &&
        *work) {

        u32 moveSerial = BattleEventVar_GetValue(VAR_MOVE_SERIAL);
        if (!moveSerial || work[1] != moveSerial) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_Message* message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_FLOWER_VEIL_MSGID);
            BattleHandler_AddArg(&message->str, currentSlot);
            BattleHandler_PopWork(serverFlow, message);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
            work[1] = moveSerial;
        }
        *work = 0;
    }
}
BattleEventHandlerTableEntry FlowerVeilHandlers[]{
    {EVENT_ADD_CONDITION_CHECK_FAIL, HandlerFlowerVeilPreventStatus},
    {EVENT_ADD_CONDITION_FAIL, HandlerFlowerVeilStatusFailMessage},
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerFlowerVeilStatCheck},
    {EVENT_STAT_STAGE_CHANGE_FAIL, HandlerFlowerVeilStatFailMessage},
};
extern "C" BattleEventHandlerTableEntry * EventAddFlowerVeil(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FlowerVeilHandlers);
    return FlowerVeilHandlers;
}
#endif // EXPAND_ABILITIES