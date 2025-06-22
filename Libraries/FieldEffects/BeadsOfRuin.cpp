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
#endif // EXPAND_FIELD_EFFECTS