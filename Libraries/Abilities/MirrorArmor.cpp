#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// MIRROR ARMOR
extern "C" void HandlerMirrorArmor(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    int volume = BattleEventVar_GetValue(VAR_VOLUME);
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && pokemonSlot != attackingSlot
        && volume < 0
        && !BattleEventVar_GetValue(VAR_MIRROR_ARMOR_FLAG)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = attackingSlot;
        statChange->stat = (StatStage)BattleEventVar_GetValue(VAR_MOVE_EFFECT);
        statChange->volume = volume;
        statChange->pad = STAT_CHANGE_MIRROR_ARMOR_FLAG;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry MirrorArmorHandlers[]{
    {EVENT_STAT_STAGE_CHANGE_LAST_CHECK, HandlerMirrorArmor},
};
extern "C" BattleEventHandlerTableEntry * EventAddMirrorArmor(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MirrorArmorHandlers);
    return MirrorArmorHandlers;
}
#endif // EXPAND_ABILITIES