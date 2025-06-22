#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

extern "C" void CommonEmbodyAspectHandler(ServerFlow * serverFlow, u32 pokemonSlot, StatStage stat) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {

        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        statChange->stat = stat;
        statChange->volume = 1;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}

// EMBODY ASPECT TEAL
extern "C" void HandlerEmbodyAspectTeal(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_SPEED);
}
BattleEventHandlerTableEntry EmbodyAspectTealHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectTeal},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectTeal(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectTealHandlers);
    return EmbodyAspectTealHandlers;
}

// EMBODY ASPECT HEARTHFLAME
extern "C" void HandlerEmbodyAspectHearthflame(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_ATTACK);
}
BattleEventHandlerTableEntry EmbodyAspectHearthflameHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectHearthflame},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectHearthflame(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectHearthflameHandlers);
    return EmbodyAspectHearthflameHandlers;
}

// EMBODY ASPECT WELLSPRING
extern "C" void HandlerEmbodyAspectWellspring(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_SPECIAL_DEFENSE);
}
BattleEventHandlerTableEntry EmbodyAspectWellspringHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectWellspring},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectWellspring(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectWellspringHandlers);
    return EmbodyAspectWellspringHandlers;
}

// EMBODY ASPECT CORNERSTONE
extern "C" void HandlerEmbodyAspectCornerstorne(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    CommonEmbodyAspectHandler(serverFlow, pokemonSlot, STATSTAGE_DEFENSE);
}
BattleEventHandlerTableEntry EmbodyAspectCornerstorneHandlers[]{
    {EVENT_AFTER_ABILITY_CHANGE, HandlerEmbodyAspectCornerstorne},
};
extern "C" BattleEventHandlerTableEntry * EventAddEmbodyAspectCornerstorne(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(EmbodyAspectCornerstorneHandlers);
    return EmbodyAspectCornerstorneHandlers;
}
#endif // EXPAND_ABILITIES