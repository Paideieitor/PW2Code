#include "settings.h"
#if EXPAND_ITEMS

#include "server_flow.h"

// MIRROR HERB
// work (as a s8 array) -> stat boosts to apply
extern "C" void HandlerMirrorHerbStatChange(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (pokemonSlot != currentSlot && GetSideFromMonID(pokemonSlot) != GetSideFromMonID(currentSlot)) {
        StatStage stat = (StatStage)BattleEventVar_GetValue(VAR_MOVE_EFFECT);
        int volume = BattleEventVar_GetValue(VAR_VOLUME);
        if (volume > 0) {
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (BattleMon_IsStatChangeValid(currentMon, stat, volume)) {
                s8* statBoosts = (s8*)work;
                statBoosts[stat] += (s8)volume;
            }
        }
    }
}
extern "C" void HandlerMirrorHerbActionEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    s8* statBoosts = (s8*)work;
    for (u32 stat = 0; stat <= STATSTAGE_EVASION; ++stat) {
        if (statBoosts[stat] > 0) {
            ItemEvent_PushRun(item, serverFlow, pokemonSlot);
            break;
        }
    }
}
extern "C" void HandlerMirrorHerbUse(BattleEventItem* item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        s8* statBoosts = (s8*)work;
        for (u16 stat = 0; stat <= STATSTAGE_EVASION; ++stat) {
            s8 volume = statBoosts[stat];
            if (volume > 0) {
                HandlerParam_ChangeStatStage* statBoost;
                statBoost = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                statBoost->pokeCount = 1;
                statBoost->pokeID[0] = (u8)pokemonSlot;
                statBoost->stat = (StatStage)stat;
                statBoost->volume = volume;
                BattleHandler_PopWork(serverFlow, statBoost);
            }
            statBoosts[stat] = 0;
        }
    }
}
BattleEventHandlerTableEntry MirrorHerbHandlers[] = {
    {EVENT_STAT_STAGE_CHANGE_APPLIED, HandlerMirrorHerbStatChange},
    {EVENT_ACTION_PROCESSING_END, HandlerMirrorHerbActionEnd},
    {EVENT_USE_ITEM, HandlerMirrorHerbUse},
};
extern "C" BattleEventHandlerTableEntry* EventAddMirrorHerb(u32* handlerAmount) {
    *handlerAmount = ARRAY_COUNT(MirrorHerbHandlers);
    return MirrorHerbHandlers;
}
#endif // EXPAND_ITEMS