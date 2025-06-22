#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// COTTON DOWN
extern "C" void HandlerCottonDown(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 0;

        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedSlot = BattleMon_GetID(affectedMon);
                if (affectedMon->battleSlot != pokemonSlot) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS) {
                        if (!BattleMon_IsFainted(affectedMon)) {
                            statChange->pokeID[statChange->pokeCount] = affectedSlot;
                            ++statChange->pokeCount;
                        }
                    }
                }
            }
        }

        statChange->moveAnimation = 1;
        statChange->stat = STATSTAGE_SPEED;
        statChange->volume = -1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}
BattleEventHandlerTableEntry CottonDownHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerCottonDown},
};
extern "C" BattleEventHandlerTableEntry * EventAddCottonDown(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CottonDownHandlers);
    return CottonDownHandlers;
}
#endif // EXPAND_ABILITIES