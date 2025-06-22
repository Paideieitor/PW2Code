#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// VESSEL OF RUIN
extern "C" void HandlerVesselOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_VESSEL_OF_RUIN, BATTLE_VESSEL_OF_RUIN_MSGID);
}
extern "C" void HandlerVesselOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_VESSEL_OF_RUIN);
}
BattleEventHandlerTableEntry VesselOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerVesselOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerVesselOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerVesselOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerVesselOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddVesselOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(VesselOfRuinHandlers);
    return VesselOfRuinHandlers;
}

// SWORD OF RUIN
extern "C" void HandlerSwordOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_SWORD_OF_RUIN, BATTLE_SWORD_OF_RUIN_MSGID);
}
extern "C" void HandlerSwordOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_SWORD_OF_RUIN);
}
BattleEventHandlerTableEntry SwordOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerSwordOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerSwordOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerSwordOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerSwordOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddSwordOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SwordOfRuinHandlers);
    return SwordOfRuinHandlers;
}

// TABLETS OF RUIN
extern "C" void HandlerTabletsOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_TABLETS_OF_RUIN, BATTLE_TABLETS_OF_RUIN_MSGID);
}
extern "C" void HandlerTabletsOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_TABLETS_OF_RUIN);
}
BattleEventHandlerTableEntry TabletsOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerTabletsOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerTabletsOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerTabletsOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerTabletsOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddTabletsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(TabletsOfRuinHandlers);
    return TabletsOfRuinHandlers;
}

// BEADS OF RUIN
extern "C" void HandlerBeadsOfRuinAdd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonAddFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_BEADS_OF_RUIN, BATTLE_BEADS_OF_RUIN_MSGID);
}
extern "C" void HandlerBeadsOfRuinRemove(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonRemoveFieldEffectAbilityHandler(serverFlow, pokemonSlot, FLDEFF_BEADS_OF_RUIN);
}
BattleEventHandlerTableEntry BeadsOfRuinHandlers[]{
    {EVENT_SWITCH_IN, HandlerBeadsOfRuinAdd}, // Add fieldeffect when switching in
    {EVENT_AFTER_ABILITY_CHANGE, HandlerBeadsOfRuinAdd}, // Add fieldeffect/dependpoke when changing ability and when ability nullify ends
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerBeadsOfRuinRemove}, // Remove dependpoke when changing ability
    {EVENT_ABILITY_NULLIFIED, HandlerBeadsOfRuinRemove}, // Remove dependpoke when ability is nullified
};
extern "C" BattleEventHandlerTableEntry * EventAddBeadsOfRuin(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(BeadsOfRuinHandlers);
    return BeadsOfRuinHandlers;
}
#endif // EXPAND_ABILITIES