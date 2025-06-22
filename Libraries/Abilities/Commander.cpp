#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"

//COMMANDER
// work[0] -> the battle slot of the Dondozo
// work[1] -> the index where the Dondozo slot is stored in the "pairedDondozos" array, plus one
extern "C" u32 FindCommanderAlly(ServerFlow * serverFlow, u32 pokemonSlot) {
    for (u8 i = 0; i < 24; ++i) {
        BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (affectedMon) {

            u32 affectedSlot = BattleMon_GetID(affectedMon);
            if (affectedSlot != pokemonSlot &&
                MainModule_IsAllyMonID(pokemonSlot, affectedSlot)) {

                u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, affectedSlot);
                // If it's currently in the battlefield
                // TODO: ADD DONDOZO
                if (affectedPos != NULL_BATTLE_POS &&
                    !BattleMon_IsFainted(affectedMon) &&
                    affectedMon->species == PKM_NULL &&
                    // Check that this Dondozo is not already paired
                    !BattleField_IsDondozoPaired(affectedSlot)) {
                    return affectedSlot;
                }
            }
        }
    }

    return BATTLE_MAX_SLOTS;
}
extern "C" void CommanderStart(ServerFlow * serverFlow, u32 pokemonSlot, u32 * work, u32 currentSlot) {
    u32 dondozoSlot = BATTLE_MAX_SLOTS;
    if (pokemonSlot != currentSlot) {
        dondozoSlot = currentSlot;
    }
    else {
        dondozoSlot = FindCommanderAlly(serverFlow, pokemonSlot);
    }

    BattleMon* tatsugiriMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    BattleMon* dondozoMon = Handler_GetBattleMon(serverFlow, dondozoSlot);
    // TODO: ADD TATSUGIRI AND DONDOZO
    if (!tatsugiriMon || !dondozoMon ||
        tatsugiriMon->species != PKM_NULL ||
        dondozoMon->species != PKM_NULL) {
        return;
    }

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

    // Remove Tatsugiris Substitute
    tatsugiriMon->substituteHP = 0;

    HandlerParam_SetConditionFlag* setConditionFlag;
    // Remove Tatsugiris other hide states if they are set
    CONDITION_FLAG hideFlag = BattleMon_GetHideCondition(tatsugiriMon);
    if (hideFlag != 16) {
        setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_RESET_CONDITION_FLAG, pokemonSlot);
        setConditionFlag->pokeID = pokemonSlot;
        setConditionFlag->flag = hideFlag;
        BattleHandler_PopWork(serverFlow, setConditionFlag);
    }

    // Remove Tatsugiris charge move state if it is set
    if (BattleMon_CheckIfMoveCondition(tatsugiriMon, CONDITION_CHARGELOCK)) {
        HandlerParam_CureCondition* cureCondition;
        cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
        cureCondition->condition = CONDITION_CHARGELOCK;
        cureCondition->pokeID[0] = pokemonSlot;
        cureCondition->pokeCount = 1;
        cureCondition->msgDisable = 1;
        BattleHandler_PopWork(serverFlow, cureCondition);
    }

    // Make Tatsugiri semi-invulnerable
    setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_CONDITION_FLAG, pokemonSlot);
    setConditionFlag->pokeID = pokemonSlot;
    setConditionFlag->flag = CONDITIONFLAG_SHADOWFORCE;
    BattleHandler_PopWork(serverFlow, setConditionFlag);

    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_Hide, pokemonSlot, 1);

    // Make Tatsugiri skip the action selection
    setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_SET_CONDITION_FLAG, pokemonSlot);
    setConditionFlag->pokeID = pokemonSlot;
    setConditionFlag->flag = CONDITIONFLAG_NOACTION;
    BattleHandler_PopWork(serverFlow, setConditionFlag);

    // Make Tatsugiri unable to switch
    HandlerParam_AddCondition* addCondition;
    addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
    addCondition->pokeID = pokemonSlot;
    addCondition->condition = CONDITION_BLOCK;
    addCondition->condData = ConditionData_MakePoke(dondozoSlot);
    BattleHandler_PopWork(serverFlow, addCondition);

    // Apply omni-boost to Dondozo
    for (u32 stat = STATSTAGE_ATTACK; stat <= STATSTAGE_SPEED; ++stat) {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = (StatStage)stat;
        statChange->volume = 2;
        statChange->pokeCount = 1;
        statChange->pokeID[0] = dondozoSlot;
        BattleHandler_PopWork(serverFlow, statChange);
    }

    // Make Dondozo unable to switch
    addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
    addCondition->pokeID = dondozoSlot;
    addCondition->condition = CONDITION_BLOCK;
    addCondition->condData = Condition_MakePermanent();
    BattleHandler_PopWork(serverFlow, addCondition);
    // Force switch prevention handled in [ServerEvent_CheckForceSwitch]

    BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

    work[0] = dondozoSlot;
    BattleField_AddPairedDondozo(dondozoSlot, &work[1]);
    ++work[1];

#if COMMANDER_DEBUG
    DPRINTF("TATSU %d \n", pokemonSlot);
    DPRINTF("   HIDE %d \n", BattleMon_GetHideCondition(tatsugiriMon));
    DPRINTF("   CHARGE %d \n", BattleMon_CheckIfMoveCondition(tatsugiriMon, CONDITION_CHARGELOCK));
    DPRINTF("   NO ACT %d \n", BattleMon_GetConditionFlag(tatsugiriMon, CONDITIONFLAG_NOACTION));
    DPRINTF("   DOZO %d \n", dondozoSlot);
    DPRINTF("   PAIR %d \n", work[1]);
#endif
}
extern "C" void HandlerCommanderSwitchIn(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {

    if (work[1] == 0) {
        u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
        if (pokemonSlot == currentSlot || MainModule_IsAllyMonID(pokemonSlot, currentSlot)) {

            CommanderStart(serverFlow, pokemonSlot, work, currentSlot);
        }
    }
}
extern "C" void HandlerCommanderSwitchInStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[1] == 0) {
        CommanderStart(serverFlow, pokemonSlot, work, pokemonSlot);
    }
}
extern "C" void HandlerCommanderFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (work[1] > 0) {
        u32 pairIndex = work[1] - 1;

        u32 currentSlot = BattleEventVar_GetValue(VAR_MON_ID);
        if (currentSlot == BattleField_GetDondozoFromPair(pairIndex)) {

            // Remove Tatsugiris hide state
            HandlerParam_SetConditionFlag* setConditionFlag;
            setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_RESET_CONDITION_FLAG, pokemonSlot);
            setConditionFlag->pokeID = pokemonSlot;
            setConditionFlag->flag = CONDITIONFLAG_SHADOWFORCE;
            BattleHandler_PopWork(serverFlow, setConditionFlag);

            // Remove Tatsugiris No Action state
            setConditionFlag = (HandlerParam_SetConditionFlag*)BattleHandler_PushWork(serverFlow, EFFECT_RESET_CONDITION_FLAG, pokemonSlot);
            setConditionFlag->pokeID = pokemonSlot;
            setConditionFlag->flag = CONDITIONFLAG_NOACTION;
            BattleHandler_PopWork(serverFlow, setConditionFlag);

            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_Hide, pokemonSlot, 0);

            work[0] = BATTLE_MAX_SLOTS;
            work[1] = 0;
        }
    }
}
BattleEventHandlerTableEntry CommanderHandlers[]{
    {EVENT_SWITCH_IN, HandlerCommanderSwitchIn},
    {EVENT_AFTER_LAST_SWITCHIN, HandlerCommanderSwitchInStart},
    {EVENT_NOTIFY_FAINTED, HandlerCommanderFainted},
};
extern "C" BattleEventHandlerTableEntry * EventAddCommander(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(CommanderHandlers);
    return CommanderHandlers;
}
#endif // EXPAND_ABILITIES