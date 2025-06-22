#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// GALVANIZE
extern "C" void HandlerGalvanizeTypeChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_NORMAL) {
            BattleEventVar_RewriteValue(VAR_MOVE_TYPE, TYPE_ELEC);
        }
    }
}
extern "C" void HandlerGalvanizePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 4915);
        }
    }
}
BattleEventHandlerTableEntry GalvanizeHandlers[]{
    {EVENT_MOVE_PARAM, HandlerGalvanizeTypeChange},
    {EVENT_MOVE_POWER, HandlerGalvanizePower},
};
extern "C" BattleEventHandlerTableEntry * EventAddGalvanize(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(GalvanizeHandlers);
    return GalvanizeHandlers;
}
#endif // EXPAND_ABILITIES