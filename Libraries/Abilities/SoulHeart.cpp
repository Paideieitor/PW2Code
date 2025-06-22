#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// SOUL-HEART
extern "C" void HandlerSoulHeart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot != currentSlot) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (BattleMon_IsFainted(currentMon)) {
            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            statChange->pokeCount = 1;
            statChange->pokeID[0] = pokemonSlot;
            statChange->stat = STATSTAGE_SPECIAL_ATTACK;
            statChange->volume = 1;
            BattleHandler_PopWork(serverFlow, statChange);
        }
    }
}
BattleEventHandlerTableEntry SoulHeartHandlers[]{
    {EVENT_NOTIFY_FAINTED, HandlerSoulHeart},
};
extern "C" BattleEventHandlerTableEntry * EventAddSoulHeart(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SoulHeartHandlers);
    return SoulHeartHandlers;
}
#endif // EXPAND_ABILITIES