#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// LIGHTNING ROD
extern "C" void HandlerLightningRod(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerLightningRodStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerLightningRodCheckNoEffect(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work);
extern "C" void HandlerLightningRodImmune(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_POKE_TYPE) == TYPE_GROUND &&
        BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_ELEC)
    {
        BattleEventVar_RewriteValue(VAR_NO_TYPE_EFFECTIVENESS, 1);
    }
}
BattleEventHandlerTableEntry LightningRodHandlers[]{
    {EVENT_REDIRECT_TARGET, HandlerLightningRod},
    {EVENT_MOVE_EXECUTE_START, HandlerLightningRodStart},
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerLightningRodCheckNoEffect},
    {EVENT_CHECK_TYPE_EFFECTIVENESS, HandlerLightningRodImmune},
};
extern "C" BattleEventHandlerTableEntry * EventAddLightningRodUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(LightningRodHandlers);
    return LightningRodHandlers;
}
#endif // EXPAND_ABILITIES