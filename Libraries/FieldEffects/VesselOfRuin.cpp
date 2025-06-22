#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

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