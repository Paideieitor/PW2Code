#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// BEADS OF RUIN FIELD
extern "C" void HandlerFieldBeadsOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot != BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (BattleMon_GetValue(defendingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_BEADS_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldBeadsOfRuinHandlers[]
{
    {EVENT_DEFENDER_GUARD, HandlerFieldBeadsOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldBeadsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldBeadsOfRuinHandlers);
    return FieldBeadsOfRuinHandlers;
}

// SWORD OF RUIN FIELD
extern "C" void HandlerFieldSwordOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON));
        if (BattleMon_GetValue(defendingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_SWORD_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldSwordOfRuinHandlers[]
{
    {EVENT_DEFENDER_GUARD, HandlerFieldSwordOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldSwordOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldSwordOfRuinHandlers);
    return FieldSwordOfRuinHandlers;
}

// TABLETS OF RUIN FIELD
extern "C" void HandlerFieldTabletsOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_PHYSICAL) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_TABLETS_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldTabletsOfRuinHandlers[]
{
    {EVENT_ATTACKER_POWER, HandlerFieldTabletsOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldTabletsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldTabletsOfRuinHandlers);
    return FieldTabletsOfRuinHandlers;
}

// VESSEL OF RUIN FIELD
extern "C" void HandlerFieldVesselOfRuin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON));
        if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_VESSEL_OF_RUIN) {
            BattleEventVar_MulValue(VAR_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldVesselOfRuinHandlers[]
{
    {EVENT_ATTACKER_POWER, HandlerFieldVesselOfRuin},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldVesselOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldVesselOfRuinHandlers);
    return FieldVesselOfRuinHandlers;
}
#endif // EXPAND_FIELD_EFFECTS