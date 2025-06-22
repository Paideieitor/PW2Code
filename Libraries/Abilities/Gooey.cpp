#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Contact.h"

#include "server_flow.h"

// GOOEY
extern "C" void HandlerGooey(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {

            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (BattleMon_IsStatChangeValid(attackingMon, STATSTAGE_SPEED, -1)) {
                if (!BattleMon_IsFainted(attackingMon)) {
                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = attackingSlot;
                    statChange->moveAnimation = 1;
                    statChange->stat = STATSTAGE_SPEED;
                    statChange->volume = -1;
                    BattleHandler_PopWork(serverFlow, statChange);

                    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
                }
            }
        }
    }
}
BattleEventHandlerTableEntry GooeyHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerGooey},
};
extern "C" BattleEventHandlerTableEntry * EventAddGooey(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GooeyHandlers);
    return GooeyHandlers;
}
#endif // EXPAND_ABILITIES