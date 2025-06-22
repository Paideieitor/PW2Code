#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/ActionOrderManager.h"

#include "server_flow.h"

// HOSPITALITY
extern "C" void HandlerHospitality(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 allyPos = CommonGetAllyPos(serverFlow, Handler_PokeIDToPokePos(serverFlow, pokemonSlot));
        if (allyPos == NULL_BATTLE_POS) {
            return;
        }

        u32 allySlot = Handler_PokePosToPokeID(serverFlow, allyPos);
        BattleMon* allyMon = Handler_GetBattleMon(serverFlow, allySlot);

        if (!BattleMon_CheckIfMoveCondition(allyMon, CONDITION_HEALBLOCK) &&
            !BattleMon_IsFullHP(allyMon)) {
            HandlerParam_RecoverHP* recoveHP;
            recoveHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
            recoveHP->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            recoveHP->pokeID = allySlot;
            recoveHP->recoverHP = DivideMaxHPZeroCheck(allyMon, 4u);
            BattleHandler_StrSetup(&recoveHP->exStr, 2u, 387u);
            BattleHandler_AddArg(&recoveHP->exStr, allySlot);
            BattleHandler_PopWork(serverFlow, recoveHP);
        }
    }
}
BattleEventHandlerTableEntry HospitalityHandlers[]{
    {EVENT_SWITCH_IN, HandlerHospitality},
};
extern "C" BattleEventHandlerTableEntry * EventAddHospitality(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(HospitalityHandlers);
    return HospitalityHandlers;
}
#endif // EXPAND_ABILITIES