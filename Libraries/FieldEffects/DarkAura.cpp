#include "settings.h"
#if EXPAND_FIELD_EFFECTS && EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// DARK AURA FIELD
extern "C" void HandlerFieldDarkAura(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_DARK) {
        if (BattleField_GetAuraBreakMons() == 0) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5448);
        }
        else {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldDarkAuraHandlers[]
{
    {EVENT_MOVE_POWER, HandlerFieldDarkAura},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldDarkAura(u32 * handlerAmount)
{
    *handlerAmount = ARRAY_COUNT(FieldDarkAuraHandlers);
    return FieldDarkAuraHandlers;
}
#endif // EXPAND_FIELD_EFFECTS && EXPAND_ABILITIES