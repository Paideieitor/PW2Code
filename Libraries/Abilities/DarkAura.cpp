#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// DARK AURA
extern "C" void HandlerDarkAuraAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_DARK_AURA, BATTLE_DARK_AURA_MSGID);
}
extern "C" void HandlerDarkAuraRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_DARK_AURA);
}
BattleEventHandlerTableEntry DarkAuraHandlers[]{
    {EVENT_SWITCH_IN, HandlerDarkAuraAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerDarkAuraAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerDarkAuraRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerDarkAuraRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddDarkAura(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(DarkAuraHandlers);
    return DarkAuraHandlers;
}
#endif // EXPAND_ABILITIES