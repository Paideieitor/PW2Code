#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

// SUPERSWEET SYRUP 
extern "C" void HandlerSupersweetSyrup(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        !BattleField_CheckSupersweetSyrupFlag(pokemonSlot)) {
        BattleField_SetSupersweetSyrupFlag(pokemonSlot);

        u16 frontPokeExist = Handler_GetExistFrontPokePos(serverFlow, pokemonSlot);
        u8* frontSlots = Handler_GetTempWork(serverFlow);
        u32 frontCount = Handler_ExpandPokeID(serverFlow, frontPokeExist | 0x100, frontSlots);
        if (frontCount) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->stat = STATSTAGE_EVASION;
            statChange->volume = -1;
            statChange->moveAnimation = 1;
            statChange->pokeCount = frontCount;
            for (u8 frontCurrent = 0; frontCurrent < frontCount; ++frontCurrent) {
                statChange->pokeID[frontCurrent] = frontSlots[frontCurrent];
            }
            BattleHandler_PopWork(serverFlow, statChange);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry SupersweetSyrupHandlers[]{
    {EVENT_SWITCH_IN, HandlerSupersweetSyrup},
    {EVENT_AFTER_ABILITY_CHANGE, HandlerSupersweetSyrup},
};
extern "C" BattleEventHandlerTableEntry * EventAddSupersweetSyrup(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SupersweetSyrupHandlers);
    return SupersweetSyrupHandlers;
}
#endif // EXPAND_ABILITIES