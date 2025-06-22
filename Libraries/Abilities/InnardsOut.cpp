#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"

#include "server_flow.h"

// INNARDS OUT
extern "C" void HandlerInnardsOut(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)) {
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFainted(defendingMon)) {
            u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            if (Handler_PokeIDToPokePos(serverFlow, attackingSlot) != NULL_BATTLE_POS) {

                u32 currentHP = BattleMon_GetValue(defendingMon, VALUE_CURRENT_HP);
                u32 beforeDmgHP = currentHP + BattleEventVar_GetValue(VAR_DAMAGE);

                HandlerParam_Damage* damage;
                damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                damage->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                damage->pokeID = attackingSlot;
                damage->damage = beforeDmgHP;
                damage->flags = damage->flags & 0xFE | 1;
                BattleHandler_StrSetup(&damage->exStr, 2u, 402u);
                BattleHandler_AddArg(&damage->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, damage);
            }
        }
    }
}
BattleEventHandlerTableEntry InnardsOutHandlers[]{
    {EVENT_MOVE_DAMAGE_REACTION_1, HandlerInnardsOut},
};
extern "C" BattleEventHandlerTableEntry * EventAddInnardsOut(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(InnardsOutHandlers);
    return InnardsOutHandlers;
}
#endif // EXPAND_ABILITIES