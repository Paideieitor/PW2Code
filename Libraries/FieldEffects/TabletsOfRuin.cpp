#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

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
#endif // EXPAND_FIELD_EFFECTS