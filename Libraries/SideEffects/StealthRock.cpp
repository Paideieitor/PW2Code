#include "settings.h"
#if EXPAND_FIELD_EFFECTS

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/FieldEffectExpansion.h"

// STEALTH ROCK
extern "C" void HandlerSideStealthRock(BattleEventItem * item, ServerFlow * serverFlow, u32 side, u32 * work) {
    u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
    if (side == GetSideFromMonID(currentSlot)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, currentSlot);
        if (CheckDisableEntryHazards(currentMon))
            return;

        u32 typePair = BattleMon_GetPokeType(currentMon);
        TypeEffectiveness effectiveness = GetTypeEffectivenessVsMon(TYPE_ROCK, typePair);

        u32 damageFraction = 32;
        switch (effectiveness)
        {
        case EFFECTIVENESS_1_2:
            damageFraction = 16;
            break;
        case EFFECTIVENESS_1:
            damageFraction = 8;
            break;
        case EFFECTIVENESS_2:
            damageFraction = 4;
            break;
        case EFFECTIVENESS_4:
            damageFraction = 2;
            break;
        }

        HandlerParam_Damage* damage;
        damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, BATTLE_MAX_SLOTS);
        damage->pokeID = currentSlot;
        damage->damage = DivideMaxHPZeroCheck(currentMon, damageFraction);
        BattleHandler_StrSetup(&damage->exStr, 2u, 854);
        BattleHandler_AddArg(&damage->exStr, currentSlot);
        BattleHandler_PopWork(serverFlow, damage);
    }
}

BattleEventHandlerTableEntry SideStealthRockHandlers[]{
    {EVENT_SWITCH_IN, HandlerSideStealthRock},
};
extern "C" BattleEventHandlerTableEntry * EventAddSideStealthRockUpdated(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(SideStealthRockHandlers);
    return SideStealthRockHandlers;
}
#endif // EXPAND_FIELD_EFFECTS