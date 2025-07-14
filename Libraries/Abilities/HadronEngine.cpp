#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

#include "server_flow.h"

// HADRON ENGINE
extern "C" void HandlerHadronEngineTerrain(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = FLDEFF_TERRAIN;
        addFieldEffect->field_D = TERRAIN_ELECTRIC;
        addFieldEffect->condData = Condition_MakeTurn(TERRAIN_ABILITY_TURNS);
        BattleHandler_StrSetup(&addFieldEffect->exStr, 2u, BATTLE_HADRON_ENGINE_MSGID);
        BattleHandler_AddArg(&addFieldEffect->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void HandlerHadronEngineAttack(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON) &&
        BattleEventVar_GetValue(VAR_MOVE_CATEGORY) == CATEGORY_SPECIAL &&
        ServerEvent_GetTerrain(serverFlow) == TERRAIN_ELECTRIC) {

        BattleEventVar_MulValue(VAR_RATIO, 5461);
    }
}
BattleEventHandlerTableEntry HadronEngineHandlers[]{
    {EVENT_SWITCH_IN, HandlerHadronEngineTerrain},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerHadronEngineTerrain},
    {EVENT_ATTACKER_POWER, HandlerHadronEngineAttack},
};
extern "C" BattleEventHandlerTableEntry * EventAddHadronEngine(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HadronEngineHandlers);
    return HadronEngineHandlers;
}
#endif // EXPAND_ABILITIES