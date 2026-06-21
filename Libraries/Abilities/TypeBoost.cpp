#include "settings.h"
#if EXPAND_ABILITIES

#include "server_flow.h"

void TypeBoostHandler(u32 pokemonSlot, POKE_TYPE type, u32 ratio) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == type) {
            BattleEventVar_MulValue(VAR_RATIO, ratio);
        }
    }
}

// TRANSISTOR
extern "C" void HandlerTransistor(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
#if GEN8_TRANSISTOR
    TypeBoostHandler(pokemonSlot, TYPE_ELEC, 6144);
#else
    TypeBoostHandler(pokemonSlot, TYPE_ELEC, 5324);
#endif
}
BattleEventHandlerTableEntry TransistorHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerTransistor},
};
extern "C" BattleEventHandlerTableEntry * EventAddTransistor(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TransistorHandlers);
    return TransistorHandlers;
}

// DRAGON'S MAW
extern "C" void HandlerDragonsMaw(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    TypeBoostHandler(pokemonSlot, TYPE_DRAGON, 6144);
}
BattleEventHandlerTableEntry DragonsMawHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerDragonsMaw},
};
extern "C" BattleEventHandlerTableEntry * EventAddDragonsMaw(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DragonsMawHandlers);
    return DragonsMawHandlers;
}

// FIRE MANE
extern "C" void HandlerFireMane(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    TypeBoostHandler(pokemonSlot, TYPE_FIRE, 6144);
}
BattleEventHandlerTableEntry FireManeHandlers[]{
    {EVENT_ATTACKER_POWER, HandlerFireMane},
};
extern "C" BattleEventHandlerTableEntry * EventAddFireMane(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FireManeHandlers);
    return FireManeHandlers;
}
#endif // EXPAND_ABILITIES
