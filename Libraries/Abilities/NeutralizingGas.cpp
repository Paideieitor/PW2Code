#include "settings.h"
#if EXPAND_ABILITIES

#include "Patches/BattleUpgrade/BattleEngine.h"
#include "Patches/BattleUpgrade/include/BattleField.h"

#include "server_flow.h"
#include "server_events.h"

#include "abilities.h"

// NEUTRALIZING GAS
extern "C" void NeutralizingGasEnd(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleField_RemoveNeutralizingGasMon();

    // Only trigger the nullify cured events if there are no more Neutralizing Gas Pokémon
    if (BattleField_GetNeutralizingGasMons() == 0) {
        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, BATTLE_NEUTRALIZING_GAS_END_MSGID);
        BattleHandler_PopWork(serverFlow, message);

        for (u8 i = 0; i < 24; ++i) {
            BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
            if (affectedMon) {

                u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, BattleMon_GetID(affectedMon));
                // If it's currently in the battlefield
                if (affectedPos != NULL_BATTLE_POS &&
                    !BattleMon_IsFainted(affectedMon)) {

                    ServerEvent_AbilityNullifyCured(serverFlow, affectedMon);
                }
            }
        }
    }
}
extern "C" void HandlerNeutralizingGasStart(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (IS_NOT_NEW_EVENT) return;

    if (pokemonSlot == BattleEventVar_GetValue(NEW_VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 1u, BATTLE_NEUTRALIZING_GAS_START_MSGID);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);

        // Only trigger the nullify events if Neutralizing Gas is not already active
        if (BattleField_GetNeutralizingGasMons() == 0) {
            for (u8 i = 0; i < 24; ++i) {
                BattleMon* affectedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
                if (affectedMon) {

                    u32 affectedPos = Handler_PokeIDToPokePos(serverFlow, BattleMon_GetID(affectedMon));
                    // If it's currently in the battlefield
                    if (affectedPos != NULL_BATTLE_POS &&
                        !BattleMon_IsFainted(affectedMon)) {

                        // Neutralizing Gas can't be neutralized by another Nutralizing Gas
                        // but since it can be neutralized by Gastro Acid it has a EVENT_ABILITY_NULLIFIED event
                        // and thus we have to make an special case here
                        if (BattleMon_GetValue(affectedMon, VALUE_ABILITY) != ABIL_NEUTRALIZING_GAS) {
                            ServerEvent_AbilityNullified(serverFlow, affectedMon);
                        }
                    }
                }
            }
        }

        BattleField_AddNeutralizingGasMon();
    }
}
extern "C" void HandlerNeutralizingGasEnd(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        NeutralizingGasEnd(serverFlow, pokemonSlot);
    }
}
extern "C" void HandlerNeutralizingGasEndFainted(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) &&
        BattleMon_IsFainted(Handler_GetBattleMon(serverFlow, pokemonSlot))) {
        NeutralizingGasEnd(serverFlow, pokemonSlot);
    }
}
BattleEventHandlerTableEntry NeutralizingGasHandlers[]{
    {EVENT_SWITCH_IN_PRIO, HandlerNeutralizingGasStart},
    {EVENT_SWITCH_OUT_END, HandlerNeutralizingGasEnd},
    {EVENT_BEFORE_ABILITY_CHANGE, HandlerNeutralizingGasEnd},
    {EVENT_ABILITY_NULLIFIED, HandlerNeutralizingGasEnd},
    {EVENT_NOTIFY_FAINTED, HandlerNeutralizingGasEndFainted},
};
extern "C" BattleEventHandlerTableEntry * EventAddNeutralizingGas(u32 * handlerAmount) {
    *handlerAmount = ARRAY_COUNT(NeutralizingGasHandlers);
    return NeutralizingGasHandlers;
}
#endif // EXPAND_ABILITIES