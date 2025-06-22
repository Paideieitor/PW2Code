#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// PRISM ARMOR
extern "C" void HandlerPrismArmor(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        u32 effectiveness = BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS);
        if (effectiveness > EFFECTIVENESS_1) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry PrismArmorHandlers[]{
    {EVENT_MOVE_DAMAGE_PROCESSING_2, HandlerPrismArmor},
};
extern "C" BattleEventHandlerTableEntry * EventAddPrismArmor(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(PrismArmorHandlers);
    return PrismArmorHandlers;
}
#endif // EXPAND_ABILITIES