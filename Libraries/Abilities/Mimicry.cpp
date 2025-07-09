#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/Terrains.h"
#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

#include "personal.h"

// MIMICRY
extern "C" u16 GetSpeciesTypePair(u16 species, u16 form) {
    POKE_TYPE type1 = PML_PersonalGetParamSingle(species, form, Personal_Type1);
    POKE_TYPE type2 = PML_PersonalGetParamSingle(species, form, Personal_Type2);

    return PokeTypePair_Make((u16)type1, (u16)type2);
}
extern "C" void MimicryTypeChange(ServerFlow * serverFlow, u32 pokemonSlot, b32 revert) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    u16 newType = GetSpeciesTypePair(currentMon->species, currentMon->form);

    if (!revert) {
        TERRAIN terrain = ServerEvent_GetTerrain(serverFlow);
        switch (terrain) {
        case TERRAIN_NULL:
            break;
        case TERRAIN_ELECTRIC:
            newType = PokeTypePair_MakeMonotype(TYPE_ELEC);
            break;
        case TERRAIN_GRASSY:
            newType = PokeTypePair_MakeMonotype(TYPE_GRASS);
            break;
        case TERRAIN_MISTY:
            newType = PokeTypePair_MakeMonotype(TYPE_FAIRY);
            break;
        case TERRAIN_PSYCHIC:
            newType = PokeTypePair_MakeMonotype(TYPE_PSY);
            break;
        }
    }

    if (newType != BattleMon_GetPokeType(currentMon)) {
        HandlerParam_ChangeType* changeType;
        changeType = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
        changeType->pokeType = newType;
        changeType->pokeID = pokemonSlot;
        BattleHandler_PopWork(serverFlow, changeType);
    }
}
extern "C" void HandlerMimicry(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MimicryTypeChange(serverFlow, pokemonSlot, 0);
    }
}
extern "C" void HandlerMimicryTerrainChange(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        MimicryTypeChange(serverFlow, pokemonSlot, 0);
    }
}
extern "C" void HandlerMimicryNullified(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MimicryTypeChange(serverFlow, pokemonSlot, 1);
    }
}
BattleEventHandlerTableEntry MimicryHandlers[]{
    {EVENT_SWITCH_IN, HandlerMimicry},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerMimicry},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerMimicryTerrainChange},
    {EVENT_ABILITY_NULLIFIED, HandlerMimicryNullified},
};
extern "C" BattleEventHandlerTableEntry * EventAddMimicry(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MimicryHandlers);
    return MimicryHandlers;
}
#endif // EXPAND_ABILITIES