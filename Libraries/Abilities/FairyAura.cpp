#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// FAIRY AURA
extern "C" void HandlerFairyAuraAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_FAIRY_AURA, BATTLE_FAIRY_AURA_MSGID);
}
extern "C" void HandlerFairyAuraRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_FAIRY_AURA);
}
BattleEventHandlerTableEntry FairyAuraHandlers[]{
    {EVENT_SWITCH_IN, HandlerFairyAuraAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerFairyAuraAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerFairyAuraRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerFairyAuraRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddFairyAura(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(FairyAuraHandlers);
    return FairyAuraHandlers;
}
#endif // EXPAND_ABILITIES