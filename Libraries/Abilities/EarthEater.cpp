#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

// EARTH EATER
extern "C" void HandlerEarthEater(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (CommonDamageRecoverCheck(serverFlow, pokemonSlot, TYPE_GROUND)) {
        CommonTypeRecoverHP(serverFlow, pokemonSlot, 4u);
    }
}
BattleEventHandlerTableEntry EarthEaterHandlers[]{
    {EVENT_ABILITY_CHECK_NO_EFFECT, HandlerEarthEater},
};
extern "C" BattleEventHandlerTableEntry * EventAddEarthEater(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EarthEaterHandlers);
    return EarthEaterHandlers;
}
#endif // EXPAND_ABILITIES