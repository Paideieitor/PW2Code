#include "settings.h"
#if EXPAND_FIELD_EFFECTS && EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// FAIRY AURA FIELD
extern "C" void HandlerFieldFairyAura(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == TYPE_FAIRY) {
        if (BattleField_GetAuraBreakMons() == 0) {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 5448);
        }
        else {
            BattleEventVar_MulValue(VAR_MOVE_POWER_RATIO, 3072);
        }
    }
}
BattleEventHandlerTableEntry FieldFairyAuraHandlers[]
{
    {EVENT_MOVE_POWER, HandlerFieldFairyAura},
};
extern "C" BattleEventHandlerTableEntry * EventAddFieldFairyAura(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FieldFairyAuraHandlers);
    return FieldFairyAuraHandlers;
}
#endif // EXPAND_FIELD_EFFECTS && EXPAND_ABILITIES