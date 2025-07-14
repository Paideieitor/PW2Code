#include "settings.h"
#if EXPAND_ITEMS

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

extern "C" void CommonTerrainSeed(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, TERRAIN currentTerrain, TERRAIN terrain, StatStage stat) {
    if (terrain == currentTerrain) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsStatChangeValid(currentMon, stat, 1)) {
            work[0] = stat;
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
        }
    }
}
extern "C" void CommonTerrainSeedUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (work[0] != STATSTAGE_NULL)
    {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->pokeCount = 1;
        statChange->pokeID[0] = (u8)pokemonSlot;
        statChange->stat = (StatStage)work[0];
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);

        work[0] = STATSTAGE_NULL;
    }
}

// ELECTRIC SEED
extern "C" void HandlerElectricSeedSwitchIn(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work, 
            ServerEvent_GetTerrain(serverFlow), TERRAIN_ELECTRIC, STATSTAGE_DEFENSE);
}
extern "C" void HandlerElectricSeed(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            BattleEventVar_GetValue(VAR_WEATHER), TERRAIN_ELECTRIC, STATSTAGE_DEFENSE);
}
BattleEventHandlerTableEntry ElectricSeedHandlers[] = {
    {EVENT_SWITCH_IN, HandlerElectricSeedSwitchIn},
    {EVENT_CHECK_ITEM_REACTION, HandlerElectricSeedSwitchIn},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerElectricSeed},
    {EVENT_USE_ITEM, CommonTerrainSeedUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddElectricSeed(u32* handleAmount) {
    *handleAmount = ARRAY_COUNT(ElectricSeedHandlers);
    return ElectricSeedHandlers;
}

// GRASSY SEED
extern "C" void HandlerGrassySeedSwitchIn(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            ServerEvent_GetTerrain(serverFlow), TERRAIN_GRASSY, STATSTAGE_DEFENSE);
}
extern "C" void HandlerGrassySeed(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            BattleEventVar_GetValue(VAR_WEATHER), TERRAIN_GRASSY, STATSTAGE_DEFENSE);
}
BattleEventHandlerTableEntry GrassySeedHandlers[] = {
    {EVENT_SWITCH_IN, HandlerGrassySeedSwitchIn},
    {EVENT_CHECK_ITEM_REACTION, HandlerGrassySeedSwitchIn},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerGrassySeed},
    {EVENT_USE_ITEM, CommonTerrainSeedUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddGrassySeed(u32 * handleAmount) {
    *handleAmount = ARRAY_COUNT(GrassySeedHandlers);
    return GrassySeedHandlers;
}

// MISTY SEED
extern "C" void HandlerMistySeedSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            ServerEvent_GetTerrain(serverFlow), TERRAIN_MISTY, STATSTAGE_SPECIAL_DEFENSE);
}
extern "C" void HandlerMistySeed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            BattleEventVar_GetValue(VAR_WEATHER), TERRAIN_MISTY, STATSTAGE_SPECIAL_DEFENSE);
}
BattleEventHandlerTableEntry MistySeedHandlers[] = {
    {EVENT_SWITCH_IN, HandlerMistySeedSwitchIn},
    {EVENT_CHECK_ITEM_REACTION, HandlerMistySeedSwitchIn},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerMistySeed},
    {EVENT_USE_ITEM, CommonTerrainSeedUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddMistySeed(u32 * handleAmount) {
    *handleAmount = ARRAY_COUNT(MistySeedHandlers);
    return MistySeedHandlers;
}

// PSYCHIC SEED
extern "C" void HandlerPsychicSeedSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            ServerEvent_GetTerrain(serverFlow), TERRAIN_PSYCHIC, STATSTAGE_SPECIAL_DEFENSE);
}
extern "C" void HandlerPsychicSeed(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT)
        return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID))
        CommonTerrainSeed(item, serverFlow, pokemonSlot, work,
            BattleEventVar_GetValue(VAR_WEATHER), TERRAIN_PSYCHIC, STATSTAGE_SPECIAL_DEFENSE);
}
BattleEventHandlerTableEntry PsychicSeedHandlers[] = {
    {EVENT_SWITCH_IN, HandlerPsychicSeedSwitchIn},
    {EVENT_CHECK_ITEM_REACTION, HandlerPsychicSeedSwitchIn},
    {EVENT_AFTER_TERRAIN_CHANGE, HandlerPsychicSeed},
    {EVENT_USE_ITEM, CommonTerrainSeedUse},
};
extern "C" BattleEventHandlerTableEntry * EventAddPsychicSeed(u32 * handleAmount) {
    *handleAmount = ARRAY_COUNT(PsychicSeedHandlers);
    return PsychicSeedHandlers;
}

#endif // EXPAND_ITEMS