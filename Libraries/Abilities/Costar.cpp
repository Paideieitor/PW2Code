#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/ActionOrderManager.h"

#include "server_flow.h"

// COSTAR
extern "C" void GetStatChanges(s8 * output, StatStageParam * statChanges) {
    output[STATSTAGE_ATTACK - 1] = statChanges->AttackStage - 6;
    output[STATSTAGE_DEFENSE - 1] = statChanges->DefenseStage - 6;
    output[STATSTAGE_SPECIAL_ATTACK - 1] = statChanges->SpAttackStage - 6;
    output[STATSTAGE_SPECIAL_DEFENSE - 1] = statChanges->SpDefenseStage - 6;
    output[STATSTAGE_SPEED - 1] = statChanges->SpeedStage - 6;
    output[STATSTAGE_ACCURACY - 1] = statChanges->AccuracyStage - 6;
    output[STATSTAGE_EVASION - 1] = statChanges->EvasionStage - 6;
}
extern "C" b32 HasStatChanges(s8 * statChanges) {
    for (u32 stat = STATSTAGE_ATTACK; stat <= STATSTAGE_EVASION; ++stat) {
        if (statChanges[stat - 1] != 6) {
            return 1;
        }
    }
    return 0;
}
extern "C" void HandlerCostar(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u32 allyPos = CommonGetAllyPos(serverFlow, Handler_PokeIDToPokePos(serverFlow, pokemonSlot));
        if (allyPos == NULL_BATTLE_POS) {
            return;
        }

        u32 allySlot = Handler_PokePosToPokeID(serverFlow, allyPos);
        BattleMon* allyMon = Handler_GetBattleMon(serverFlow, allySlot);
        s8 statChanges[7];
        GetStatChanges(statChanges, &allyMon->statStageParam);

        b32 hasFocusEnergy = BattleMon_GetConditionFlag(allyMon, CONDITIONFLAG_FOCUSENERGY);
        if (HasStatChanges(statChanges) || hasFocusEnergy) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            for (u32 stat = STATSTAGE_ATTACK; stat <= STATSTAGE_EVASION; ++stat) {
                s8 volume = statChanges[stat - 1];
                if (volume != 0) {
                    HandlerParam_ChangeStatStage* statChange;
                    statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
                    statChange->pokeCount = 1;
                    statChange->pokeID[0] = pokemonSlot;
                    statChange->stat = (StatStage)stat;
                    statChange->volume = volume;
                    BattleHandler_PopWork(serverFlow, statChange);
                }
            }

            if (hasFocusEnergy) {
                HandlerParam_SetConditionFlag* setConditionFlag;
                setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_CONDITION_FLAG, pokemonSlot);
                setConditionFlag->pokeID = pokemonSlot;
                setConditionFlag->flag = CONDITIONFLAG_FOCUSENERGY;
                BattleHandler_PopWork(serverFlow, setConditionFlag);

                HandlerParam_Message* message;
                message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
                BattleHandler_StrSetup(&message->str, 2u, 1041u);
                BattleHandler_AddArg(&message->str, pokemonSlot);
                BattleHandler_PopWork(serverFlow, message);
            }

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}
BattleEventHandlerTableEntry CostarHandlers[]{
    {EVENT_SWITCH_IN, HandlerCostar},
};
extern "C" BattleEventHandlerTableEntry * EventAddCostar(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CostarHandlers);
    return CostarHandlers;
}
#endif // EXPAND_ABILITIES