#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// FUR COAT
extern "C" void HandlerFurCoat(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (PML_MoveGetCategory(moveID) == CATEGORY_PHYSICAL) {
            BattleEventVar_MulValue(VAR_RATIO, 2048);
        }
    }
}
BattleEventHandlerTableEntry FurCoatHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerFurCoat},
};
extern "C" BattleEventHandlerTableEntry * EventAddFurCoat(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FurCoatHandlers);
    return FurCoatHandlers;
}
#endif // EXPAND_ABILITIES