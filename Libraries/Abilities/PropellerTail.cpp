#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// PROPELLER TAIL
extern "C" void HandlerPropellerTail(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    // Got through all the actions this turn
    for (u8 currentActionIdx = 0; currentActionIdx < 6; ++currentActionIdx) {
        ActionOrderWork* currentActionOrder = &serverFlow->actionOrderWork[currentActionIdx];

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // If the action is performed by a Propeller Tail or Stalwart Pokémon
        if (currentActionOrder->battleMon) {
            if (currentActionOrder->battleMon == currentMon) {
                u32 action = BattleAction_GetAction(&currentActionOrder->action);
                // And the action is a fight move
                if (action == 1) {
                    BattleAction_Fight* fight = (BattleAction_Fight*)&currentActionOrder->action;
                    if (fight->targetPos != NULL_BATTLE_POS) {

                        u32 swapPos1 = Handler_PokeIDToPokePos(serverFlow, BattleEventVar_GetValue(NEW_VAR_ATTACKING_MON));
                        u32 swapPos2 = Handler_PokeIDToPokePos(serverFlow, BattleEventVar_GetValue(NEW_VAR_DEFENDING_MON));
                        // Swap the target so it remains as it was originally selected
                        if (fight->targetPos == swapPos1) {
                            fight->targetPos = swapPos2;
                        }
                        else if (fight->targetPos == swapPos2) {
                            fight->targetPos = swapPos1;
                        }

                    }
                }
            }
        }
    }
}
BattleEventHandlerTableEntry PropellerTailHandlers[]{
    {EVENT_SWAP_POKE, HandlerPropellerTail},
};
extern "C" BattleEventHandlerTableEntry * EventAddPropellerTail(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PropellerTailHandlers);
    return PropellerTailHandlers;
}
#endif // EXPAND_ABILITIES