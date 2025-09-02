#include "settings.h"
#include "BattleEngine.h"
#include "GameVariables.h"
#include "include/Battlefield.h"

#include "include/AbilityExpansion.h"
#include "include/ItemExpansion.h"
#include "include/MoveExpansion.h"

#include "system/game_input.h"

#include "kDLL.h"

#include "server_flow.h"
#include "server_events.h"
#include "handler_params.h"

#include "moves.h"
#include "battle_events.h"
#include "types.h"
#include "abilities.h"
#include "field_effects.h"
#include "side_effects.h"
#include "personal.h"
#include "items.h"
#include "pos_effects.h"

#define MAX_LOADED_DLLS 72
struct LoadedDll
{
    const char* name;
    u32 count;
};

// List of loaded DLLs in the current battle
// Loaded in [FieldEffectEvent_AddItem], [SideEffectEvent_AddItem], [AbilityEvent_AddItem], [ItemEvent_AddItem], [MoveEvent_AddItem]
// Freed in [BattleEventItem_Remove]
// Reset in [BattleField_Free]
LoadedDll loadedDlls[MAX_LOADED_DLLS];
extern "C" void ClearDll(u32 idx) {
    loadedDlls[idx].name = nullptr;
    loadedDlls[idx].count = 0;
}
extern "C" void ClearLoadedDlls() {
#if LIBRARY_LOAD_DEBUG
    DPRINT("Clearing Dlls!\n");
#endif
    for (u32 dllIdx = 0; dllIdx < MAX_LOADED_DLLS; ++dllIdx)
        ClearDll(dllIdx);
}
#if LIBRARY_LOAD_DEBUG
extern "C" void PrintLoadedDlls() {
    for (u8 dllIdx = 0; dllIdx < 24; ++dllIdx) {

        if (loadedDlls[dllIdx].name == nullptr) {
            DPRINT("--- \n");
        }
        else {
            DPRINTF("%s (%d) \n", loadedDlls[dllIdx].name, loadedDlls[dllIdx].count);
        }
    }
}
#endif

extern "C" int strcmp(const char* str1, const char* str2);
extern "C" b32 LoadDll(const char* dllName)
{
    if (dllName == nullptr)
        return 1;

    for (u8 dllIdx = 0; dllIdx < MAX_LOADED_DLLS; ++dllIdx)
    {
        if (loadedDlls[dllIdx].name == nullptr)
        {
            k::dll::LibraryHandle handle = k::dll::LoadLibrary(dllName);
            if (!handle)
            {
#if LIBRARY_LOAD_DEBUG
                DPRINTF("Failed to load %s! \n", dllName);
#endif
                return 0;
            }
#if LIBRARY_LOAD_DEBUG
            DPRINTF("Loaded %s! \n", dllName);
#endif
            loadedDlls[dllIdx].name = dllName;
            loadedDlls[dllIdx].count = 1;
            break;
        }
        if (strcmp(loadedDlls[dllIdx].name, dllName) == 0)
        {
#if LIBRARY_LOAD_DEBUG
            DPRINTF("Added %s! \n", dllName);
#endif
            ++loadedDlls[dllIdx].count;
            break;
        }
    }
    return 1;
}
extern "C" void FreeDll(const char* dllName) {
    if (dllName == nullptr)
        return;

    for (u8 dllIdx = 0; dllIdx < MAX_LOADED_DLLS; ++dllIdx) {
        if (loadedDlls[dllIdx].name == nullptr)
            break;

        if (strcmp(loadedDlls[dllIdx].name, dllName) == 0) {
            
            --loadedDlls[dllIdx].count;
            if (loadedDlls[dllIdx].count == 0) {
                k::dll::ReleaseLibrary(loadedDlls[dllIdx].name);
#if LIBRARY_LOAD_DEBUG
                DPRINTF("Released %s! \n", dllName);
#endif
                for (; dllIdx < MAX_LOADED_DLLS; ++dllIdx) {
                    if (dllIdx == MAX_LOADED_DLLS - 1 || 
                        loadedDlls[dllIdx + 1].name == nullptr) {
                        ClearDll(dllIdx);
                        break;
                    }
                        
                    loadedDlls[dllIdx] = loadedDlls[dllIdx + 1];
                }
            }
#if LIBRARY_LOAD_DEBUG
            else {
                DPRINTF("Substracted %s! \n", dllName);
            }
#endif
            return;
        }
    }
#if LIBRARY_LOAD_DEBUG
    DPRINTF("Library %s was not loaded! \n", dllName);
#endif
}
extern "C" void FreeLoadedDlls() {
    // Free all the loaded code
    for (u8 dllIdx = 0; dllIdx < MAX_LOADED_DLLS; ++dllIdx)
    {
        if (loadedDlls[dllIdx].name == nullptr)
            break;

        k::dll::ReleaseLibrary(loadedDlls[dllIdx].name);
#if LIBRARY_LOAD_DEBUG
        DPRINTF("Freed %s!\n", loadedDlls[dllIdx]);
#endif
        loadedDlls[dllIdx].name = nullptr;
        loadedDlls[dllIdx].count = 0;
    }
    ClearLoadedDlls();
}

extern "C" b32 SearchArray(const u32* const arr, const u32 arrSize, const u32 value) {
    for (u32 i = 0; i < arrSize; ++i) {
        if (arr[i] == value) {
            return 1;
        }
    }
    return 0;
}
#define SEARCH_ARRAY(arr, value) SearchArray(arr, ARRAY_COUNT(arr), value)

#if ADD_LEVEL_CAP || DISABLE_EVS

extern "C" u32 CalcBaseExpGain(BattleMon * defeatedMon, u32 keySystemLevelAdjust);
extern "C" u32 ScaleExpGainedByLevel(BattleMon * battleMon, u32 expToAdd, u32 currentLevel, u32 defeatedMonLevel);
extern "C" u32 PassPower_ApplyEXP(u32 baseExp);
extern "C" void AddEVs(BattleMon * battleMon, BattleMon * defeatedMon, CalcExpWork * calcExpWork);
extern "C" u32 PML_UtilGetPkmLvExp(u16 species, u16 form, int level);

extern "C" void THUMB_BRANCH_SAFESTACK_AddExpAndEVs(ServerFlow * serverFlow, BattleParty * party, BattleMon * defeatedMon, CalcExpWork * partyCalcExpWork) {
    BtlSetup* btlSetup = MainModule_GetBtlSetup(serverFlow->mainModule);

    u32 baseExp = CalcBaseExpGain(defeatedMon, btlSetup->keySystemLevelAdjust);
    if (MainModule_GetBattleType(serverFlow->mainModule) == BtlType_Trainer) {
        baseExp = 15 * baseExp / 10;
    }

    u32 currExpSlot = 0;
    while (currExpSlot < 6) {
        sys_memset(&partyCalcExpWork[currExpSlot], 0, 12);
        ++currExpSlot;
    }

    u32 monsInParty = BattleParty_GetPartyCount(party);
    u32 monsWithExpShare = 0;
    if (monsInParty) {
        u32 currMonInParty = 0;

        while (currMonInParty < monsInParty) {
            BattleMon* currPartyBattleMon = BattleParty_GetMonData(party, currMonInParty);

            if (!BattleMon_IsFainted(currPartyBattleMon) &&
                BattleMon_GetHeldItem(currPartyBattleMon) == ITEM_EXP_SHARE) {
                ++monsWithExpShare;
            }
            ++currMonInParty;
        }
    }

    if (monsWithExpShare) {
        u32 halfExp = baseExp / 2;
        baseExp -= halfExp;

        u32 splitSharedExp = halfExp / monsWithExpShare;
        if (!splitSharedExp) {
            splitSharedExp = 1;
        }

        for (u32 currMonInParty = 0; currMonInParty < monsInParty; ++currMonInParty) {
            BattleMon* currPartyBattleMon = BattleParty_GetMonData(party, currMonInParty);

            if (!BattleMon_IsFainted(currPartyBattleMon) &&
                BattleMon_GetHeldItem(currPartyBattleMon) == ITEM_EXP_SHARE) {
                partyCalcExpWork[currMonInParty].level = splitSharedExp;
            }
        }
    }

    u32 confrontedMons = BattleMon_GetConfrontRecCount(defeatedMon);
    u32 monsToShareExp = 0;
    if (confrontedMons > 0) {
        u32 currConfrontedMon = 0;

        while (currConfrontedMon < confrontedMons) {
            u32 confrontedSlot = BattleMon_GetConfrontedPokeID(defeatedMon, currConfrontedMon);

            BattleMon* confrontedMon = PokeCon_GetBattleMon(serverFlow->pokeCon, confrontedSlot);
            if (!BattleMon_IsFainted(confrontedMon)) {
                ++monsToShareExp;
            }
            ++currConfrontedMon;
        }
    }

    u32 splitBaseExp = baseExp / monsToShareExp;
    if (!(baseExp / monsToShareExp)) {
        splitBaseExp = 1;
    }

    for (u32 i = 0; i < monsInParty; ++i) {
        BattleMon* partyMon = BattleParty_GetMonData(party, i);

        if (!BattleMon_IsFainted(partyMon)) {
            int partySlot = BattleMon_GetID(partyMon);

            for (int currConfrontedMon = 0; currConfrontedMon < confrontedMons; ++currConfrontedMon) {
                if (partySlot == BattleMon_GetConfrontedPokeID(defeatedMon, currConfrontedMon)) {
                    partyCalcExpWork[i].level += splitBaseExp;
                }
            }
        }
    }

    for (u32 i = 0; i < monsInParty; i = ++i) {
        CalcExpWork* calcExpWork = &partyCalcExpWork[i];

        if (calcExpWork->level) {
            BattleMon* partyMon = BattleParty_GetMonData(party, i);

            u32 level = BattleMon_GetValue(partyMon, VALUE_LEVEL);
            u32 defeatedMonLevel = BattleMon_GetValue(defeatedMon, VALUE_LEVEL);
            if (btlSetup->keySystemLevelAdjust < 0) {
                defeatedMonLevel -= btlSetup->keySystemLevelAdjust;
            }
            calcExpWork->level = ScaleExpGainedByLevel(partyMon, calcExpWork->level, level, defeatedMonLevel);

            PartyPkm* partyPkm = BattleMon_GetSrcData(partyMon);
            void* trainerInfo = MainModule_GetPlayerStatus(serverFlow->mainModule);
            if (!IsTrainerOT(&partyPkm->Base, trainerInfo)) {
                u32 region = PokeParty_GetParam(partyPkm, PF_Region, 0);
                u32 expRatio = 0;

                if (region == TrainerInfo_GetRegion(trainerInfo)) {
                    expRatio = 6144;
                }
                else {
                    expRatio = 6963;
                }
                calcExpWork->level = fixed_round(calcExpWork->level, expRatio);
                calcExpWork->isBoosted = 1;
            }
            if (BattleMon_GetHeldItem(partyMon) == ITEM_LUCKY_EGG) {
                calcExpWork->level = fixed_round(calcExpWork->level, 6144);
                calcExpWork->isBoosted = 1;
            }
            u32 passPowerExp = PassPower_ApplyEXP(calcExpWork->level);
            calcExpWork->level = passPowerExp;
            if (passPowerExp > 100000) {
                calcExpWork->level = 100000;
            }

#if ADD_LEVEL_CAP
            u8 lvlCap = (u8)GetLvlCap();
            u8 currentLevel = partyMon->level;
            if (currentLevel >= lvlCap) {
                sys_memset(&partyCalcExpWork[i], 0, 12u);
            }
            else {
                u32 expToCap = PML_UtilGetPkmLvExp(partyMon->species, partyMon->form, lvlCap);
                expToCap -= partyMon->experience;

                if (partyCalcExpWork[i].level > expToCap) {
                    partyCalcExpWork[i].level = expToCap;
                }
            }
#endif
        }
    }

#if !DISABLE_EVS
    for (u32 i = 0; i < monsInParty; i = ++i) {
        if (partyCalcExpWork[i].level) {
            BattleMon* partyMon = BattleParty_GetPartyMember(party, i);
            AddEVs(partyMon, defeatedMon, &partyCalcExpWork[i]);
        }
    }
#endif
}

#endif // ADD_LEVEL_CAP || GIVE_EVS

#if DYNAMIC_SPEED

extern "C" u32 j_j_FaintRecord_GetCount_1(FaintRecord * faintRecord, u32 turn);
extern "C" u32 Handler_IsPosOpenForRevivedMon(ServerFlow * serverFlow);
extern "C" void SortActionOrderBySpeed(ServerFlow * serverFlow, ActionOrderWork * actionOrder, u32 remainingActions);
extern "C" b32 ActionOrder_InterruptReserve(ServerFlow * serverFlow, u32 pokemonSlot);
extern "C" u32 ActionOrder_SendToLast(ServerFlow * serverFlow, u32 pokemonSlot);

// Records if an InterruptAction event (After You) was triggered in the current turn
// - Used in [ServerFlow_ActOrderProcMain] & [BattleHandler_InterruptAction]
u8 interruptActionFlag = 0;

// Records the battleslot of the mons that have been sent last (Quash) in the current turn
// - Used in [PokeSet_SortBySpeedDynamic] & [BattleHandler_SendLast]
u8 sendLastSlots[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

extern "C" void SwapPokemonOrder(ActionOrderWork * actionOrder, u16 * speedStats, u8 * priority, u8 * eventPriority, u8 slowIdx, u8 fastIdx) {
    BattleMon* bufferMon = actionOrder[fastIdx].battleMon;
    BattleActionParam bufferAction = actionOrder[fastIdx].action;
    u32 buffer_speed = actionOrder[fastIdx].speed;
    u8 buffer_partyID = actionOrder[fastIdx].partyID;
    u8 buffer_done = actionOrder[fastIdx].done;
    u8 buffer_field_E = actionOrder[fastIdx].field_E;
    u8 buffer_field_F = actionOrder[fastIdx].field_F;
    u16 bufferSpeed = speedStats[fastIdx];
    u8 bufferPriority = priority[fastIdx];
    u8 bufferEventPriority = eventPriority[fastIdx];

    actionOrder[fastIdx].battleMon = actionOrder[slowIdx].battleMon;
    actionOrder[slowIdx].battleMon = bufferMon;

    actionOrder[fastIdx].action = actionOrder[slowIdx].action;
    actionOrder[slowIdx].action = bufferAction;

    actionOrder[fastIdx].speed = actionOrder[slowIdx].speed;
    actionOrder[slowIdx].speed = buffer_speed;

    actionOrder[fastIdx].partyID = actionOrder[slowIdx].partyID;
    actionOrder[slowIdx].partyID = buffer_partyID;

    actionOrder[fastIdx].done = actionOrder[slowIdx].done;
    actionOrder[slowIdx].done = buffer_done;

    actionOrder[fastIdx].field_E = actionOrder[slowIdx].field_E;
    actionOrder[slowIdx].field_E = buffer_field_E;

    actionOrder[fastIdx].field_F = actionOrder[slowIdx].field_F;
    actionOrder[slowIdx].field_F = buffer_field_F;

    speedStats[fastIdx] = speedStats[slowIdx];
    speedStats[slowIdx] = bufferSpeed;

    priority[fastIdx] = priority[slowIdx];
    priority[slowIdx] = bufferPriority;

    eventPriority[fastIdx] = eventPriority[slowIdx];
    eventPriority[slowIdx] = bufferEventPriority;
}

extern "C" void PokeSet_SortBySpeedDynamic(ServerFlow* serverFlow, ActionOrderWork * actionOrder, u8 firstIdx, u8 turnStart) {
    // Skip the Pokémons that have already moved.
    u8 startIdx = firstIdx;
    if (!turnStart)
        startIdx += 1;

    // "firstIdx" is the latest Pokémon to move.
    u8 pokeAmount = serverFlow->numActOrder - startIdx;

    // Reset the send last slots at the start of the turn.
    if (startIdx == 0)
        sys_memset(sendLastSlots, 0xFF, 6);

    if (pokeAmount > 1) {
        u16 speedStats[6];
        sys_memset(speedStats, 0, 12);

        u8 priority[6];
        sys_memset(priority, 0, 6);

        // 7 is the default, 6-0 are reserved for quash and 8 for escaping battle.
        u8 eventPriority[6];
        sys_memset(eventPriority, 7, 6);

        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i) {
            if (!BattleMon_IsFainted(actionOrder[i].battleMon)) {
                speedStats[i] = (u16)ServerEvent_CalculateSpeed(serverFlow, actionOrder[i].battleMon, 1);

                priority[i] = ACTION_ORDER_GET_PRIO(actionOrder, i);
                // Special priority takes into account item & ability prio boosts (1 = no added prio).
                int specialPriority = ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, i);
                priority[i] += (specialPriority - 1);

                for (u8 j = 0; j < 6; ++j) {
                    if (sendLastSlots[j] == 0xFF)
                        break;

                    // If the mon has been sent last we give them a
                    // "quash speed" from 6 - 0 (non-quashed mons have 7)
                    if (sendLastSlots[j] == actionOrder[i].battleMon->battleSlot)
                        eventPriority[i] = 6 - j;
                    // We don't break after finding a match, a mon can be quashed
                    // multiple times and the only one that matters is the last one
                }

                // This means the mon is making an escape attempt.
                // This makes it first, and overrides any other move priority.
                if (BattleAction_GetAction(&actionOrder[i].action) == 4)
                    eventPriority[i] = 8;
            }
            else {
                priority[i] = 0xFF;
            }
        }

        // We randomize the initial order to resolve speed ties.
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i) {
            u8 randomSpot = startIdx + (u8)BattleRandom(pokeAmount);

            if (randomSpot != i)
                SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, randomSpot);
        }

        u8 changes = 1;
        while (changes != 0) {
            changes = 0;

            for (u8 i = startIdx; i < serverFlow->numActOrder; ++i) {
                // Check if the first Pokémon is fainted.
                if (priority[i] != 0xFF) {
                    if (i + 1 < serverFlow->numActOrder) {
                        for (u8 j = i + 1; j < serverFlow->numActOrder; ++j) {
                            // Check if the second Pokémon is fainted.
                            if (priority[j] != 0xFF) {
                                u8 poke1EventPriority = eventPriority[i];
                                u8 poke2EventPriority = eventPriority[j];

                                if (poke2EventPriority > poke1EventPriority) {
                                    SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, j);
                                    ++changes;
                                }
                                else if (poke2EventPriority == poke1EventPriority) {
                                    u8 poke1Priority = priority[i];
                                    u8 poke2Priority = priority[j];

                                    if (poke2Priority > poke1Priority) {
                                        SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, j);
                                        ++changes;
                                    }
                                    else if (poke2Priority == poke1Priority) {
                                        u16 poke1Speed = speedStats[i];
                                        u16 poke2Speed = speedStats[j];

                                        if (poke2Speed > poke1Speed) {
                                            SwapPokemonOrder(actionOrder, speedStats, priority, eventPriority, i, j);
                                            ++changes;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

#if DYNAMIC_SPEED_DEBUG
        DPRINTF("ORDER(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", actionOrder[i].battleMon->battleSlot);
        DPRINTF("\nSPEED(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", speedStats[i]);
        DPRINTF("\nPRIORITY(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", priority[i]);
        DPRINTF("\nEVENT(%d):", startIdx);
        for (u8 i = startIdx; i < serverFlow->numActOrder; ++i)
            DPRINTF("%d.", eventPriority[i]);
        DPRINT("\n\n");
#endif
    }
}

extern "C" u32 THUMB_BRANCH_BattleHandler_InterruptAction(ServerFlow * serverFlow, HandlerParam_InterruptPoke * params) {
    if (!ActionOrder_InterruptReserve(serverFlow, params->pokeID))
    {
        return 0;
    }
    BattleHandler_SetString(serverFlow, &params->exStr);

    interruptActionFlag = 1;
    return 1;
}

extern "C" u32 THUMB_BRANCH_BattleHandler_SendLast(ServerFlow * serverFlow, HandlerParam_SendLast * params) {
    if (!ActionOrder_SendToLast(serverFlow, params->pokeID)) {
        return 0;
    }
    BattleHandler_SetString(serverFlow, &params->exStr);

    // Add the Pokémon slot to the send last slots.
    for (u8 i = 0; i < 6; ++i) {
        if (sendLastSlots[i] == 0xFF) {
            sendLastSlots[i] = params->pokeID;
            break;
        }
    }

    return 1;
}

#endif // DYNAMIC_SPEED

#if ADD_MEGA_EVOLUTION

#define UNTOGGLED_MEGA 0
#define OTHER_MEGA 0xFE
#define ALREADY_MEGA 0xFF
// Stores if this client has already used Mega-Evolution in the current fight
u8 megaEvolveFlag = UNTOGGLED_MEGA;

extern "C" void BattleAction_SetMegaEvolution(BattleActionParam* actionParam, u8 form)
{
    ((BattleAction_Fight*)actionParam)->pad = form;
}
extern "C" void BattleAction_ResetMegaEvolution(BattleActionParam* actionParam)
{
    ((BattleAction_Fight*)actionParam)->pad = 0;
}
extern "C" u8 BattleAction_CheckMegaEvolution(const BattleActionParam* actionParam)
{
    return ((BattleAction_Fight*)actionParam)->pad;
}

struct MegaEvolution
{
    SPECIES species;
    ITEM item;
    u8 form;
};
MegaEvolution megaevolutions[]{
    {PKM_VENUSAUR, ITEM_VENUSAURITE, 1},
    {PKM_CHARIZARD, ITEM_CHARIZARDITE_X, 1},
    {PKM_CHARIZARD, ITEM_CHARIZARDITE_Y, 2},
    {PKM_BLASTOISE, ITEM_BLASTOISINITE, 1},
    {PKM_BEEDRILL, ITEM_BEEDRILLITE, 1},
    {PKM_PIDGEOT, ITEM_PIDGEOTITE, 1},
    {PKM_ALAKAZAM, ITEM_ALAKAZITE, 1},
    {PKM_SLOWBRO, ITEM_SLOWBRONITE, 1},
    {PKM_GENGAR, ITEM_GENGARITE, 1},
    {PKM_KANGASKHAN, ITEM_KANGASKHANITE, 1},
    {PKM_PINSIR, ITEM_PINSIRITE, 1},
    {PKM_GYARADOS, ITEM_GYARADOSITE, 1},
    {PKM_AERODACTYL, ITEM_AERODACTYLITE, 1},
    {PKM_MEWTWO, ITEM_MEWTWONITE_X, 1},
    {PKM_MEWTWO, ITEM_MEWTWONITE_Y, 2},
    {PKM_AMPHAROS, ITEM_AMPHAROSITE, 1},
    {PKM_STEELIX, ITEM_STEELIXITE, 1},
    {PKM_SCIZOR, ITEM_SCIZORITE, 1},
    {PKM_HERACROSS, ITEM_HERACRONITE, 1},
    {PKM_HOUNDOOM, ITEM_HOUNDOOMINITE, 1},
    {PKM_TYRANITAR, ITEM_TYRANITARITE, 1},
    {PKM_SCEPTILE, ITEM_SCEPTILITE, 1},
    {PKM_BLAZIKEN, ITEM_BLAZIKENITE, 1},
    {PKM_SWAMPERT, ITEM_SWAMPERTITE, 1},
    {PKM_GARDEVOIR, ITEM_GARDEVOIRITE, 1},
    {PKM_SABLEYE, ITEM_SABLENITE, 1},
    {PKM_MAWILE, ITEM_MAWILITE, 1},
    {PKM_AGGRON, ITEM_AGGRONITE, 1},
    {PKM_MEDICHAM, ITEM_MEDICHAMITE, 1},
    {PKM_MANECTRIC, ITEM_MANECTITE, 1},
    {PKM_SHARPEDO, ITEM_SHARPEDONITE, 1},
    {PKM_CAMERUPT, ITEM_CAMERUPTITE, 1},
    {PKM_ALTARIA, ITEM_ALTARIANITE, 1},
    {PKM_BANETTE, ITEM_BANETTITE, 1},
    {PKM_ABSOL, ITEM_ABSOLITE, 1},
    {PKM_GLALIE, ITEM_GLALITITE, 1},
    {PKM_SALAMENCE, ITEM_SALAMENCITE, 1},
    {PKM_METAGROSS, ITEM_METAGROSSITE, 1},
    {PKM_LATIAS, ITEM_LATIASITE, 1},
    {PKM_LATIOS, ITEM_LATIOSITE, 1},
    {PKM_LOPUNNY, ITEM_LOPUNNITE, 1},
    {PKM_GARCHOMP, ITEM_GARCHOMPITE, 1},
    {PKM_LUCARIO, ITEM_LUCARIONITE, 1},
    {PKM_ABOMASNOW, ITEM_ABOMASITE, 1},
    {PKM_GALLADE, ITEM_GALLADITE, 1},
    {PKM_AUDINO, ITEM_AUDINITE, 1},
    {PKM_MEWTWO, ITEM_MEWTWONITE_X, 1},
    {PKM_NULL, ITEM_DIANCITE, 1}, // TODO: ADD DIANCIE
};
extern "C" u8 CanMegaEvolve(BattleMon* battleMon)
{
    for (u32 idx = 0; idx < ARRAY_COUNT(megaevolutions); ++idx)
    {
        MegaEvolution* mega = &(megaevolutions[idx]);
        if (mega->species == battleMon->species &&
            mega->item == battleMon->heldItem)
            return mega->form;
    }
    return 0;
}

extern "C" void BattleAction_SetNull(BattleActionParam* actionParam);
extern "C" void THUMB_BRANCH_LINK_BattleClient_ActionSelect_Root_0x58(BattleActionParam* actionParam)
{
    if (megaEvolveFlag != ALREADY_MEGA)
    {
        // Reset the Mega-Evolution flag if the deleted action was supposed to Mega-Evolve
        if (BattleAction_CheckMegaEvolution(actionParam))
            megaEvolveFlag = UNTOGGLED_MEGA; 
    }

    BattleAction_SetNull(actionParam);
}

extern "C" u32 BattleViewCmd_UI_SelectMove_Wait(BtlvCore* btlCore);
extern "C" u32 THUMB_BRANCH_LINK_BattleClient_ActionSelectFight_0x8E(BtlvCore* btlCore)
{
    if (megaEvolveFlag != ALREADY_MEGA && 
        megaEvolveFlag != OTHER_MEGA)
    {
        if ((GCTX_HIDGetPressedKeys() & KEY_START) == KEY_START)
        {
            // Toggle the Mega-Evolution flag
            if (megaEvolveFlag == UNTOGGLED_MEGA)
            {
                megaEvolveFlag = CanMegaEvolve(btlCore->activeMon);
                if (megaEvolveFlag)
                    DPRINT("MEGA-EVOLUTION!!!!!!!!! \n");
            }
            else
            {
                megaEvolveFlag = UNTOGGLED_MEGA;
                DPRINT("DEACTIVATED MEGA-EVOLUTION \n");
            }
        }
    }

    return BattleViewCmd_UI_SelectMove_Wait(btlCore);
}

extern "C" u32 THUMB_BRANCH_LINK_BattleClient_ActionSelectFight_0x9A(BattleActionParam* actionParam)
{
    u32 action = BattleAction_GetAction(actionParam);

    if (action)
        if (megaEvolveFlag != ALREADY_MEGA && 
            megaEvolveFlag != OTHER_MEGA && 
            megaEvolveFlag != UNTOGGLED_MEGA)
        {
            // Set the Mega-Evolution flag in the action data
            BattleAction_SetMegaEvolution(actionParam, megaEvolveFlag);
            megaEvolveFlag = OTHER_MEGA;
        }
    
    return action;
}

// Make sure this function doesn't override the pad value
extern "C" void THUMB_BRANCH_BattleAction_SetFightParam(BattleActionParam* actionParam, u32 moveID, u32 targePos)
{
    actionParam->baFight.cmd = 1;
    actionParam->baFight.targetPos = targePos;
    actionParam->baFight.moveID = moveID;
}

// Mega-Evolution - Change ability after Mega-Evolving
extern "C" void ServerControl_UnnerveAction(ServerFlow* serverFlow, BattleMon* battleMon, ABILITY ability);
extern "C" bool THUMB_BRANCH_BattleHandler_ChangeForm(ServerFlow* serverFlow, HandlerParam_ChangeForm* params)
{
    BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->pokeID);
    if (BattleMon_IsFainted(battleMon)
        || BattleMon_TransformCheck(battleMon)
        || BattleMon_GetValue(battleMon, VALUE_FORM) == params->newForm)
        return 0;

    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0)
        ServerDisplay_AbilityPopupAdd(serverFlow, battleMon);

    BattleMon_ChangeForm(battleMon, params->newForm);
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_ChangeForm, params->pokeID, params->newForm);
    BattleHandler_SetString(serverFlow, &params->exStr);

    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0)
        ServerDisplay_AbilityPopupRemove(serverFlow, battleMon);

    u8 megaevolution = CanMegaEvolve(battleMon);
    if (megaevolution && megaevolution == battleMon->form) 
    {
        ABILITY prevAbility = BattleMon_GetValue(battleMon, VALUE_ABILITY);
        ABILITY ability = PokeParty_GetParam(battleMon->partySrc, PF_Ability, 0);

        // Call change ability events to end any effect of the previous ability
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);
        ServerEvent_ChangeAbilityBefore(serverFlow, battleMon->battleSlot, prevAbility, ability);
        HEManager_PopState(&serverFlow->HEManager, HEID);

        // Remove the old ability event
        AbilityEvent_RemoveItem(battleMon);
        // Update the BattleMon ability
        BattleMon_ChangeAbility(battleMon, ability);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_ChangeAbility, params->pokeID, ability);
        // Add the new ability event
        AbilityEvent_AddItem(battleMon);

        // Call change ability events to start any effect of the new ability
        HEID = HEManager_PushState(&serverFlow->HEManager);
        ServerEvent_ChangeAbilityAfter(serverFlow, battleMon->battleSlot);
        HEManager_PopState(&serverFlow->HEManager, HEID);

        if (!BattleMon_CheckIfMoveCondition(battleMon, CONDITION_GASTROACID))
        {
            if (prevAbility == ABIL_KLUTZ)
                ServerControl_CheckItemReaction(serverFlow, battleMon, 0);
            
            ServerControl_UnnerveAction(serverFlow, battleMon, prevAbility);
        }
    }
    
    return 1;
}

#endif // ADD_MEGA_EVOLUTION

#if DYNAMIC_SPEED || EXPAND_ABILITIES || ADD_MEGA_EVOLUTION
#include "include/ActionOrderManager.h"

#if EXPAND_ABILITIES

// Defines if a switch should be made after the turn has been processed
// The vanilla game does not have logic to switch after all actions have been performed
// like for example after receiving damage from a status condition
// - Set in [HandlerEmergencyExitSwitchEnd]
// - Reset & used in [ServerFlow_ActOrderProcMain]
u8 endTurnSwitchFlag = 0;

extern "C" b32 CheckEndTurnSwitchFlag()
{
    return endTurnSwitchFlag;
}
extern "C" void SetEndTurnSwitchFlag()
{
    endTurnSwitchFlag = 1;
}
extern "C" void ResetEndTurnSwitchFlag()
{
    endTurnSwitchFlag = 0;
}

extern "C" u32 CommonGetAllyPos(ServerFlow * serverFlow, u32 battlePos) {
    BattleStyle battleStyle = BtlSetup_GetBattleStyle(serverFlow->mainModule);
    if (battleStyle != BTL_STYLE_DOUBLE && battleStyle != BTL_STYLE_TRIPLE) {
        return 6;
    }

    u8 isEnemy = battlePos & 1;
    if (isEnemy) {
        battlePos -= 1;
    }

    u32 allyPos = 0;
    if (battleStyle != BTL_STYLE_TRIPLE) {
        if (battlePos == 0) {
            allyPos = 2;
        }
        else {
            allyPos = 0;
        }
    }
    else {
        if (IsCenterInTripleBattle(battlePos)) {
            allyPos = BattleRandom(2) * 4;
        }
        else {
            allyPos = 2;
        }
    }

    return allyPos + isEnemy;
}

#endif

// Stores data of extra action generated by Dancer, Instruct...
// - Set in [HandlerDancerCheckMove]
// - Reset and used in [ServerFlow_ActOrderProcMain]
ActionOrderWork extraActionOrder[6];

extern "C" void ShiftExtraActionOrders()
{
    for (u8 i = 5; i != 0; --i) {
        sys_memcpy(&extraActionOrder[i - 1], &extraActionOrder[i], sizeof(ActionOrderWork));
    }
    sys_memset(&extraActionOrder[0], 0, sizeof(ActionOrderWork));
}
extern "C" ActionOrderWork* GetExtraActionOrder(u32 actionIdx)
{
    return extraActionOrder + actionIdx;
}

// Defines if an extra action is taking place
// - Set and reset in [ServerFlow_ActOrderProcMain]
u8 extraActionFlag = 0;

extern "C" b32 CheckExtraActionFlag()
{
    return extraActionFlag;
}
extern "C" void SetExtraActionFlag()
{
    extraActionFlag = 1;
}
extern "C" void ResetExtraActionFlag()
{
    extraActionFlag = 0;
}

extern "C" int THUMB_BRANCH_ServerFlow_ActOrderProcMain(ServerFlow * serverFlow, u32 currentActionIdx) {
    u32 procAction = 0;
    ActionOrderWork* actionOrderWork = serverFlow->actionOrderWork;

    for (u8 i = 0; i < 6; ++i) {
        sys_memset(&extraActionOrder[i], 0, sizeof(ActionOrderWork));
    }

    // Re-calc speed after switch-ins
    PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 1);

#if ADD_MEGA_EVOLUTION
        
    // If a Mega-Evolution was succesful, lock the flag for the rest of the battle
    if (megaEvolveFlag == OTHER_MEGA)
        megaEvolveFlag = ALREADY_MEGA;

    u8 hasMegaEvolved = 0;
    for (u32 actionIdx = 0; actionIdx < serverFlow->numActOrder; ++actionIdx)
    {
        BattleActionParam* actionParam = &(actionOrderWork[actionIdx].action);
        u8 megaevolution = BattleAction_CheckMegaEvolution(actionParam);
        
        // Check if the fight actions have the Mega-Evolution flag set
        if (BattleAction_GetAction(actionParam) == 1 &&
            megaevolution)
        {
            u8 pokemonSlot = actionOrderWork[actionIdx].battleMon->battleSlot;  
            u16 item = actionOrderWork[actionIdx].battleMon->heldItem;

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 2u, BATTLE_MEGA_RING_MSGID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_AddArg(&message->str, item);
            BattleHandler_PopWork(serverFlow, message);

            HandlerParam_ChangeForm* changeForm;
            changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
            changeForm->pokeID = pokemonSlot;
            changeForm->newForm = megaevolution;
            BattleHandler_StrSetup(&changeForm->exStr, 2u, BATTLE_MEGA_EVOLVE_MSGID);
            BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
            BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, changeForm);

            // Clear the Mega-Evolution flag
            BattleAction_ResetMegaEvolution(actionParam);

            hasMegaEvolved = 1;
        }
    }
    // Re-calc speed if any Pokémon has Mega-Evolved
    if (hasMegaEvolved)
        PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 1);

#endif
    
    while (currentActionIdx < serverFlow->numActOrder || extraActionOrder[0].battleMon != nullptr) {
        ActionOrderWork* currentActionOrder = &actionOrderWork[currentActionIdx];
        // Swap the current action if an extra one has been scheduled
        u8 isExtraAction = 0;
        if (extraActionOrder[0].battleMon != nullptr) {
            isExtraAction = 1;
            currentActionOrder = &extraActionOrder[0];

            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_ACTIONSTART);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_ACTIONDONE);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_MOVEPROCDONE);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_MOVED);
            Turnflag_Clear(currentActionOrder->battleMon, TURNFLAG_USINGFLING);

            SetExtraActionFlag();
        }

        if (!CheckExtraActionFlag()) {
            // [1 = fight] [2 = item] [3 = switch] [4 = run]
            u32 action = BattleAction_GetAction(&currentActionOrder->action);
            // I think this recals speed in rotation battles
            if (procAction == 6 && action != 6) {
                ServerControl_CheckActivation(serverFlow);
                SortActionOrderBySpeed(serverFlow, currentActionOrder, (u32)serverFlow->numActOrder - currentActionIdx);
            }
        }
#if EXPAND_ABILITIES
        // Skip action if the Pokémon is invulnerable because of Commander
        b32 isCommander = (!BattleMon_CheckIfMoveCondition(currentActionOrder->battleMon, CONDITION_CHARGELOCK) &&
            BattleMon_GetHideCondition(currentActionOrder->battleMon) == CONDITIONFLAG_SHADOWFORCE);
        if (!isCommander) {
#endif
            procAction = ActionOrder_Proc(serverFlow, currentActionOrder);
#if EXPAND_ABILITIES
        }
#endif
        // Skip speed calcs if After You was used
        if (interruptActionFlag != 1) {
            // Re-calc speed
            PokeSet_SortBySpeedDynamic(serverFlow, actionOrderWork, (u8)currentActionIdx, 0);
        }
        interruptActionFlag = 0;

        // Move the next extra action to the first slot
        if (isExtraAction) {
            for (u8 i = 0; i < 5; ++i) {
                sys_memcpy(&extraActionOrder[i + 1], &extraActionOrder[i], sizeof(ActionOrderWork));
            }
            sys_memset(&extraActionOrder[5], 0, sizeof(ActionOrderWork));
           
            ResetExtraActionFlag();
        }

        u32 getExp = ServerControl_CheckExpGet(serverFlow);
        b32 matchup = ServerControl_CheckMatchup(serverFlow);

        // Stop the turn if the battle ends
        if (matchup) {
            serverFlow->flowResult = 4;
            // Don't advance an action if we have an extra action
            if (isExtraAction) {
                return currentActionIdx;
            }
            else {
                return currentActionIdx + 1;
            }
        }

        // Stop the turn if a new Pokémon has to enter the battle
        if (serverFlow->flowResult == 6 || serverFlow->flowResult == 1) {
            // Don't advance an action if we have an extra action
            if (isExtraAction) {
                return currentActionIdx;
            }
            else {
                return currentActionIdx + 1;
            }
        }

        // Stop the turn if a Pokémon died but the battle is not over
        if (getExp) {
            serverFlow->flowResult = 3;
            // Don't advance an action if we have an extra action
            if (isExtraAction) {
                return currentActionIdx;
            }
            else {
                return currentActionIdx + 1;
            }
        }

        // Don't advance an action if we have an extra action
        if (!isExtraAction) {
            ++currentActionIdx;
        }
    }

    // Only TurnCheck if the turn is completely over
    if (!serverFlow->flowResult) {
        u32 turnCheck = ServerControl_TurnCheck(serverFlow);
        
        // Battle is over
        if (ServerControl_CheckMatchup(serverFlow)) {
            serverFlow->flowResult = 4;
            return serverFlow->numActOrder;
        }
        
        // A Pokémon fainted during the TurnCheck
        if (turnCheck) {
            serverFlow->flowResult = 3;
            return serverFlow->numActOrder;
        }

#if EXPAND_ABILITIES
        if (CheckEndTurnSwitchFlag()) {
            ResetEndTurnSwitchFlag();

            // Skip TurnCheck if a Pokémon has to enter the battle during the TurnCheck
            serverFlow->turnCheckSeq = 7;

            serverFlow->flowResult = 1;
            return serverFlow->numActOrder;
        }
#endif

        u32 faintedCount = j_j_FaintRecord_GetCount_1(&serverFlow->faintRecord, 0);
        if (Handler_IsPosOpenForRevivedMon(serverFlow) || faintedCount) {
            ServerFlow_ReqChangePokeForServer(serverFlow, &serverFlow->field_4CE);
            ServerDisplay_IllusionSet(serverFlow, &serverFlow->field_4CE);
            serverFlow->flowResult = 2;
            return serverFlow->numActOrder;
        }

        serverFlow->flowResult = 0;
    }
    return serverFlow->numActOrder;
}

#endif // DYNAMIC_SPEED || EXPAND_ABILITIES

#if EXPAND_FIELD_EFFECTS

// FIELD EFFECT EXPANSION

// The battle engine stores 2 BattleField structs
//  - One is dyanamically allocated using BattleField_Init
//  - The other one is in a hardcoded memory address [0x21DD928] I don't know when this gets 
//    allocated, this means I can't make room for the extra data this patch adds to the struct.
// This code disregards the hardcoded instance of the data
BattleFieldExt* g_BattleField;

#if EXPAND_ABILITIES

extern "C" b32 BattleField_CheckParentalBondFlag() {
    return g_BattleField->parentalBondFlag;
}
extern "C" void BattleField_SetParentalBondFlag() {
    g_BattleField->parentalBondFlag = 1;
}
extern "C" void BattleField_ResetParentalBondFlag() {
    g_BattleField->parentalBondFlag = 0;
}

extern "C" u32 BattleField_GetAuraBreakMons() {
    return g_BattleField->auraBreakMons;
}
extern "C" void BattleField_AddAuraBreakMon() {
    ++g_BattleField->auraBreakMons;
}
extern "C" void BattleField_RemoveAuraBreakMon() {
    --g_BattleField->auraBreakMons;
}

extern "C" u32 BattleField_GetExtremeWeatherMons() {
    return g_BattleField->extremeWeatherMons;
}
extern "C" void BattleField_AddExtremeWeatherMon() {
    ++g_BattleField->extremeWeatherMons;
}
extern "C" void BattleField_RemoveExtremeWeatherMon() {
    --g_BattleField->extremeWeatherMons;
}
extern "C" void BattleField_ResetExtremeWeatherMons() {
    g_BattleField->extremeWeatherMons = 0;
}

extern "C" u32 BattleField_GetSubstituteDamage(u32 battleSlot)
{
    return g_BattleField->actionSubstituteDamage[battleSlot];
}
extern "C" void BattleField_SetSubstituteDamage(u32 battleSlot, u32 value)
{
    g_BattleField->actionSubstituteDamage[battleSlot] = value;
}

extern "C" b32 BattleField_CheckEmergencyExitFlag(u32 battleSlot) {
    return (g_BattleField->emergencyExitFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetEmergencyExitFlag(u32 battleSlot) {
    g_BattleField->emergencyExitFlag |= (1 << battleSlot);
}
extern "C" void BattleField_ResetEmergencyExitFlag(u32 battleSlot) {
    g_BattleField->emergencyExitFlag &= ~(1 << battleSlot);
}

extern "C" b32 BattleField_CheckBattleBondFlag(u32 battleSlot) {
    return (g_BattleField->battleBondFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetBattleBondFlag(u32 battleSlot) {
    g_BattleField->battleBondFlag |= (1 << battleSlot);
}

extern "C" void HandlerAddExtremeWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather) {
    if (BattleField_GetExtremeWeatherMons()) {
        if (BattleField_GetWeather() == weather) {
            // If this Extreme Weather is already in the field, just add 1 to the counter
            BattleField_AddExtremeWeatherMon();
            return;
        }
        else {
            // If a different Extreme Weather is already in the field, reset the counter
            BattleField_ResetExtremeWeatherMons();
        }
    }

    HandlerParam_ChangeWeather* changeWeather;
    changeWeather = (HandlerParam_ChangeWeather*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_WEATHER, pokemonSlot);
    changeWeather->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
    changeWeather->weather = weather;
    changeWeather->turn = INFINITE_FIELD_TURNS;
    // This sets the weather in its Extreme form
    changeWeather->field_7 = 1;
    BattleHandler_PopWork(serverFlow, changeWeather);

    // Add 1 to the Extreme Weather counter
    BattleField_AddExtremeWeatherMon();
}
extern "C" void HandlerRemoveExtremeWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather) {
    WEATHER currentWeather = BattleField_GetWeather();
    if (currentWeather == weather && BattleField_GetExtremeWeatherMons()) {
        // Substract 1 to the Extreme Weather counter
        BattleField_RemoveExtremeWeatherMon();

        // If there are no more Pokémon invoking Extreme Weather remove it
        if (!BattleField_GetExtremeWeatherMons()) {
            BattleField_EndWeather(g_BattleField);
            // I set the 8th bit in the weather variable to indicate that it is Extreme Weather
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_WeatherEnd, weather | 0x80);
            ServerControl_ChangeWeatherAfter(serverFlow, WEATHER_NULL);
        }
    }
}
extern "C" void HandlerExtremeWeatherImmunity(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather, POKE_TYPE type, u16 msgID) {
    if (ServerEvent_GetWeather(serverFlow) == weather && BattleField_GetExtremeWeatherMons()) {
        if (BattleEventVar_GetValue(VAR_MOVE_TYPE) == type &&
            PML_MoveGetCategory(BattleEventVar_GetValue(VAR_MOVE_ID)) != CATEGORY_STATUS) {
            BattleEventVar_RewriteValue(VAR_NO_EFFECT_FLAG, 1);

            HandlerParam_Message* message;
            message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
            BattleHandler_StrSetup(&message->str, 1u, msgID);
            BattleHandler_AddArg(&message->str, pokemonSlot);
            BattleHandler_PopWork(serverFlow, message);
        }
    }
}

extern "C" b32 CanWeatherBeChanged(ServerFlow * serverFlow, WEATHER weather, u32 turns, u8 isExtreme) {
    if (weather > WEATHER_STRONGWINDS) {
        return 0;
    }

    // Regular weather can't override Extreme weather
    if (BattleField_GetExtremeWeatherMons() && !isExtreme) {
        return 0;
    }

    // Allow the weather to change to it's extreme variant
    if (isExtreme && weather == BattleField_GetWeather()) {
        return 1;
    }

    return weather != BattleField_GetWeather() || turns == INFINITE_FIELD_TURNS && BattleField_GetWeatherTurns() != INFINITE_FIELD_TURNS;
}
extern "C" void ChangeWeather(ServerFlow * serverFlow, WEATHER weather, u32 turns, u8 isExtreme)
{
    BattleField_SetWeather(weather, turns);

    u8 serverWeather = (u8)weather;
    if (isExtreme) {
        // I set the 8th bit in the weather variable to indicate that it is Extreme Weather
        serverWeather |= 0x80;
    }
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_WeatherStart, serverWeather, turns);

    ServerControl_ChangeWeatherAfter(serverFlow, weather);
}

#if GEN9_INTREPID_SWORD
extern "C" b32 BattleField_CheckIntrepidSwordFlag(u32 battleSlot) {
    return (g_BattleField->intrepidSwordFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetIntrepidSwordFlag(u32 battleSlot) {
    g_BattleField->intrepidSwordFlag |= (1 << battleSlot);
}
#endif

#if GEN9_DAUNTLESS_SHIELD
extern "C" b32 BattleField_CheckDauntlessShieldFlag(u32 battleSlot) {
    return (g_BattleField->dauntlessShieldFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetDauntlessShieldFlag(u32 battleSlot) {
    g_BattleField->dauntlessShieldFlag |= (1 << battleSlot);
}
#endif

extern "C" u32 BattleField_GetNeutralizingGasMons() {
    return g_BattleField->neutralizingGasMons;
}
extern "C" void BattleField_AddNeutralizingGasMon() {
    ++g_BattleField->neutralizingGasMons;
}
extern "C" void BattleField_RemoveNeutralizingGasMon() {
    --g_BattleField->neutralizingGasMons;
}

extern "C" b32 BattleField_GetAsOnePopUpFlags()
{
    return g_BattleField->asOneAbilityPopUpFlags;
}
extern "C" void BattleField_SetAsOnePopUpFlags(u8 flags)
{
    g_BattleField->asOneAbilityPopUpFlags = flags;
}

extern "C" void BattleField_AddPairedDondozo(u32 battleSlot, u32* index) {
    for (u32 i = 0; i < 4; ++i) {
        if (g_BattleField->pairedDondozos[i] == BATTLE_MAX_SLOTS) {
            g_BattleField->pairedDondozos[i] = battleSlot;
            *index = i;
            return;
        }
    }
}
extern "C" b32 BattleField_IsDondozoPaired(u32 battleSlot) {
    for (u32 i = 0; i < 4; ++i) {
        if (g_BattleField->pairedDondozos[i] == battleSlot) {
            return 1;
        }
    }
    return 0;
}
extern "C" u8 BattleField_GetDondozoFromPair(u32 pairIndex) {
    return g_BattleField->pairedDondozos[pairIndex];
}

extern "C" b32 BattleField_CheckParadoxAbilityFlag(u32 battleSlot) {
    return (g_BattleField->paradoxAbilityFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetParadoxAbilityFlag(u32 battleSlot) {
    g_BattleField->paradoxAbilityFlag |= (1 << battleSlot);
}
extern "C" void BattleField_ResetParadoxAbilityFlag(u32 battleSlot) {
    g_BattleField->paradoxAbilityFlag &= ~(1 << battleSlot);
}

extern "C" b32 BattleField_CheckSupersweetSyrupFlag(u32 battleSlot) {
    return (g_BattleField->supersweetSyrupFlag >> battleSlot) & 1;
}
extern "C" void BattleField_SetSupersweetSyrupFlag(u32 battleSlot) {
    g_BattleField->supersweetSyrupFlag |= (1 << battleSlot);
}

extern "C" b32 SwitchedInThisTurn(ServerFlow * serverFlow, BattleMon * battleMon) {
    if (serverFlow->turnCount == 0) {
        for (u8 i = 0; i < 6; ++i) {
            if (g_BattleField->firstTurnMons[i] == battleMon->battleSlot) {
                // A Pokémon that started the battle doesn't count as just switched in
                return 0;
            }
        }
    }

    return !battleMon->turnCount;
}

#endif // EXPAND_ABILITIES

#if EXPAND_MOVES

extern "C" b32 BattleField_CheckConsumedBerryFlag(u32 battleSlot) {
    return (g_BattleField->consumedBerryFlags >> battleSlot) & 1;
}
extern "C" void BattleField_SetConsumedBerryFlag(u32 battleSlot) {
    g_BattleField->consumedBerryFlags |= (1 << battleSlot);
}

extern "C" POKE_TYPE BattleField_GetExtraType(u32 battleSlot) {
    return g_BattleField->extraTypes[battleSlot];
}
extern "C" void BattleField_SetExtraType(u32 battleSlot, POKE_TYPE type) {
    g_BattleField->extraTypes[battleSlot] = (u8)type;
}

#endif // EXPAND_MOVES


extern "C" TERRAIN BattleField_GetTerrain(BattleFieldExt * battleField) {
    return battleField->terrain;
}
extern "C" u32 BattleField_GetTerrainTurns(BattleFieldExt * battleField) {
    return battleField->terrainTurns;
}
extern "C" void BattleField_SetTerrain(BattleFieldExt * battleField, TERRAIN terrain, u32 turns) {
    battleField->terrain = terrain;
    battleField->terrainTurns = turns;
}
extern "C" void BattleField_EndTerrain(BattleFieldExt * battleField) {
    battleField->terrain = 0;
    battleField->terrainTurns = 0;
}

extern "C" u32 TerrainPowerMod(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon, TERRAIN terrain, POKE_TYPE type)
{
    switch (terrain) {
    case TERRAIN_ELECTRIC:
        if (type == TYPE_ELEC &&
            !ServerControl_CheckFloating(serverFlow, attackingMon, 1)) {
#if TERRAIN_GEN7_DMG
            return 6.144;
#endif
            return 5325;
        }
        break;
    case TERRAIN_GRASSY:
        if (type == TYPE_GRASS &&
            !ServerControl_CheckFloating(serverFlow, attackingMon, 1)) {
#if TERRAIN_GEN7_DMG
            return 6.144;
#endif
            return 5325;
        }
        break;
    case TERRAIN_MISTY:
        if (type == TYPE_DRAGON && !ServerControl_CheckFloating(serverFlow, defendingMon, 1)) {
            return 2048;
        }
        break;
    case TERRAIN_PSYCHIC:
        if (type == TYPE_PSY &&
            !ServerControl_CheckFloating(serverFlow, attackingMon, 1)) {
#if TERRAIN_GEN7_DMG
            return 6.144;
#endif
            return 5325;
        }
        break;
    }
    return 4096;
}

// Called when setting non-infinite terrain [BattleHandler_AddFieldEffect]
extern "C" u32 ServerEvent_IncreaseMoveTerrainTurns(ServerFlow * serverFlow, TERRAIN terrain, u32 attackingSlot) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
    BattleEventVar_SetValue(VAR_EFFECT_TURN_COUNT, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_TERRAIN_TURN_COUNT);
    u32 extraTurns = BattleEventVar_GetValue(VAR_EFFECT_TURN_COUNT);
    BattleEventVar_Pop();
    return extraTurns;
}
// Called before a mon changes the terrain [ServerControl_ChangeTerrain] & [BattleField_TurnCheckCore]
extern "C" b32 ServerEvent_ChangeTerrain(ServerFlow * serverFlow, TERRAIN terrain) {
    b32 failFlag = 0;

    for (u8 i = 0; i < 24; ++i) {

        BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (currentMon) {

            u32 currentSlot = BattleMon_GetID(currentMon);

            u32 currentPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
            if (currentPos != NULL_BATTLE_POS) {
                u32 HEID = HEManager_PushState(&serverFlow->HEManager);

                if (!BattleMon_IsFainted(currentMon)) {
                    BattleEventVar_Push();
                    SET_UP_NEW_EVENT;
                    u32 currentSlot = BattleMon_GetID(currentMon);
                    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
                    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
                    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
                    BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHANGE);
                    failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
                    if (failFlag) {
                        BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHANGE_FAIL);
                    }
                    BattleEventVar_Pop();
                }

                HEManager_PopState(&serverFlow->HEManager, HEID);
            }
        }
    }

    return !failFlag;
}
// Called after a mon changes the terrain [BattleHandler_AddFieldEffect]
extern "C" void ServerEvent_ChangeTerrainAfter(ServerFlow* serverFlow, TERRAIN terrain) {
    for (u8 i = 0; i < 24; ++i) {

        BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (currentMon) {
            u32 currentSlot = BattleMon_GetID(currentMon);

            u32 currentPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
            if (currentPos != NULL_BATTLE_POS) {
                int HEID = HEManager_PushState(&serverFlow->HEManager);
                if (!BattleMon_IsFainted(currentMon)) {
                    BattleEventVar_Push();
                    SET_UP_NEW_EVENT;
                    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
                    BattleEventVar_SetConstValue(VAR_WEATHER, terrain);
                    BattleEvent_CallHandlers(serverFlow, EVENT_AFTER_TERRAIN_CHANGE);
                    BattleEventVar_Pop();
                }
                HEManager_PopState(&serverFlow->HEManager, HEID);
            }
        }
    }
}
// Called when trying to get the terrain [HandlerSurgeSurfer]
extern "C" TERRAIN ServerEvent_GetTerrain(ServerFlow* serverFlow) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_TERRAIN_CHECK);
    int failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();
    if (failFlag)
        return 0;
    return BattleField_GetTerrain(g_BattleField);
}
// Called when a mon is grounded by the Gravity effect [BattleHandler_GravityCheck]
extern "C" void ServerEvent_GroundedByGravity(ServerFlow * serverFlow, BattleMon * battleMon) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 pokemonSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, pokemonSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_GROUNDED_BY_GRAVITY);
    BattleEventVar_Pop();
}

extern "C" b32 ServerControl_ChangeTerrainCheck(ServerFlow * serverFlow, TERRAIN terrain, u32 turns) {
    if (terrain > TERRAIN_PSYCHIC) {
        return 0;
    }

    return terrain != BattleField_GetTerrain(g_BattleField) ||
        turns == INFINITE_FIELD_TURNS && BattleField_GetTerrainTurns(g_BattleField) != INFINITE_FIELD_TURNS;
}
extern "C" u32 ServerControl_ChangeTerrain(ServerFlow * serverFlow, TERRAIN terrain, u32 turns) {
    if (!ServerControl_ChangeTerrainCheck(serverFlow, terrain, turns)) {
        return 0;
    }

    if (!ServerEvent_ChangeTerrain(serverFlow, terrain)) {
        return 0;
    }

    BattleField_SetTerrain(g_BattleField, terrain, turns);
    return 1;
}

struct FieldEffectEventAddTableExt
{
    FIELD_EFFECT fieldEffect;
    FieldEffectEventAddFunc func;
    const char* dllName;
};
FieldEffectEventAddTableExt fieldEffectEventAddTableExt[] = {
    { FLDEFF_TERRAIN, EventAddFieldTerrain, "FieldEffects/Terrain" },
#if EXPAND_ABILITIES
    { FLDEFF_DARK_AURA, EventAddFieldDarkAura, "FieldEffects/AuraField" },
    { FLDEFF_FAIRY_AURA, EventAddFieldFairyAura, "FieldEffects/AuraField" },
    { FLDEFF_VESSEL_OF_RUIN, EventAddFieldVesselOfRuin, "FieldEffects/RuinField" },
    { FLDEFF_SWORD_OF_RUIN, EventAddFieldSwordOfRuin, "FieldEffects/RuinField" },
    { FLDEFF_TABLETS_OF_RUIN, EventAddFieldTabletsOfRuin, "FieldEffects/RuinField" },
    { FLDEFF_BEADS_OF_RUIN, EventAddFieldBeadsOfRuin, "FieldEffects/RuinField" },
#endif // EXPAND_ABILITIES
};

extern "C" BattleEventItem * AddFieldEffectEvent(FIELD_EFFECT fieldEffect, u32 pokemonSlot, FieldEffectEventAddFunc func) {
    u32 handlerAmount = 0;
    BattleEventHandlerTableEntry* battleEventEntry = func(&handlerAmount);

    return BattleEvent_AddItem(EVENTITEM_FIELD, fieldEffect, EVENTPRI_FIELD_DEFAULT, 0, pokemonSlot, battleEventEntry, handlerAmount);
}

// Add the Field Effect battle event handlers to the event pool
extern "C" BattleEventItem* FieldEffectEvent_AddItem(FIELD_EFFECT fieldEffect, u32 pokemonSlot) {
    // Check for new or overriden field effects
    for (FIELD_EFFECT currentFieldEffect = 0; currentFieldEffect < ARRAY_COUNT(fieldEffectEventAddTableExt); ++currentFieldEffect) {
        
        FieldEffectEventAddTableExt* fieldEffectAddEvent = &(fieldEffectEventAddTableExt[currentFieldEffect]);
        if (fieldEffect == fieldEffectAddEvent->fieldEffect) {
            // Load the DLL with the code for the field if needed
            if (!LoadDll(fieldEffectAddEvent->dllName))
                return 0;
            
            return AddFieldEffectEvent(fieldEffect, pokemonSlot, fieldEffectAddEvent->func);
        }
    }
    // Check for vanilla field effects
    FieldEffectEventAddTable* fieldEffectEventAddTable = (FieldEffectEventAddTable*)0x0689D7D8;
    for (FIELD_EFFECT currentFieldEffect = 0; currentFieldEffect < 8; ++currentFieldEffect) {

        FieldEffectEventAddTable* fieldEffectAddEvent = &(fieldEffectEventAddTable[currentFieldEffect]);
        if (fieldEffect == fieldEffectAddEvent->fieldEffect) {
            return AddFieldEffectEvent(fieldEffect, pokemonSlot, fieldEffectAddEvent->func);
        }
    }
    return 0;
}

// Remove a specific dependent Pokémon from a specific field effect
extern "C" void BattleField_RemoveDependPokeEffectCoreFromEffect(BattleFieldExt * battleField, u32 pokemonSlot, FIELD_EFFECT fieldEffect) {
    if (BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {

        u32 pokeCount = battleField->dependPokeCount[fieldEffect];
        if (pokeCount) {
            u32* dependPokes = battleField->dependPokeID[fieldEffect];

            // Find the slot to remove
            u32 removeSlot = 0;
            while (pokemonSlot != dependPokes[removeSlot]) {
                if (++removeSlot >= pokeCount) {
                    return;
                }
            }

            // Move back slots after removed slot by one
            for (; removeSlot < 5; ++removeSlot) {
                // WARNING: "BattleField_CheckFieldEffectCore" is not needed here
                // WARNING: not adding a call to another function here seems to be crashing the game
                // WARNING: debugging it seems to be creating a loop to itself in ASM
                // WARNING: it's similar to when you use a function that is declared and not defined
                BattleField_CheckFieldEffectCore(battleField, fieldEffect);
                dependPokes[removeSlot] = dependPokes[removeSlot + 1];
            }

            // Set last slot to null & remove 1 from the total count
            dependPokes[removeSlot] = BATTLE_MAX_SLOTS;
            pokeCount = --battleField->dependPokeCount[fieldEffect];

            // Remove the field effect if the count is empty
            if (pokeCount == 0) {
                BattleField_RemoveEffectCore(battleField, fieldEffect);
            }
            else {
                u32 condPokemonSlot = Condition_GetMonID(battleField->conditionData[fieldEffect]);
                // If the condition Pokémon is the one removed, set it to the first available Pokémon
                if (pokemonSlot == condPokemonSlot) {
                    Condition_SetMonID(&battleField->conditionData[fieldEffect], dependPokes[0]);
                }
            }
        }
    }
}

extern "C" void CommonAddFieldEffectAbilityHandler(ServerFlow * serverFlow, u32 pokemonSlot, FIELD_EFFECT fieldEffect, u16 msgID) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = fieldEffect;
        addFieldEffect->condData = Condition_MakePoke(pokemonSlot);
        addFieldEffect->addDependPoke = 1;
        BattleHandler_StrSetup(&addFieldEffect->exStr, 2u, msgID);
        BattleHandler_AddArg(&addFieldEffect->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}
extern "C" void CommonRemoveFieldEffectAbilityHandler(ServerFlow * serverFlow, u32 pokemonSlot, FIELD_EFFECT fieldEffect) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleField_RemoveDependPokeEffectCoreFromEffect(g_BattleField, pokemonSlot, fieldEffect);
    }
}

extern "C" void CommonTerrainChangeAbility(ServerFlow* serverFlow, u32 pokemonSlot, TERRAIN terrain, u16 msgID) {
    if (BattleField_GetTerrain(g_BattleField) != terrain) {
        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_AddFieldEffect* addFieldEffect;
        addFieldEffect = (HandlerParam_AddFieldEffect*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_FIELD_EFFECT, pokemonSlot);
        addFieldEffect->effect = FLDEFF_TERRAIN;
        addFieldEffect->field_D = terrain;
        addFieldEffect->condData = Condition_MakeTurn(TERRAIN_ABILITY_TURNS);
        BattleHandler_StrSetup(&addFieldEffect->exStr, 1u, msgID);
        BattleHandler_AddArg(&addFieldEffect->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, addFieldEffect);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}

extern "C" void THUMB_BRANCH_BattleField_Clear(WEATHER weather) {
    BattleField_InitCore(g_BattleField, weather);
}

// Clears the data of off a field effect in the Extended BattleField struct
extern "C" void THUMB_BRANCH_BattleField_ClearFactorWork(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect) {
    battleField->battleEventItems[fieldEffect] = 0;
    battleField->conditionData[fieldEffect] = Condition_MakeNull();
    battleField->turnCount[fieldEffect] = 0;
    battleField->dependPokeCount[fieldEffect] = 0;
    battleField->effectEnableFlags[fieldEffect] = 0;

    for (u32 dependPoke = 0; dependPoke < 6; ++dependPoke) {
        battleField->dependPokeID[fieldEffect][dependPoke] = BATTLE_MAX_SLOTS;
    }
}

// Initializes the data in the Extended BattleField struct
extern "C" void THUMB_BRANCH_BattleField_InitCore(BattleFieldExt * battleField, WEATHER weather) {
    battleField->weather = weather;
    battleField->weatherTurns = INFINITE_FIELD_TURNS;
    battleField->terrain = 0;
    battleField->terrainTurns = INFINITE_FIELD_TURNS;

    for (FIELD_EFFECT currentEffect = FLDEFF_WEATHER; currentEffect < FIELD_EFFECT_AMOUNT; ++currentEffect) {
        BattleField_ClearFactorWork(battleField, currentEffect);
    }

#if EXPAND_ABILITIES
    battleField->tempItem = ITEM_NULL;

    battleField->auraBreakMons = 0;

    battleField->extremeWeatherMons = 0;

    battleField->battleBondFlag = 0;

    battleField->emergencyExitFlag = 0;

    sys_memset(battleField->firstTurnMons, BATTLE_MAX_SLOTS, 6);

#if GEN9_INTREPID_SWORD
    battleField->intrepidSwordFlag = 0;
#endif
#if GEN9_DAUNTLESS_SHIELD
    battleField->dauntlessShieldFlag = 0;
#endif

    battleField->neutralizingGasMons = 0;

    sys_memset(battleField->pairedDondozos, BATTLE_MAX_SLOTS, 4);

    battleField->paradoxAbilityFlag = 0;

    battleField->supersweetSyrupFlag = 0;
#endif

#if EXPAND_MOVES

    battleField->consumedBerryFlags = 0;

    sys_memset(battleField->extraTypes, TYPE_NULL, BATTLE_MAX_SLOTS);

#endif 
}

// Allocates the memory for the Extended BattleField struct
extern "C" BattleFieldExt * THUMB_BRANCH_BattleField_Init(u16 heapID) {
    if (!g_BattleField) {
        g_BattleField = (BattleFieldExt*)GFL_HeapAllocate(heapID, sizeof(BattleFieldExt), 1, "btl_field.c", 268u);
        BattleField_InitCore(g_BattleField, WEATHER_NULL);
    }
    
    return g_BattleField;
}

extern "C" void THUMB_BRANCH_BattleField_Free(BattleFieldExt * battleField) {
    if (g_BattleField) {
        GFL_HeapFree(g_BattleField);
        g_BattleField = nullptr;

        FreeLoadedDlls();
    }
}

extern "C" WEATHER THUMB_BRANCH_BattleField_GetWeatherCore(BattleFieldExt * battleField) {
    return battleField->weather;
}

extern "C" WEATHER THUMB_BRANCH_BattleField_GetWeather() {
    return BattleField_GetWeatherCore(g_BattleField);
}

extern "C" u32 THUMB_BRANCH_BattleField_GetWeatherTurnsCore(BattleFieldExt * battleField) {
    if (battleField->weather) {
        return battleField->weatherTurns;
    }
    return 0;
}

extern "C" u32 THUMB_BRANCH_BattleField_GetWeatherTurns() {
    return BattleField_GetWeatherTurnsCore(g_BattleField);
}

extern "C" void THUMB_BRANCH_BattleField_SetWeatherCore(BattleFieldExt * battleField, WEATHER weather, u32 turns) {
    battleField->weather = weather;
    battleField->weatherTurns = turns;
}

extern "C" void THUMB_BRANCH_BattleField_SetWeather(WEATHER weather, u32 turns) {
    BattleField_SetWeatherCore(g_BattleField, weather, turns);
}

extern "C" void THUMB_BRANCH_BattleField_EndWeather(BattleFieldExt * battleField) {
    battleField->weather = WEATHER_NULL;
    battleField->weatherTurns = 0;
}

// Reduces weather turn count each turn and sets weather to NULL when it ends, returns current weather
extern "C" WEATHER THUMB_BRANCH_BattleField_TurnCheckWeatherCore(BattleFieldExt * battleField) {
    WEATHER weather = battleField->weather;
    if (!weather) {
        return WEATHER_NULL;
    }

    if (battleField->weatherTurns == 255) {
        return WEATHER_NULL;
    }

    battleField->weatherTurns -= 1;
    if (battleField->weatherTurns) {
        return WEATHER_NULL;
    }

    battleField->weather = WEATHER_NULL;
    return weather;
}

extern "C" WEATHER THUMB_BRANCH_BattleField_TurnCheckWeather() {
    return BattleField_TurnCheckWeatherCore(g_BattleField);
}

// Checks if a Field Effect is currently active
extern "C" b32 THUMB_BRANCH_BattleField_CheckFieldEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect) {
    return battleField->effectEnableFlags[fieldEffect];
}

extern "C" b32 THUMB_BRANCH_BattleField_CheckEffect(FIELD_EFFECT fieldEffect) {
    return BattleField_CheckFieldEffectCore(g_BattleField, fieldEffect);
}

// Adds a non-active Field Effect
extern "C" b32 THUMB_BRANCH_BattleField_AddEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, ConditionData conditionData, b32 addEvents) {
    if (battleField->effectEnableFlags[fieldEffect]) {
        return 0;
    }

    if (addEvents) {
        BattleEventItem* effectEvent = FieldEffectEvent_AddItem(fieldEffect, 0);
        battleField->battleEventItems[fieldEffect] = effectEvent;

        if (!effectEvent) {
            return 0;
        }
    }

    battleField->effectEnableFlags[fieldEffect] = 1;
    battleField->conditionData[fieldEffect] = conditionData;
    battleField->turnCount[fieldEffect] = 0;
    battleField->dependPokeCount[fieldEffect] = 0;

    for (u32 dependPoke = 0; dependPoke < 6; ++dependPoke) {
        battleField->dependPokeID[fieldEffect][dependPoke] = BATTLE_MAX_SLOTS;
    }

    u32 pokemonSlot = Condition_GetMonID(conditionData);
    if (pokemonSlot != BATTLE_MAX_SLOTS) {
        BattleField_AddDependPokeCore(battleField, fieldEffect, pokemonSlot);
    }

    return 1;
}

extern "C" b32 THUMB_BRANCH_BattleField_AddEffect(FIELD_EFFECT fieldEffect, ConditionData conditionData) {
    return BattleField_AddEffectCore(g_BattleField, fieldEffect, conditionData, true);
}

// Removes an active Field Effect
extern "C" b32 THUMB_BRANCH_BattleField_RemoveEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect) {
    if (!BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {
        return 0;
    }

    BattleEventItem* battleEventItem = battleField->battleEventItems[fieldEffect];
    if (battleEventItem) {
        BattleEventItem_Remove(battleEventItem);
        battleField->battleEventItems[fieldEffect] = 0;
    }

    BattleField_ClearFactorWork(battleField, fieldEffect);
    return 1;
}

extern "C" b32 THUMB_BRANCH_BattleField_RemoveEffect(FIELD_EFFECT fieldEffect) {
    return BattleField_RemoveEffectCore(g_BattleField, fieldEffect);
}

// Makes a given Field Effect dependent on a Pokémon, and makes affected Pokémons dependent on it
extern "C" b32 THUMB_BRANCH_BattleField_AddDependPokeCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, u32 pokemonSlot) {
    if (BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {

        u32 pokeCount = battleField->dependPokeCount[fieldEffect];
        // Don't add if this effect already depends on this Pokémon
        for (u32 i = 0; i < pokeCount; ++i) {
            if (battleField->dependPokeID[fieldEffect][i] == pokemonSlot) {
                return 0;
            }
        }
        if (pokeCount < 6) {
            battleField->dependPokeID[fieldEffect][pokeCount] = pokemonSlot;
            ++battleField->dependPokeCount[fieldEffect];
            return 1;
        }
    }
    return 0;
}

extern "C" b32 THUMB_BRANCH_BattleField_AddDependPoke(FIELD_EFFECT fieldEffect, u32 pokemonSlot) {
    return BattleField_AddDependPokeCore(g_BattleField, fieldEffect, pokemonSlot);
}

// Removes Field Effect dependency
extern "C" void THUMB_BRANCH_BattleField_RemoveDependPokeEffectCore(BattleFieldExt * battleField, u32 pokemonSlot) {
    for (FIELD_EFFECT fieldEffect = FLDEFF_WEATHER; fieldEffect < FIELD_EFFECT_AMOUNT; ++fieldEffect) {
        BattleField_RemoveDependPokeEffectCoreFromEffect(battleField, pokemonSlot, fieldEffect);
    }
}

extern "C" void THUMB_BRANCH_BattleField_RemoveDependPokeEffect(u32 pokemonSlot) {
    BattleField_RemoveDependPokeEffectCore(g_BattleField, pokemonSlot);
}

// Checks if a move can be used under the effect of Imprison
extern "C" b32 THUMB_BRANCH_BattleField_CheckImprisonCore(BattleFieldExt * battleField, PokeCon * pokeCon, BattleMon * battleMon, MOVE_ID moveID) {
    u32 dependPokeCount = battleField->dependPokeCount[FLDEFF_IMPRISON];
    if (!dependPokeCount) {
        return 0;
    }

    u32 currentSlot = BattleMon_GetID(battleMon);
    for (u32 dependPoke = 0; dependPoke < dependPokeCount; ++dependPoke) {

        u32 dependSlot = battleField->dependPokeID[FLDEFF_IMPRISON][dependPoke];
        if (!MainModule_IsAllyMonID(currentSlot, dependSlot)) {

            BattleMon* dependMon = PokeCon_GetBattleMon(pokeCon, dependSlot);
            if (Move_IsUsable(dependMon, moveID)) {
                return 1;
            }
        }
    }
    return 0;
}

extern "C" b32 THUMB_BRANCH_BattleField_CheckImprison(PokeCon * pokeCon, BattleMon * battleMon, MOVE_ID moveID) {
    return BattleField_CheckImprisonCore(g_BattleField, pokeCon, battleMon, moveID);
}

// Reduces Field Effect turn count each turn and removes Field Effect when it ends
extern "C" void THUMB_BRANCH_BattleField_TurnCheckCore(BattleFieldExt * battleField, void(*callback)(FIELD_EFFECT, ServerFlow*), ServerFlow * serverFlow) {
    // Since the both BattleField structs have been merged, we ignore the client side calls
    if (!callback) {
        return;
    }
    
    for (FIELD_EFFECT fieldEffect = 1; fieldEffect < FIELD_EFFECT_AMOUNT; ++fieldEffect) {
        if (BattleField_CheckFieldEffectCore(battleField, fieldEffect)) {

            u32 turnMax = Condition_GetTurnMax(battleField->conditionData[fieldEffect]);
            if (turnMax) {
                u32 turnCount = battleField->turnCount[fieldEffect];
                ++turnCount;

                battleField->turnCount[fieldEffect] = turnCount;
                if (fieldEffect == FLDEFF_TERRAIN) {
                    // Ignore turns if terrain is permanent
                    if (battleField->terrainTurns == INFINITE_FIELD_TURNS)
                        continue;

                    // Update terrain turns
                    battleField->terrainTurns = turnCount;
                }

                if (turnCount >= turnMax) {
                    if (fieldEffect == FLDEFF_TERRAIN) {
                        // Remove terrain data
                        BattleField_EndTerrain(battleField);
                        ServerEvent_ChangeTerrain(serverFlow, TERRAIN_NULL);
                    }

                    BattleField_RemoveEffectCore(battleField, fieldEffect);
                    if (callback) {
                        callback(fieldEffect, serverFlow);
                    }
                }
            }
        }
    }
}

extern "C" void THUMB_BRANCH_BattleField_TurnCheck(void(*callback)(FIELD_EFFECT, ServerFlow*), ServerFlow * serverFlow) {
    BattleField_TurnCheckCore(g_BattleField, callback, serverFlow);
}

extern "C" b32 THUMB_BRANCH_BattleHandler_AddFieldEffect(ServerFlow * serverFlow, HandlerParam_AddFieldEffect* params) {
    u8 prevTerrain = TERRAIN_NULL;
    u8 terrain = params->field_D;
    u32 pokemonSlot = params->header.flags << 19 >> 27;
    BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, pokemonSlot);

    if (params->effect == FLDEFF_TERRAIN) {

        prevTerrain = BattleField_GetTerrain(g_BattleField);
        u8 turns = Condition_GetTurnMax(params->condData);

        if (turns != INFINITE_FIELD_TURNS) {
            u8 extraTurns = ServerEvent_IncreaseMoveTerrainTurns(serverFlow, terrain, pokemonSlot);
            if (extraTurns) {
                turns += extraTurns;
                params->condData = Condition_MakeTurn(turns);
            }
        }

        if (!ServerControl_ChangeTerrain(serverFlow, terrain, turns)) {
            return 0;
        }
    }

    if (!prevTerrain && !ServerControl_FieldEffectCore(serverFlow, params->effect, params->condData, params->addDependPoke))
    {
        return 0;
    }

    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupAdd(serverFlow, currentMon);
    }

    if (params->effect == FLDEFF_TERRAIN) {
        MOVE_ID moveID = 0;
        switch (terrain)
        {
        case TERRAIN_ELECTRIC:
            moveID = ELECTRIC_TERRAIN_MOVE_ANIM;
            break;
        case TERRAIN_GRASSY:
            moveID = GRASSY_TERRAIN_MOVE_ANIM;
            break;
        case TERRAIN_MISTY:
            moveID = MISTY_TERRAIN_MOVE_ANIM;
            break;
        case TERRAIN_PSYCHIC:
            moveID = PSYCHIC_TERRAIN_MOVE_ANIM;
            break;
        }
        u32 pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, pokemonSlot);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_MoveAnim, pokePos, pokePos, moveID, 0, 0);

        // ChangeBattleBackground(10); // TODO: CHANGE BACKGROUND WHEN THE TERRAIN STARTS AND ENDS
        // OVL_168:021E0244 in the IDB
    }

    BattleHandler_SetString(serverFlow, &params->exStr);

    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupRemove(serverFlow, currentMon);
    }

    if (params->effect == FLDEFF_TERRAIN) {
        ServerEvent_ChangeTerrainAfter(serverFlow, terrain);
    }

    return 1;
}

extern "C" void THUMB_BRANCH_ServerControl_FieldEffectEnd(ServerFlow * serverFlow, FIELD_EFFECT fieldEffect) {
    u16 msgID = 0xFFFF;
    switch (fieldEffect) {
    case FLDEFF_TRICK_ROOM:
        msgID = 116;
        break;
    case FLDEFF_GRAVITY:
        msgID = 118;
        break;
    case FLDEFF_WONDER_ROOM:
        msgID = 179;
        break;
    case FLDEFF_MAGIC_ROOM:
        msgID = 181;
        break;
    case FLDEFF_TERRAIN:
        msgID = BATTLE_TERRAIN_END_MSGID;
        break;
    default:
        break;
    }
    if (msgID != 0xFFFF) {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, msgID, 0xFFFF0000);
    }

    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_RemoveFieldEffect, fieldEffect);
    if (fieldEffect == FLDEFF_MAGIC_ROOM) {
        ServerControl_SortBySpeed(serverFlow, serverFlow->pokeSetTemp);
        PokeSet_SeekStart(serverFlow->pokeSetTemp);
        for (BattleMon* currentMon = PokeSet_SeekNext(serverFlow->pokeSetTemp); currentMon; currentMon = PokeSet_SeekNext(serverFlow->pokeSetTemp)) {
            if (BattleMon_CanBattle(currentMon)) {
                ServerControl_CheckItemReaction(serverFlow, currentMon, 0);
            }
        }
    }
}

extern "C" u32 THUMB_BRANCH_BattleHandler_GravityCheck(ServerFlow * serverFlow, HandlerParam_Header * params) {
    u8 battleSlots[8];
    u32 pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, params->flags << 19 >> 27);

    u32 pokeCount = Handler_ExpandPokeID(serverFlow, pokePos | 0x800, battleSlots);
    for (u32 i = 0; i < pokeCount; i = ++i) {

        BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, battleSlots[i]);
        b32 grounded = false;
        if (BattleMon_GetConditionFlag(battleMon, CONDITIONFLAG_FLY)) {
            ServerControl_HideTurnCancel(serverFlow, battleMon, CONDITIONFLAG_FLY);
            grounded = true;
        }
        if (ServerEvent_CheckFloating(serverFlow, battleMon, 1)) {
            grounded = true;
        }
        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_FLOATING)) {
            ServerControl_CureCondition(serverFlow, battleMon, CONDITION_FLOATING, 0);
            grounded = true;
        }
        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_TELEKINESIS)) {
            ServerControl_CureCondition(serverFlow, battleMon, CONDITION_TELEKINESIS, 0);
            grounded = true;
        }

        if (grounded) {
            ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, 1083, battleSlots[i], -65536);
            ServerEvent_GroundedByGravity(serverFlow, battleMon);
        }
    }
    return 1;
}

extern "C" void THUMB_BRANCH_HandlerRestCheckFail(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && !BattleEventVar_GetValue(VAR_FAIL_CAUSE)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SLEEP)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER);
        }
        else if (BattleMon_CheckIfMoveCondition(currentMon, CONDITION_HEALBLOCK)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_HEALBLOCK);
        }
        else if (BattleMon_IsFullHP(currentMon)) {
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_HPFULL);
        }
        else if ((BattleField_GetTerrain(g_BattleField) == TERRAIN_ELECTRIC || BattleField_GetTerrain(g_BattleField) == TERRAIN_MISTY) &&
            !ServerControl_CheckFloating(serverFlow, currentMon, 1)) {
            // Electric Terrain & Misty Terrain fail
            BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_OTHER);
        }
        else {
            ABILITY currentAbility = BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY);
            if (currentAbility == ABIL_INSOMNIA ||
                currentAbility == ABIL_VITAL_SPIRIT ||
                currentAbility == ABIL_SWEET_VEIL) {
                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_INSOMNIA);
            }

#if EXPAND_ABILITIES
            // Flower Veil& Sweet Veil fail
            for (u8 i = 0; i < 24; ++i) {
                BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
                if (currentMon) {

                    u32 currentSlot = BattleMon_GetID(currentMon);
                    u32 currentPos = Handler_PokeIDToPokePos(serverFlow, currentSlot);
                    if (currentPos != NULL_BATTLE_POS) {
                        if (!BattleMon_IsFainted(currentMon) && MainModule_IsAllyMonID(pokemonSlot, currentSlot)) {
                            if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_SWEET_VEIL) {
                                BattleEventVar_RewriteValue(VAR_FAIL_CAUSE, MOVEFAIL_INSOMNIA);
                                break;
                            }
                        }
                    }
                }
            }
#endif
        }
    }
}

extern "C" u32 ServerFlow_GetBattlePedestal(ServerFlow * serverFlow) {
    return serverFlow->mainModule->btlSetup->fieldStatus.battlePedestalID;
}

extern "C" void THUMB_BRANCH_HandlerNaturePower(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        MOVE_ID newMoveID = 0;

        TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
        switch (terrain) {
        case TERRAIN_ELECTRIC:
            newMoveID = MOVE_THUNDERBOLT;
            break;
        case TERRAIN_GRASSY:
            newMoveID = MOVE_ENERGY_BALL;
            break;
        case TERRAIN_MISTY:
            newMoveID = MOVE_PSYCHIC; // TODO: SWITCH TO MOONBLAST
            break;
        case TERRAIN_PSYCHIC:
            newMoveID = MOVE_PSYCHIC;
            break;
        default:
        {
            u32 battleGround = ServerFlow_GetBattlePedestal(serverFlow);
            switch (battleGround) {
            case 0u:
            case 5u:
                newMoveID = MOVE_SEED_BOMB;
                break;
            case 1u:
            case 2u:
            case 3u:
            case 8u:
            case 15u:
            case 17u:
                newMoveID = MOVE_EARTHQUAKE;
                break;
            case 4u:
            case 14u:
            case 16u:
            case 18u:
                newMoveID = MOVE_TRI_ATTACK;
            case 6u:
            case 11u:
            case 12u:
                newMoveID = MOVE_HYDRO_PUMP;
                break;
            case 7u:
                newMoveID = MOVE_BLIZZARD;
                break;
            case 9u:
                newMoveID = MOVE_MUD_BOMB;
                break;
            case 10u:
                newMoveID = MOVE_ROCK_SLIDE;
                break;
            case 13u:
                newMoveID = MOVE_ICE_BEAM;
                break;
            default:
                newMoveID = MOVE_TRI_ATTACK;
                break;
            }
        }
        break;
        }

        u32 targetPos = Handler_ReqMoveTargetAuto(serverFlow, pokemonSlot, newMoveID);
        BattleEventVar_RewriteValue(VAR_MOVE_ID, newMoveID);
        BattleEventVar_RewriteValue(VAR_POKE_POS, targetPos);
    }
}

extern "C" u32 THUMB_BRANCH_CommonSecretPowerGetParams(ServerFlow * serverFlow, u8 * effectPtr, u8 * changeAmountPtr)
{
    u8 effect;
    u8 changeAmount;
    u32 result;

    MOVE_ID newMoveID = 0;

    TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
    switch (terrain)
    {
    case TERRAIN_ELECTRIC:
        effect = SCTPOWEFF_ADD_COND;
        changeAmount = CONDITION_PARALYSIS;
        result = 0;
        break;
    case TERRAIN_GRASSY:
        effect = SCTPOWEFF_ADD_COND;
        changeAmount = CONDITION_SLEEP;
        result = 7;
        break;
    case TERRAIN_MISTY:
        effect = SCTPOWEFF_LOWER_STAT;
        changeAmount = STATSTAGE_SPECIAL_ATTACK;
        result = 5;
        break;
    case TERRAIN_PSYCHIC:
        effect = SCTPOWEFF_LOWER_STAT;
        changeAmount = STATSTAGE_SPEED;
        result = 2;
        break;
    default:
    {
        u32 battleGround = ServerFlow_GetBattlePedestal(serverFlow);
        switch (battleGround) {
        case 0:
        case 5:
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_SLEEP;
            result = 7;
            break;
        case 1:
        case 2:
        case 3:
        case 8:
        case 15:
        case 17:
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_ACCURACY;
            result = 1;
            break;
        case 6:
        case 11:
        case 12:
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_ATTACK;
            result = 5;
            break;
        case 7:
        case 13:
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_FREEZE;
            if (battleGround == 7) {
                result = 3;
            }
            else {
                result = 4;
            }
            break;
        case 9:
            effect = SCTPOWEFF_LOWER_STAT;
            changeAmount = STATSTAGE_SPEED;
            result = 2;
            break;
        case 10:
        case 19:
            effect = SCTPOWEFF_FLINCH;
            changeAmount = 0;
            result = 6;
            break;
        default:
            effect = SCTPOWEFF_ADD_COND;
            changeAmount = CONDITION_PARALYSIS;
            result = 0;
            break;
        }
    }
    break;
    }

    if (effectPtr) {
        *effectPtr = effect;
    }
    if (changeAmountPtr) {
        *changeAmountPtr = changeAmount;
    }
    return result;
}

extern "C" void THUMB_BRANCH_HandlerCamouflage(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        POKE_TYPE pokeType = TYPE_NORMAL;

        TERRAIN terrain = BattleField_GetTerrain(g_BattleField);
        switch (terrain) {
        case TERRAIN_ELECTRIC:
            pokeType = TYPE_ELEC;
            break;
        case TERRAIN_GRASSY:
            pokeType = TYPE_GRASS;
            break;
        case TERRAIN_MISTY:
            pokeType = TYPE_NORMAL; // TODO: SWITCH TO FAIRY
            break;
        case TERRAIN_PSYCHIC:
            pokeType = TYPE_PSY;
            break;
        default:
        {
            u32 battleGround = ServerFlow_GetBattlePedestal(serverFlow);
            switch (battleGround) {
            case 0:
            case 5:
                pokeType = TYPE_GRASS;
                break;
            case 1:
            case 2:
            case 3:
            case 8:
            case 9:
            case 15:
            case 17:
                pokeType = TYPE_GROUND;
                break;
            case 6:
            case 11:
            case 12:
                pokeType = TYPE_WATER;
                break;
            case 7:
            case 13:
                pokeType = TYPE_ICE;
                break;
            case 10:
            case 19:
                pokeType = TYPE_ROCK;
                break;
            default:
                pokeType = TYPE_NORMAL;
                break;
            }
        }
        break;
        }

        u16 monotype = PokeTypePair_MakeMonotype(pokeType);
        BattleMon* battleMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (monotype != BattleMon_GetPokeType(battleMon)) {
            HandlerParam_ChangeType* changeType;
            changeType = (HandlerParam_ChangeType*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_TYPE, pokemonSlot);
            changeType->pokeType = monotype;
            changeType->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, changeType);
        }
    }
}

#if EXPAND_ABILITIES

extern "C" b32 THUMB_BRANCH_BattleHandler_ChangeWeather(ServerFlow * serverFlow, HandlerParam_ChangeWeather * params) {
    BattleMon* currentMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->header.flags << 19 >> 27);

    if (!params->airLock) {
        if (CanWeatherBeChanged(serverFlow, params->weather, params->turn, params->field_7))
        {
            if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
                ServerDisplay_AbilityPopupAdd(serverFlow, currentMon);
            }

            // Changing the weather to NULL means ending the weather
            if (params->weather == WEATHER_NULL) {
                WEATHER currentWeather = BattleField_GetWeather();
                if (currentWeather != WEATHER_NULL) {
                    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_WeatherEnd, currentWeather);
                    ServerControl_ChangeWeatherAfter(serverFlow, WEATHER_NULL);
                }
            }
            else {
                ChangeWeather(serverFlow, params->weather, params->turn, params->field_7);
                BattleHandler_SetString(serverFlow, &params->exStr);
            }

            if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
                ServerDisplay_AbilityPopupRemove(serverFlow, currentMon);
            }

            return 1;
        }
    }
    else {
        if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
            ServerDisplay_AbilityPopupAdd(serverFlow, currentMon);
        }

        BattleHandler_SetString(serverFlow, &params->exStr);
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);
        ServerEvent_NotifyAirLock(serverFlow);
        HEManager_PopState(&serverFlow->HEManager, HEID);

        if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
            ServerDisplay_AbilityPopupRemove(serverFlow, currentMon);
        }

        return 1;
    }

    return 0;
}
// This changes asume the only way to summon Extreme Weather is through BattleHandlers (abilities, items or moves)
// in order to summon Extreme Weather other ways, like at the start of a battle, look at [ServerControl_ChangeWeather]

struct WeatherGraphics {
    u16 animID;
    u16 extremeAnimID;
    u16 msgID;
    u16 extremeMsgID;
    u16 endMsgID;
    u16 endExtremeMsgID;
};
WeatherGraphics WeatherGraphicsData[WEATHER_AMOUNT]{
    {0, 0, 0, 0, 0, 0}, // NULL
    {619, DESOLATE_LAND_ANIM, 84,  BATTLE_DESOLATE_LAND_MSGID, 89, BATTLE_DESOLATE_LAND_END_MSGID}, // SUN
    {616, PRIMORDIAL_SEA_ANIM, 85,  BATTLE_PRIMORDIAL_SEA_MSGID, 90, BATTLE_PRIMORDIAL_SEA_END_MSGID}, // RAIN
    {617, 0, 87,  0, 92, 0}, // HAIL
    {618, 0, 86,  0, 91, 0}, // SANDSTORM
    {0, DELTA_STREAM_ANIM, 0, BATTLE_DELTA_STREAM_MSGID, 0, BATTLE_DELTA_STREAM_END_MSGID}, // STRONGWINDS
};
extern "C" b32 THUMB_BRANCH_ServerCommand_WeatherStart(BtlClientWk * clientWork, u32 * state, u32 * data) {
    WEATHER weather = data[0] & 0x7F;
    u8 isExtremeWeather = (data[0] & 0x80) != 0;

    if (!*state) {
        u32 turns = data[1];
        BattleField_SetWeatherCore((BattleFieldExt*)clientWork->battleField, weather, turns);
        if (IsChapterSkipMode(clientWork)) {
            return 1;
        }

        if (weather < WEATHER_AMOUNT) {
            if (!isExtremeWeather) {
                BattleViewCmd_AddAnim(WeatherGraphicsData[weather].animID);
            }
            else {
                BattleViewCmd_AddAnim(WeatherGraphicsData[weather].extremeAnimID);
            }
        }

        ++(*state);
        return 0;
    }
    if (*state == 1) {
        if (BattleViewCmd_WaitEffect()) {
            return 0;
        }

        if (!isExtremeWeather) {
            BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].msgID, 0);
        }
        else {
            BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].extremeMsgID, 0);
        }

        ++(*state);
        return 0;
    }

    return *state == 2 && BattleViewCmd_WaitMessage(clientWork->btlvCore);
}
extern "C" b32 THUMB_BRANCH_ServerCommand_WeatherEnd(BtlClientWk * clientWork, u32 * state, u32 * data) {
    WEATHER weather = data[0] & 0x7F;
    u8 isExtremeWeather = (data[0] & 0x80) != 0;

    if (*state) {
        if (*state == 1 && BattleViewCmd_WaitMessage(clientWork->btlvCore)) {
            BattleField_EndWeather((BattleFieldExt*)clientWork->battleField);
            return 1;
        }
    }
    else {
        if (weather < WEATHER_AMOUNT) {
            if (!isExtremeWeather) {
                BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].endMsgID, 0);
            }
            else {
                BattleViewCmd_StartMessageStandard(clientWork->btlvCore, WeatherGraphicsData[weather].endExtremeMsgID, 0);
            }
        }
        ++ * state;
    }
    return 0;
}

#endif // EXPAND_ABILITIES

// SIDE EFFECT EXPANSION
struct BattleSideEffectExt {
    BattleEventItem* item;
    ConditionData condData;
    u32 turnCounter;
    u32 count;
};
struct BattleSideExt {
    BattleSideEffectExt effects[SIDE_EFFECT_AMOUNT];
};
struct SideManagerExt {
    BattleSideExt sides[2];
};
SideManagerExt g_SideManager;

extern "C" void BattleSideEffect_Clear(BattleSideEffectExt* sideEffect) {
    sideEffect->item = nullptr;
    sideEffect->condData = Condition_MakeNull();
    sideEffect->turnCounter = 0;
    sideEffect->count = 0;
}

struct SideEffectEventAddTableExt
{
    SIDE_EFFECT sideEffect;
    SideEffectEventAddFunc func;
    u32 maxCount;
    const char* dllName;
};
SideEffectEventAddTableExt sideEffectEventAddTableExt[] = {
#if EXPAND_ITEMS
    { SIDEEFF_SPIKES, EventAddSideSpikesUpdated, 3, "SideEffects/Spikes"},
    { SIDEEFF_TOXIC_SPIKES, EventAddSideToxicSpikesUpdated, 2, "SideEffects/ToxicSpikes" },
    { SIDEEFF_STEALTH_ROCK, EventAddSideStealthRockUpdated, 1, "SideEffects/StealthRock" },
#endif
#if EXPAND_ABILITIES
    { SIDEEFF_WIDE_GUARD, EventAddSideWideGuardUpdated, 1, "SideEffects/WideGuard" },
    { SIDEEFF_QUICK_GUARD, EventAddSideQuickGuardUpdated, 1, "SideEffects/QuickGuard" },
#endif
#if EXPAND_MOVES
    { SIDEEFF_STICKY_WEB, EventAddSideStickyWeb, 1, "SideEffects/StickyWebSide" },
#endif
};

extern "C" BattleEventItem * AddSideEffectEvent(BattleSideEffectExt* sideEffect, u32 side, SIDE_EFFECT effect, ConditionData condData, SideEffectEventAddFunc func) {
    u32 handlerAmout = 0;
    BattleEventHandlerTableEntry* handlers = func(&handlerAmout);

    BattleEventItem* item = BattleEvent_AddItem(EVENTITEM_SIDE, effect, EVENTPRI_SIDE_DEFAULT, 0, side, handlers, handlerAmout);
    BattleEventItem_SetWorkValue(item, 6, *((u32*)(&condData)));
    sideEffect->item = item;
    sideEffect->condData = condData;
    sideEffect->turnCounter = 0;
    sideEffect->count = 1;

    return item;
}

extern "C" BattleEventItem* THUMB_BRANCH_SAFESTACK_SideEffectEvent_AddItem_Core(u32 side, SIDE_EFFECT effect, ConditionData condData) {
    BattleSideEffectExt* sideEffect = &(g_SideManager.sides[side].effects[effect]);

    // Check for new or overriden side effects
    for (u32 effectIdx = 0; effectIdx < ARRAY_COUNT(sideEffectEventAddTableExt); ++effectIdx) {

        SideEffectEventAddTableExt* sideEffectEvent = &(sideEffectEventAddTableExt[effectIdx]);
        if (effect == sideEffectEvent->sideEffect) {
            if (!sideEffect->count) {
                // Load the DLL with the code for the field if needed
                if (!LoadDll(sideEffectEvent->dllName))
                    return 0;

                return AddSideEffectEvent(sideEffect, side, effect, condData, sideEffectEvent->func);
            }
            else if (sideEffect->count < sideEffectEvent->maxCount) {
                ++sideEffect->count;
                return sideEffect->item;
            }
        }
    }
    // Check for vanilla side effects
    SideEffectEventAddTable* sideEffectEventAddTable = (SideEffectEventAddTable*)0x0689D908;
    for (u32 effectIdx = 0; effectIdx < 14; ++effectIdx) {

        SideEffectEventAddTable* sideEffectEvent = &(sideEffectEventAddTable[effectIdx]);
        if (effect == sideEffectEvent->sideEffect) {
            if (!sideEffect->count) {
                return AddSideEffectEvent(sideEffect, side, effect, condData, sideEffectEvent->func);
            }
            else if (sideEffect->count < sideEffectEvent->maxCount) {
                ++sideEffect->count;
                return sideEffect->item;
            }
        }
    }
    return 0;
}

// WARNING BattleHandler_RemoveSideEffectCore is used in BattleUpgrade.S

extern "C" void THUMB_BRANCH_SAFESTACK_BattleSideStatus_Init() {
    for (u32 side = 0; side < 2; ++side) {
        for (SIDE_EFFECT effect = 0; effect < SIDE_EFFECT_AMOUNT; ++effect) {
            BattleSideEffectExt* sideEffect = &(g_SideManager.sides[side].effects[effect]);
            BattleSideEffect_Clear(sideEffect);
        }
    }
}

extern "C" b32 THUMB_BRANCH_SAFESTACK_BattleSideStatus_IsEffectActive(u32 side, SIDE_EFFECT effect) {
    return g_SideManager.sides[side].effects[effect].item != nullptr;
}

extern "C" u32 THUMB_BRANCH_SAFESTACK_BattleSideStatus_GetCount(u32 side, SIDE_EFFECT effect) {
    return g_SideManager.sides[side].effects[effect].count;
}

extern "C" u32 THUMB_BRANCH_SAFESTACK_BattleSideStatus_GetCountFromBattleEventItem(BattleEventItem * item, u32 side) {
    SIDE_EFFECT effect = item->subID;
    if (effect < SIDE_EFFECT_AMOUNT && side < 2) {
        return g_SideManager.sides[side].effects[effect].count;
    }
    return 0;
}

extern "C" b32 THUMB_BRANCH_SAFESTACK_SideEffectEvent_RemoveItem(u32 side, SIDE_EFFECT effect) {
    BattleSideEffectExt* sideEffect = &(g_SideManager.sides[side].effects[effect]);
    if (!sideEffect->item) {
        return 0;
    }

    BattleEventItem_Remove(sideEffect->item);
    BattleSideEffect_Clear(sideEffect);
    return 1;
}

extern "C" void THUMB_BRANCH_SAFESTACK_BattleSideStatus_TurnCheck(void(* endMessageCallback)(u32 side, SIDE_EFFECT sideEffect, ServerFlow* serverFlow), ServerFlow* serverFlow) {
    for (u32 side = 0; side < 2; ++side) {
        for (SIDE_EFFECT effect = 0; effect < SIDE_EFFECT_AMOUNT; ++effect) {

            BattleSideEffectExt* sideEffect = &(g_SideManager.sides[side].effects[effect]);
            if (sideEffect->count && sideEffect->condData.status == 2) {

                ++sideEffect->turnCounter;
                if (sideEffect->turnCounter >= sideEffect->condData.turncount)
                {
                    BattleEventItem_Remove(sideEffect->item);
                    BattleSideEffect_Clear(sideEffect);

                    endMessageCallback(side, effect, serverFlow);
                }
            }
        }
    }
}

extern "C" void THUMB_BRANCH_ServerControl_SideEffectEndMessageCore(ServerFlow* serverFlow, SIDE_EFFECT sideEffect, u32 side) {
    u16 msgID = 0xFFFF;
    switch (sideEffect)
    {
    case SIDEEFF_REFLECT:
        msgID = 126;
        break;
    case SIDEEFF_LIGHT_SCREEN:
        msgID = 130;
        break;
    case SIDEEFF_SAFEGUARD:
        msgID = 134;
        break;
    case SIDEEFF_MIST:
        msgID = 138;
        break;
    case SIDEEFF_TAILWIND:
        msgID = 142;
        break;
    case SIDEEFF_LUCKY_CHANT:
        msgID = 146;
        break;
    case SIDEEFF_SPIKES:
        msgID = 150;
        break;
    case SIDEEFF_TOXIC_SPIKES:
        msgID = 154;
        break;
    case SIDEEFF_STEALTH_ROCK:
        msgID = 158;
        break;
    case SIDEEFF_RAINBOW:
        msgID = 166;
        break;
    case SIDEEFF_SEA_OF_FIRE:
        msgID = 170;
        break;
    case SIDEEFF_SWAMP:
        msgID = 174;
        break;
    case SIDEEFF_STICKY_WEB:
        msgID = BATTLE_STICKY_WEB_REMOVE_MSGID + side;
        side = 0xFFFF0000;
        break;
    default:
        break;
    }
    
    if (msgID == 0xFFFF) {
        return;  
    }

    ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, msgID, side);
}

#endif // EXPAND_FIELD_EFFECTS

#if GEN6_CRIT || EXPAND_FIELD_EFFECTS

extern "C" u32 CalcBaseDamage(u32 power, u32 attack, u32 level, u32 defense);
extern "C" u32 WeatherPowerMod(u32 weather, u32 subProcID);
extern "C" u32 TypeEffectivenessPowerMod(u32 damage, EFFECTIVENESS typeEffectiveness);

extern "C" ITEM BattleMon_GetUsableItem(BattleMon * battleMon) {
    if (BattleField_CheckEffect(FLDEFF_MAGIC_ROOM) ||
        BattleMon_GetValue(battleMon, VALUE_EFFECTIVE_ABILITY) == ABIL_KLUTZ ||
        BattleMon_CheckIfMoveCondition(battleMon, CONDITION_EMBARGO) ||
        BattleMon_GetTurnFlag(battleMon, TURNFLAG_CANTUSEITEM))
        return ITEM_NULL;

    return BattleMon_GetHeldItem(battleMon);
}

#if EXPAND_MOVES
extern "C" u32 ExtraTypeEffectivenessPowerMod(u32 damage, EFFECTIVENESS effectiveness) {
    switch (effectiveness)
    {
    case EFFECTIVENESS_IMMUNE: return 0;
    case EFFECTIVENESS_1_8: return damage >> 3;
    case EFFECTIVENESS_1_4: return damage >> 2;
    case EFFECTIVENESS_1_2: return damage >> 1;
    case EFFECTIVENESS_2: return damage * 2;
    case EFFECTIVENESS_4: return damage * 4;
    case EFFECTIVENESS_8: return damage * 8;
    }
    return damage;
}
#endif
extern "C" u32 THUMB_BRANCH_SAFESTACK_ServerEvent_CalcDamage(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon,
    MoveParam * moveParam, EFFECTIVENESS typeEffectiveness, u32 targetDmgRatio, u32 critFlag, u32 battleDebugMode, u16 * destDamage) {
    u32 category = PML_MoveGetCategory(moveParam->moveID);
    u32 isFixedDamage = 0;
    BattleEventVar_Push();
    BattleEventVar_SetConstValue(VAR_TYPE_EFFECTIVENESS, typeEffectiveness);
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_CRITICAL_FLAG, critFlag);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveParam->moveType);
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveParam->moveID);
    BattleEventVar_SetConstValue(VAR_MOVE_CATEGORY, category);
    BattleEventVar_SetValue(VAR_FIXED_DAMAGE, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_1);

    u32 finalDamage = BattleEventVar_GetValue(VAR_FIXED_DAMAGE);
    if (finalDamage) {
        isFixedDamage = 1;
    }
    else {
        u32 power = ServerEvent_GetMovePower(serverFlow, attackingMon, defendingMon, moveParam);
        u32 attack = ServerEvent_GetAttackPower(serverFlow, attackingMon, defendingMon, moveParam, critFlag);
        u32 defense = ServerEvent_GetTargetDefenses(serverFlow, attackingMon, defendingMon, moveParam, critFlag);
        u8 level = BattleMon_GetValue(attackingMon, VALUE_LEVEL);
        u32 baseDamage = CalcBaseDamage(power, attack, level, defense);
        u32 damage = baseDamage;
        if (targetDmgRatio != 4096) {
            damage = fixed_round(baseDamage, targetDmgRatio);
        }

        u32 weather = ServerEvent_GetWeather(serverFlow);
        u32 weatherDmgRatio = WeatherPowerMod(weather, moveParam->moveType);
        if (weatherDmgRatio != 4096
#if EXPAND_ITEMS
            // Utility Umbrella - Holder doesn't get hit by boosted weather damage
            && BattleMon_GetUsableItem(defendingMon) != ITEM_UTILITY_UMBRELLA
#endif
            ) {
            damage = fixed_round(damage, weatherDmgRatio);
        }
#if EXPAND_FIELD_EFFECTS
        u32 terrain = ServerEvent_GetTerrain(serverFlow);
        u32 terrainDmgRatio = TerrainPowerMod(serverFlow, attackingMon, defendingMon, terrain, moveParam->moveType);
        if (terrainDmgRatio != 4096) {
            damage = fixed_round(damage, terrainDmgRatio);
        }
#endif 
        if (critFlag) {
#if GEN6_CRIT
            damage += damage / 2;
#else
            damage += damage;
#endif
        }

        if (!MainModule_GetDebugFlag() && ServerFlow_IsCompetitorScenarioMode(serverFlow)) {
            u32 damageRoll = 85;
            if (!battleDebugMode) {
                damageRoll = (100 - BattleRandom(16u));
            }
            damage = damageRoll * damage / 100;
        }

        PokeType moveType = (PokeType)moveParam->moveType;
        if (moveType < TYPE_NULL) {
            u32 stab = ServerEvent_SameTypeAttackBonus(serverFlow, attackingMon, moveType);
            damage = fixed_round(damage, stab);
        }

#if EXPAND_MOVES
        damage = ExtraTypeEffectivenessPowerMod(damage, typeEffectiveness);
#else
        damage = TypeEffectivenessPowerMod(damage, typeEffectiveness);
#endif

        if (category == CATEGORY_PHYSICAL
            && BattleMon_GetStatus(attackingMon) == CONDITION_BURN
            && BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_GUTS) {
            damage = 50 * damage / 100u;
        }
        if (!damage) {
            damage = 1;
        }

        BattleEventVar_SetMulValue(VAR_RATIO, 4096, 41, 0x20000);
        BattleEventVar_SetValue(VAR_DAMAGE, damage);
        BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_2);
        int damageRatio = BattleEventVar_GetValue(VAR_RATIO);
        int damageAfterProc2 = BattleEventVar_GetValue(VAR_DAMAGE);
        finalDamage = fixed_round(damageAfterProc2, damageRatio);
    }
    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_DAMAGE_PROCESSING_END);
    BattleEventVar_Pop();
    *destDamage = (u16)finalDamage;

    return isFixedDamage;
}

#endif // GEN6_CRIT || EXPAND_FIELD_EFFECTS

#if EXPAND_ITEMS || GRASS_IMMUNE_TO_POWDER || GEN6_OVERCOAT

extern "C" void CommonContactStatusAbility(ServerFlow* serverFlow, u32 currentSlot, CONDITION condition, ConditionData condData, u8 effectChance);
extern "C" void THUMB_BRANCH_LINK_HandlerEffectSpore_0x44(ServerFlow* serverFlow, u32 defendingSlot, CONDITION condition, ConditionData condData, u8 effectChance) {
    BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
    if (defendingMon) {
#if EXPAND_ITEMS
        if (BattleMon_GetUsableItem(defendingMon) == ITEM_SAFETY_GOGGLES)
            return;
#endif
#if GRASS_IMMUNE_TO_POWDER
        if (BattleMon_HasType(defendingMon, TYPE_GRASS))
            return;
#endif
#if GEN6_OVERCOAT
        if (BattleMon_GetValue(defendingMon, VALUE_EFFECTIVE_ABILITY) == ABIL_OVERCOAT)
            return;
#endif
        CommonContactStatusAbility(serverFlow, defendingSlot, condition, condData, 30u);
    }
}

#endif // GRASS_IMMUNE_TO_POWDER

#if GHOST_IGNORES_TRAPS

extern "C" b32 THUMB_BRANCH_LINK_ServerControl_RunCore_0x5C(ServerFlow * serverFlow, BattleMon * battleMon) {
    if (BattleMon_HasType(battleMon, TYPE_GHOST)) {
        return false;
    }
    return ServerEvent_CheckRunPrevent(serverFlow, battleMon);
}

extern "C" u32 IsMonTrapped(BtlClientWk * client, BattleMon * battleMon, u32 * trappingSlot, u16 * trappingAbility);
extern "C" u32 THUMB_BRANCH_LINK_CanMonSwitch_0x2A(BtlClientWk * client, BattleMon * battleMon, u32 * trappingSlot, u16 * trappingAbility) {
    if (BattleMon_HasType(battleMon, TYPE_GHOST))
        return 4;
    return IsMonTrapped(client, battleMon, trappingSlot, trappingAbility);
}
extern "C" u32 THUMB_BRANCH_LINK_IsForbidEscape_0x86(BtlClientWk * client, BattleMon * battleMon, u32 * trappingSlot, u16 * trappingAbility) {
    if (BattleMon_HasType(battleMon, TYPE_GHOST))
        return 4;
    return IsMonTrapped(client, battleMon, trappingSlot, trappingAbility);
}
#endif // GHOST_IGNORES_TRAPS

#if EXPAND_ABILITIES || EXPAND_ITEMS
#include "include/Contact.h"

extern "C" b32 IsContact(ServerFlow * serverFlow, MOVE_ID moveID, u32 attackingSlot) {
    if (getMoveFlag(moveID, FLAG_CONTACT)) {
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        if (attackingMon) {
#if EXPAND_ABILITIES
            if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) == ABIL_LONG_REACH) {
                return 0;
            }
#endif
#if EXPAND_ITEMS
            if (getMoveFlag(moveID, FLAG_PUNCH) &&
                BattleMon_GetUsableItem(attackingMon) == ITEM_PUNCHING_GLOVE) {
                return 0;
            }
#endif
        }
        return 1;
    }
    return 0;
}
extern "C" b32 MakesContact(ServerFlow * serverFlow, MOVE_ID moveID, u32 attackingSlot, u32 defendingSlot) {
    if (IsContact(serverFlow, moveID, attackingSlot)) {
#if EXPAND_ITEMS
        if (!BattleField_CheckEffect(FLDEFF_MAGIC_ROOM)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (attackingMon && BattleMon_GetUsableItem(attackingMon) == ITEM_PROTECTIVE_PADS) {
                    return 0;
            }
        }
#endif
        return 1;
    }
    return 0;
}

extern "C" void THUMB_BRANCH_SAFESTACK_CommonContactStatusAbility(ServerFlow * serverFlow, u32 currentSlot, CONDITION condition, ConditionData condData, u8 effectChance) {
    if (currentSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot) &&
            AbilityEvent_RollEffectChance(serverFlow, effectChance)) {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, currentSlot);
            addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            addCondition->condition = condition;
            addCondition->condData = condData;
            addCondition->almost = 0;
            addCondition->pokeID = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerRoughSkin(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot))
        {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (!BattleMon_IsFainted(attackingMon)) {
                HandlerParam_Damage* damage;
                damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                damage->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                damage->pokeID = attackingSlot;
                damage->damage = DivideMaxHPZeroCheck(attackingMon, 8u);
                BattleHandler_StrSetup(&damage->exStr, 2u, 430);
                BattleHandler_AddArg(&damage->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, damage);
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerAftermath(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
    if (pokemonSlot == defendingSlot) {

        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (BattleMon_IsFainted(defendingMon)) {

            MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
            if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);

                HandlerParam_Damage* damage;
                damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
                damage->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                damage->pokeID = attackingSlot;
                damage->damage = DivideMaxHPZeroCheck(attackingMon, 4u);
                damage->flags = damage->flags & 0xFE | 1;
                BattleHandler_StrSetup(&damage->exStr, 2u, 402);
                BattleHandler_AddArg(&damage->exStr, attackingSlot);
                BattleHandler_PopWork(serverFlow, damage);
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerPickpocket(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (HandlerCommon_CheckTargetMonID(pokemonSlot)) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (!HandlerCommon_CheckIfCanStealPokeItem(serverFlow, pokemonSlot, attackingSlot)) {

            MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
            u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
            if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {

                BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
                if (BattleMon_GetHeldItem(currentMon) == ITEM_NULL) {

                    BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
                    ITEM heldItem = BattleMon_GetHeldItem(attackingMon);
                    if (heldItem) {
                        HandlerParam_SwapItem* swapItem;
                        swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                        swapItem->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                        swapItem->pokeID = attackingSlot;
                        BattleHandler_StrSetup(&swapItem->exStr, 2u, 460);
                        BattleHandler_AddArg(&swapItem->exStr, attackingSlot);
                        BattleHandler_AddArg(&swapItem->exStr, heldItem);
                        BattleHandler_PopWork(serverFlow, swapItem);
                    }
                }
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerPoisonTouch(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !BattleEventVar_GetValue(VAR_SHIELD_DUST_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot) &&
            AbilityEvent_RollEffectChance(serverFlow, 30u)) {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
            addCondition->pokeID = defendingSlot;
            addCondition->condition = CONDITION_POISON;
            addCondition->condData = MakeBasicStatus(CONDITION_POISON);
            BattleHandler_StrSetup(&addCondition->exStr, 2u, 472);
            BattleHandler_AddArg(&addCondition->exStr, addCondition->pokeID);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerMummy(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)
        && !Handler_CheckMatchup(serverFlow)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, pokemonSlot)) {

            BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            if (defendingMon) {
                ABILITY ability = BattleMon_GetValue(defendingMon, VALUE_ABILITY);

                BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
                if (attackingMon && BattleMon_GetValue(attackingMon, VALUE_ABILITY) != ability) {
                    HandlerParam_ChangeAbility* changeAbility;
                    changeAbility = (HandlerParam_ChangeAbility*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_ABILITY, pokemonSlot);
                    changeAbility->ability = ability;
                    changeAbility->pokeID = attackingSlot;
                    BattleHandler_StrSetup(&changeAbility->exStr, 2u, 463);
                    BattleHandler_AddArg(&changeAbility->exStr, changeAbility->pokeID);
                    BattleHandler_AddArg(&changeAbility->exStr, changeAbility->ability);
                    if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot)) {
                        changeAbility->header.flags |= HANDLER_ABILITY_POPUP_FLAG;
                    }
                    BattleHandler_PopWork(serverFlow, changeAbility);
                }
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerStickyBarbDamageReaction(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            if (BattleMon_GetHeldItem(attackingMon) == ITEM_NULL) {
                HandlerParam_SwapItem* swapItem;
                swapItem = (HandlerParam_SwapItem*)BattleHandler_PushWork(serverFlow, EFFECT_SWAP_ITEM, pokemonSlot);
                swapItem->pokeID = attackingSlot;
                BattleHandler_PopWork(serverFlow, swapItem);
            }
        }
    }
}
extern "C" void THUMB_BRANCH_HandlerRockyHelmet(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON) &&
        !BattleEventVar_GetValue(VAR_SUBSTITUTE_FLAG)) {

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        if (MakesContact(serverFlow, moveID, attackingSlot, defendingSlot)) {
            BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
            u8 itemUseParam = CommonGetItemParam(item, ITSTAT_USE_PARAM);

            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, itemUseParam);
            BattleHandler_StrSetup(&damage->exStr, 2u, 424);
            BattleHandler_AddArg(&damage->exStr, damage->pokeID);
            BattleHandler_PopWork(serverFlow, damage);
        }
    }
}

extern "C" WEATHER Handler_CheckWeather(ServerFlow* serverFlow, u32 pokemonSlot, WEATHER weather) {
#if EXPAND_ITEMS
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (currentMon && BattleMon_GetUsableItem(currentMon) == ITEM_UTILITY_UMBRELLA) {
        return WEATHER_NULL;
    }
#endif
    return weather;
}
#endif // EXPAND_ABILITIES || EXPAND_ITEMS

#if EXPAND_ABILITIES || EXPAND_ITEMS || GRASS_IMMUNE_TO_POWDER || GEN6_OVERCOAT
// CONTACT REWORK - Modifies contact checks to allow the battle 
// engine to modify it from the items and abilities

// REDIRECTION CHANGES - Abilities and Items that ignore Follow Me and Rage Powder
#if EXPAND_ABILITIES
ABILITY abilityIgnoresBait[] = {
    ABIL_PROPELLER_TAIL,
    ABIL_STALWART,
};
extern "C" b32 AbilityIgnoresBait(ABILITY ability) { 
    return SEARCH_ARRAY(abilityIgnoresBait, ability); }
#endif
extern "C" void THUMB_BRANCH_HandlerFollowMeBaitTarget(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot, u32 * work) {
    BattleStyle battleStyle = BtlSetup_GetBattleStyle(serverFlow->mainModule);
    if (battleStyle != BTL_STYLE_SINGLE &&
        battleStyle != BTL_STYLE_ROTATION) {

        u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
    
        if (attackingMon) {
#if EXPAND_ABILITIES
            // Propeller Tail & Stallwart ignore redirection
            ABILITY ability = BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY);
            if (AbilityIgnoresBait(ability))
                return;
#endif
        
#if EXPAND_ITEMS || GRASS_IMMUNE_TO_POWDER || GEN6_OVERCOAT
            // If the redirection is made by a powder move
            if (getMoveFlag((MOVE_ID)item->subID, FLAG_POWDER)) {
#if GRASS_IMMUNE_TO_POWDER
                if (BattleMon_HasType(attackingMon, TYPE_GRASS))
                    return;
#endif
#if EXPAND_ITEMS
                if (BattleMon_GetUsableItem(attackingMon) == ITEM_SAFETY_GOGGLES)
                    return;
#endif
#if GEN6_OVERCOAT
                if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) == ABIL_OVERCOAT)
                    return;
#endif
            }
#endif
        }

        u16 moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!MainModule_IsAllyMonID(pokemonSlot, attackingSlot)
            && moveID != MOVE_SKY_DROP
            && !BattleMon_CheckIfMoveCondition(currentMon, CONDITION_SKYDROP)
            && !Handler_IsMonSwitchOutInterrupted(serverFlow)) {

            if (!Handler_IsTargetInRange(serverFlow, attackingSlot, pokemonSlot, moveID))
                BattleEventVar_RewriteValue(VAR_DEFENDING_MON, pokemonSlot);
        }
    }
}

#endif // EXPAND_ABILITIES || EXPAND_ITEMS || GRASS_IMMUNE_TO_POWDER

#if EXPAND_ABILITIES || EXPAND_ITEMS || ELECTRIC_IMMUNE_TO_PARALYSIS

ABILITY abilityImmuneToGastroAcid[] = {
    ABIL_MULTITYPE,
    ABIL_ZEN_MODE,
    ABIL_STANCE_CHANGE,
    ABIL_SHIELDS_DOWN,
    ABIL_SCHOOLING,
    ABIL_DISGUISE,
    ABIL_BATTLE_BOND,
    ABIL_POWER_CONSTRUCT,
    ABIL_COMATOSE,
    ABIL_RKS_SYSTEM,
    ABIL_GULP_MISSILE,
    ABIL_ICE_FACE,
    ABIL_AS_ONE_ICE,
    ABIL_AS_ONE_SHADOW,
    ABIL_ZERO_TO_HERO,
    ABIL_COMMANDER,
    ABIL_PROTOSYNTHESIS,
    ABIL_QUARK_DRIVE,
    ABIL_TERA_SHIFT,
};
extern "C" b32 AbilityImmuneToGastroAcid(ABILITY ability) {
    return SEARCH_ARRAY(abilityImmuneToGastroAcid, ability);
}
// Corrosion, Comatose & Neutralizing Gas - Handle when a condition should fail
extern "C" u32 THUMB_BRANCH_SAFESTACK_ServerControl_AddConditionCheckFail(ServerFlow * serverFlow, BattleMon * defendingMon, BattleMon * attackingMon, CONDITION condition, ConditionData condData, u8 overrideMode, u32 almost) {
    u32 failStatus = AddConditionCheckFailOverwrite(serverFlow, defendingMon, condition, condData, overrideMode);

#if EXPAND_ABILITIES
    // Ignore poison immunity if the Pokémon has Corrotion
    if (condition == CONDITION_POISON &&
        failStatus == 2 &&
        BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) == ABIL_CORROSION) {
        failStatus = 0;
    }
    // Make any status fail if the Pokémon has Comatose
    if ((condition < CONDITION_CONFUSION || condition == CONDITION_YAWN) &&
        defendingMon->currentAbility == ABIL_COMATOSE) {
        failStatus = 3;
    }

    if (condition == CONDITION_GASTROACID &&
        (defendingMon->conditions[condition].status) == 0) {

        switch (failStatus) {
        case 0:
            // Gastro Acid fails agains certain abilities
            if (AbilityImmuneToGastroAcid(BattleMon_GetValue(defendingMon, VALUE_ABILITY))) {
                failStatus = 3;
            }
            break;
        case 1:
            // Gastro Acid should not fail if the condition is already detected 
            // only because the Pokémon is affected by Neutralizing Gas
            failStatus = 0;
            break;
        }
    }
#endif
#if EXPAND_ITEMS
    // Utility Umbrella - Ignore defrost by harsh sunlight
    if (condition == CONDITION_FREEZE &&
        failStatus == 3 &&
        BattleMon_GetUsableItem(defendingMon) == ITEM_UTILITY_UMBRELLA) {
        failStatus = 0;
    }
#endif
#if ELECTRIC_IMMUNE_TO_PARALYSIS
    if (condition == CONDITION_PARALYSIS && 
        failStatus == 0 && 
        BattleMon_HasType(defendingMon, TYPE_ELEC)) {
        failStatus = 2;
    }
#endif

    if (failStatus) {
        if (almost) {
            AddConditionCheckFailStandard(serverFlow, defendingMon, failStatus, condition);
        }
        return 1;
    }
    else {
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);
        u32 failFlag = ServerEvent_MoveConditionCheckFail(serverFlow, attackingMon, defendingMon, condition);
        // Added new state to force the fail
        if ((failFlag && almost) || failFlag == FORCE_FAIL_MESSAGE) {
            ServerEvent_AddConditionFailed(serverFlow, defendingMon, attackingMon, condition);
            // This disables the default fail message
            serverFlow->field_78A |= 0x10u;
        }
        HEManager_PopState(&serverFlow->HEManager, HEID);
        return failFlag;
    }
}

#endif // EXPAND_ABILITIES || EXPAND_ITEMS || ELECTRIC_IMMUNE_TO_PARALYSIS

#if EXPAND_ABILITIES
// References for the implementations in https://bulbapedia.bulbagarden.net/wiki/Ability#List_of_Abilities 
// and https://www.youtube.com/@mupokepedia
//      CORROSION -> Implemented in [ServerControl_AddConditionCheckFail]
//      COMATOSE -> Implemented in [ServerControl_AddConditionCheckFail], [BattleMon_CheckIfMoveCondition], [HandlerHex] & [CommonStatusReaction]

// Called when the ability of a Pokemon stops being nullifyed [ServerControl_CureCondition]
extern "C" void ServerEvent_AbilityNullifyCured(ServerFlow * serverFlow, BattleMon * battleMon) {
    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    // Abilities with an effect when given also activate when they stop being nullifyed
    BattleEvent_CallHandlers(serverFlow, EVENT_AFTER_ABILITY_CHANGE);
    BattleEventVar_Pop();
}
// Called after receiving simple damage (status, recoil, items...) [ServerControl_SimpleDamageCore]
extern "C" void ServerEvent_SimpleDamageReaction(ServerFlow * serverFlow, BattleMon * battleMon, u32 damage) {
    u32 HEID = HEManager_PushState(&serverFlow->HEManager);

    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 currentSlot = BattleMon_GetID(battleMon);
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetConstValue(VAR_DAMAGE, damage);
    BattleEvent_CallHandlers(serverFlow, EVENT_SIMPLE_DAMAGE_REACTION);
    BattleEventVar_Pop();

    HEManager_PopState(&serverFlow->HEManager, HEID);
}
// Called when 2 Pokémon are swapping places [BattleHandler_SwapPoke]
extern "C" void ServerEvent_SwapPoke(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon) {
    u32 HEID = HEManager_PushState(&serverFlow->HEManager);

    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(NEW_VAR_DEFENDING_MON, defendingSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_SWAP_POKE);
    BattleEventVar_Pop();

    HEManager_PopState(&serverFlow->HEManager, HEID);
}
// Called before using a temp item [ServerEvent_EquipTempItem]
extern "C" void ServerEvent_UseTempItemBefore(ServerFlow * serverFlow, u32 currentSlot, u32 attackingSlot, ITEM itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_ForceCallHandlers(serverFlow, EVENT_USE_TEMP_ITEM_BEFORE);
    BattleEventVar_Pop();
}
// Called before any other switch-in events [ServerEvent_AfterSwitchInPrevious]
extern "C" void ServerEvent_SwitchInPriority(ServerFlow* serverFlow) {
    PokeSet_SeekStart(&serverFlow->currentpokeSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(&serverFlow->currentpokeSet); currentMon; currentMon = PokeSet_SeekNext(&serverFlow->currentpokeSet)) {
        u32 HEID = HEManager_PushState(&serverFlow->HEManager);

        u32 currentSlot = BattleMon_GetID(currentMon);
        BattleEventVar_Push();
        SET_UP_NEW_EVENT;
        BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
        BattleEvent_CallHandlers(serverFlow, EVENT_SWITCH_IN_PRIO);
        BattleEventVar_Pop();

        HEManager_PopState(&serverFlow->HEManager, HEID);
    }
}

ABILITY abilityCantBeNeutralized[] = {
    ABIL_MULTITYPE,
    ABIL_ZEN_MODE,
    ABIL_STANCE_CHANGE,
    ABIL_SHIELDS_DOWN,
    ABIL_SCHOOLING,
    ABIL_DISGUISE,
    ABIL_BATTLE_BOND,
    ABIL_COMATOSE,
    ABIL_RKS_SYSTEM,
    ABIL_GULP_MISSILE,
    ABIL_ICE_FACE,
    ABIL_NEUTRALIZING_GAS,
    ABIL_AS_ONE_ICE,
    ABIL_AS_ONE_SHADOW,
#if NEUTRALIZE_PARADOX_ABILITIES
    ABIL_PROTOSYNTHESIS,
    ABIL_QUARK_DRIVE,
#endif
    ABIL_TERA_SHIFT,
};
extern "C" b32 AbilityCantBeNeutralized(ABILITY ability) {
    return SEARCH_ARRAY(abilityCantBeNeutralized, ability); }

#if GEN9_INTIMIDATE_INTERACTIONS

extern "C" void HandlerIgnoreIntimidate(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && BattleEventVar_GetValue(VAR_INTIMIDATE_FLAG)) {
        BattleEventVar_RewriteValue(VAR_MOVE_FAIL_FLAG, 1);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

        HandlerParam_Message* message;
        message = (HandlerParam_Message*)BattleHandler_PushWork(serverFlow, EFFECT_MESSAGE, pokemonSlot);
        BattleHandler_StrSetup(&message->str, 2u, 201);
        BattleHandler_AddArg(&message->str, pokemonSlot);
        BattleHandler_PopWork(serverFlow, message);

        BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
    }
}

#endif // GEN9_INTIMIDATE_INTERACTIONS

struct AbilityEventAddTableExt
{
    ABILITY ability;
    AbilityEventAddFunc func;
    const char* dllName;
};
AbilityEventAddTableExt abilityEventAddTableExt[]{
#if GEN9_INTIMIDATE_INTERACTIONS
    { ABIL_OBLIVIOUS, EventAddObliviousUpdated, "Abilities/Oblivious" },
    { ABIL_OWN_TEMPO, EventAddOwnTempoUpdated, "Abilities/OwnTempo" },
    { ABIL_INNER_FOCUS, EventAddInnerFocusUpdated, "Abilities/InnerFocus" },
    { ABIL_SCRAPPY, EventAddScrappyUpdated, "Abilities/Scrappy" },
    { ABIL_RATTLED, EventAddRattledUpdated, "Abilities/Rattled" },
#endif
#if GEN7_LIGHTNING_ROD
    { ABIL_LIGHTNING_ROD, EventAddLightningRodUpdated, "Abilities/LightningRod" },
#endif
#if GEN6_KEEN_EYE
    { ABIL_KEEN_EYE, EventAddKeenEyeUpdated, "Abilities/KeenEye" },
#endif
#if GEN9_ILLUMINATE
    { ABIL_ILLUMINATE, EventAddKeenEyeUpdated, "Abilities/KeenEye" },
#endif
#if GEN6_TRACE
    { ABIL_TRACE, EventAddTraceUpdated, "Abilities/Trace" },
#endif
#if GEN8_SOUNDPROOF
    { ABIL_SOUNDPROOF, EventAddSoundproofUpdated, "Abilities/Soundproof" },
#endif
#if GEN7_NORMALIZE
    { ABIL_NORMALIZE, EventAddNormalizeUpdated, "Abilities/Normalize" },
#endif
#if GEN6_FRISK
    { ABIL_FRISK, EventAddFriskUpdated, "Abilities/Frisk" },
#endif
#if GEN6_OVERCOAT
    { ABIL_OVERCOAT, EventAddOvercoatUpdated, "Abilities/Overcoat" },
#endif
#if GEN7_PRANKSTER
    { ABIL_PRANKSTER, EventAddPranksterUpdated, "Abilities/Prankster" },
#endif
    { ABIL_AROMA_VEIL, EventAddAromaVeil, "Abilities/AromaVeil" },
    { ABIL_FLOWER_VEIL, EventAddFlowerVeil, "Abilities/FlowerVeil" },
    { ABIL_CHEEK_POUCH, EventAddCheekPouch, "Abilities/CheekPouch" },
    { ABIL_PROTEAN, EventAddProtean, "Abilities/Protean" },
    { ABIL_FUR_COAT, EventAddFurCoat, "Abilities/FurCoat" },
    { ABIL_MAGICIAN, EventAddMagician, "Abilities/Magician" },
    { ABIL_BULLETPROOF, EventAddBulletProof, "Abilities/BulletProof" },
    { ABIL_COMPETITIVE, EventAddCompetitive, "Abilities/Competitive" },
    { ABIL_STRONG_JAW, EventAddStrongJaw, "Abilities/StrongJaw" },
    { ABIL_REFRIGERATE, EventAddRefrigerate, "Abilities/Refrigerate" },
    { ABIL_SWEET_VEIL, EventAddSweetVeil, "Abilities/SweetVeil" },
    { ABIL_STANCE_CHANGE, EventAddStanceChange, "Abilities/StanceChange" },
    { ABIL_GALE_WINGS, EventAddGaleWings, "Abilities/GaleWings" },
    { ABIL_MEGA_LAUNCHER, EventAddMegaLauncher, "Abilities/MegaLauncher" },
    { ABIL_GRASS_PELT, EventAddGrassPelt, "Abilities/GrassPelt" },
    { ABIL_SYMBIOSIS, EventAddSymbiosis, "Abilities/Symbiosis" },
    { ABIL_TOUGH_CLAWS, EventAddToughClaws, "Abilities/ToughClaws" },
    { ABIL_PIXILATE, EventAddPixilate, "Abilities/Pixilate" },
    { ABIL_GOOEY, EventAddGooey, "Abilities/Gooey" },
    { ABIL_AERILATE , EventAddAerilate, "Abilities/Aerilate" },
    { ABIL_PARENTAL_BOND, EventAddParentalBond, "Abilities/ParentalBond" },
    { ABIL_DARK_AURA, EventAddDarkAura, "Abilities/DarkAura" },
    { ABIL_FAIRY_AURA, EventAddFairyAura, "Abilities/FairyAura" },
    { ABIL_AURA_BREAK, EventAddAuraBreak, "Abilities/AuraBreak" },
    { ABIL_PRIMORDIAL_SEA, EventAddPrimordialSea, "Abilities/PrimordialSea" },
    { ABIL_DESOLATE_LAND, EventAddDesolateLand, "Abilities/DesolateLand" },
    { ABIL_DELTA_STREAM, EventAddDeltaStream, "Abilities/DeltaStream" },
    { ABIL_STAMINA, EventAddStamina, "Abilities/Stamina" },
    { ABIL_WIMP_OUT, EventAddEmergencyExit, "Abilities/EmergencyExit" },
    { ABIL_EMERGENCY_EXIT, EventAddEmergencyExit, "Abilities/EmergencyExit" },
    { ABIL_WATER_COMPACTION, EventAddWaterCompaction, "Abilities/WaterCompaction" },
    { ABIL_MERCILESS, EventAddMerciless, "Abilities/Merciless" },
    { ABIL_SHIELDS_DOWN, EventAddShieldsDown, "Abilities/ShieldsDown" },
    { ABIL_STAKEOUT, EventAddStakeout, "Abilities/Stakeout" },
    { ABIL_WATER_BUBBLE, EventAddWaterBubble, "Abilities/BurnPrevention" },
    { ABIL_STEELWORKER, EventAddSteelworker, "Abilities/Steelworker" },
    { ABIL_BERSERK, EventAddBerserk, "Abilities/Berserk" },
    { ABIL_SLUSH_RUSH, EventAddSlushRush, "Abilities/SlushRush" },
    { ABIL_LIQUID_VOICE, EventAddLiquidVoice, "Abilities/LiquidVoice" },
    { ABIL_TRIAGE, EventAddTriage, "Abilities/Triage" },
    { ABIL_GALVANIZE, EventAddGalvanize, "Abilities/Galvanize" },
    { ABIL_SURGE_SURFER, EventAddSurgeSurfer, "Abilities/SurgeSurfer" },
    { ABIL_SCHOOLING, EventAddSchooling, "Abilities/Schooling" },
    { ABIL_DISGUISE, EventAddDisguise, "Abilities/Disguise" },
    { ABIL_BATTLE_BOND, EventAddBattleBond, "Abilities/BattleBond" },
    { ABIL_POWER_CONSTRUCT, EventAddPowerConstruct, "Abilities/PowerConstruct" },
    { ABIL_QUEENLY_MAGESTY, EventAddQueenlyMagesty, "Abilities/QueenlyMagesty" },
    { ABIL_INNARDS_OUT, EventAddInnardsOut, "Abilities/InnardsOut" },
    { ABIL_DANCER, EventAddDancer, "Abilities/Dancer" },
    { ABIL_BATTERY, EventAddBattery, "Abilities/Battery" },
    { ABIL_FLUFFY, EventAddFluffy, "Abilities/Fluffy" },
    { ABIL_DAZZLING, EventAddQueenlyMagesty, "Abilities/QueenlyMagesty" },
    { ABIL_SOUL_HEART, EventAddSoulHeart, "Abilities/SoulHeart" },
    { ABIL_TANGLING_HAIR, EventAddGooey, "Abilities/Gooey" },
    { ABIL_RECEIVER, EventAddReceiver, "Abilities/Receiver" },
    { ABIL_POWER_OF_ALCHEMY, EventAddReceiver, "Abilities/Receiver" },
    { ABIL_BEAST_BOOST, EventAddBeastBoost, "Abilities/BeastBoost" },
    { ABIL_ELECTRIC_SURGE, EventAddElectricSurge, "Abilities/ElectricSurge" },
    { ABIL_PSYCHIC_SURGE, EventAddPsychicSurge, "Abilities/PsychicSurge" },
    { ABIL_MISTY_SURGE, EventAddMistySurge, "Abilities/MistySurge" },
    { ABIL_GRASSY_SURGE, EventAddGrassySurge, "Abilities/GrassySurge" },
    { ABIL_FULL_METAL_BODY, EventAddClearBody, nullptr },
    { ABIL_SHADOW_SHIELD, EventAddMultiscale, nullptr },
    { ABIL_PRISM_ARMOR, EventAddPrismArmor, "Abilities/PrismArmor" },
    { ABIL_NEUROFORCE, EventAddNeuroforce, "Abilities/Neuroforce" },
    { ABIL_INTREPID_SWORD, EventAddIntrepidSword, "Abilities/IntrepidSword" },
    { ABIL_DAUNTLESS_SHIELD, EventAddDauntlessShield, "Abilities/DauntlessShield" },
    { ABIL_LIBERO, EventAddProtean, "Abilities/Protean" },
    { ABIL_BALL_FETCH, EventAddBallFetch, "Abilities/BallFetch" },
    { ABIL_COTTON_DOWN, EventAddCottonDown, "Abilities/CottonDown" },
    { ABIL_PROPELLER_TAIL, EventAddPropellerTail, "Abilities/PropellerTail" },
    { ABIL_MIRROR_ARMOR, EventAddMirrorArmor, "Abilities/MirrorArmor" },
    { ABIL_GULP_MISSILE, EventAddGulpMissile, "Abilities/GulpMissile" },
    { ABIL_STALWART, EventAddPropellerTail, "Abilities/PropellerTail" },
    { ABIL_STEAM_ENGINE, EventAddSteamEngine, "Abilities/SteamEngine" },
    { ABIL_PUNK_ROCK, EventAddPunkRock, "Abilities/PunkRock" },
    { ABIL_SAND_SPIT, EventAddSandSpit, "Abilities/SandSpit" },
    { ABIL_ICE_SCALES, EventAddIceScales, "Abilities/IceScales" },
    { ABIL_RIPEN, EventAddRipen, "Abilities/Ripen" },
    { ABIL_ICE_FACE, EventAddIceFace, "Abilities/IceFace" },
    { ABIL_POWER_SPOT, EventAddPowerSpot, "Abilities/PowerSpot" },
    { ABIL_MIMICRY, EventAddMimicry, "Abilities/Mimicry" },
    { ABIL_SCREEN_CLEANER, EventAddScreenCleaner, "Abilities/ScreenCleaner" },
    { ABIL_STEELY_SPIRIT, EventAddSteelySpirit, "Abilities/SteelySpirit" },
    { ABIL_PERISH_BODY, EventAddPerishBody, "Abilities/PerishBody" },
    { ABIL_WANDERING_SPIRIT, EventAddWanderingSpirit, "Abilities/WanderingSpirit" },
    { ABIL_GORILLA_TACTICS, EventAddGorillaTactics, "Abilities/GorillaTactics" },
    { ABIL_NEUTRALIZING_GAS, EventAddNeutralizingGas, "Abilities/NeutralizingGas" },
    { ABIL_PASTEL_VEIL, EventAddPastelVeil, "Abilities/PastelVeil" },
    { ABIL_HUNGER_SWITCH, EventAddHungerSwitch, "Abilities/HungerSwitch" },
    { ABIL_QUICK_DRAW, EventAddQuickDraw, "Abilities/QuickDraw" },
    { ABIL_UNSEEN_FIST, EventAddUnseenFist, "Abilities/UnseenFist" },
    { ABIL_CURIOUS_MEDICINE, EventAddCuriousMedicine, "Abilities/CuriousMedicine" },
    { ABIL_TRANSISTOR, EventAddTransistor, "Abilities/Transistor" },
    { ABIL_DRAGON_S_MAW, EventAddDragonsMaw, "Abilities/DragonsMaw" },
    { ABIL_CHILLING_NEIGH, EventAddChillingNeigh, "Abilities/AsOne" },
    { ABIL_GRIM_NEIGH, EventAddGrimNeigh, "Abilities/AsOne" },
    { ABIL_AS_ONE_ICE, EventAddAsOne, "Abilities/AsOne" },
    { ABIL_AS_ONE_SHADOW, EventAddAsOne, "Abilities/AsOne" },
    { ABIL_LINGERING_AROMA, EventAddMummy, nullptr },
    { ABIL_SEED_SOWER, EventAddSeedSower, "Abilities/SeedSower" },
    { ABIL_THERMAL_EXCHANGE, EventAddThermalExchange, "Abilities/BurnPrevention" },
    { ABIL_ANGER_SHELL, EventAddAngerShell, "Abilities/AngerShell" },
    { ABIL_PURIFYING_SALT, EventAddPurifyingSalt, "Abilities/BurnPrevention" },
    { ABIL_WELL_BAKED_BODY, EventAddWellBakedBody, "Abilities/WellBakedBody" },
    { ABIL_WIND_RIDER, EventAddWindRider, "Abilities/WindBoosted" },
    { ABIL_GUARD_DOG, EventAddGuardDog, "Abilities/GuardDog" },
    { ABIL_ROCKY_PAYLOAD, EventAddRockyPayload, "Abilities/RockyPayload" },
    { ABIL_WIND_POWER, EventAddWindPower, "Abilities/WindBoosted" },
    { ABIL_ZERO_TO_HERO, EventAddZeroToHero, "Abilities/ZeroToHero" },
    { ABIL_COMMANDER, EventAddCommander, "Abilities/Commander" },
    { ABIL_ELECTROMORPHOSIS, EventAddElectromorphosis, "Abilities/WindBoosted" },
    { ABIL_PROTOSYNTHESIS, EventAddProtosynthesis, "Abilities/Paradox" },
    { ABIL_QUARK_DRIVE, EventAddQuarkDrive, "Abilities/Paradox" },
    { ABIL_GOOD_AS_GOLD, EventAddGoodAsGold, "Abilities/GoodAsGold" },
    { ABIL_VESSEL_OF_RUIN, EventAddVesselOfRuin, "Abilities/Ruin" },
    { ABIL_SWORD_OF_RUIN, EventAddSwordOfRuin, "Abilities/Ruin" },
    { ABIL_TABLETS_OF_RUIN, EventAddTabletsOfRuin, "Abilities/Ruin" },
    { ABIL_BEADS_OF_RUIN, EventAddBeadsOfRuin, "Abilities/Ruin" },
    { ABIL_ORICHALCUM_PULSE, EventAddOrichalcumPulse, "Abilities/OrichalcumPulse" },
    { ABIL_HADRON_ENGINE, EventAddHadronEngine, "Abilities/HadronEngine" },
    { ABIL_OPPORTUNIST , EventAddOpportunist, "Abilities/Opportunist" },
    { ABIL_CUD_CHEW, EventAddCudChew, "Abilities/CudChew" },
    { ABIL_SHARPNESS, EventAddSharpness, "Abilities/Sharpness" },
    { ABIL_SUPREME_OVERLORD, EventAddSupremeOverlord, "Abilities/SupremeOverlord" },
    { ABIL_COSTAR, EventAddCostar, "Abilities/Costar" },
    { ABIL_TOXIC_DEBRIS, EventAddToxicDebris, "Abilities/ToxicDebris" },
    { ABIL_ARMOR_TAIL, EventAddQueenlyMagesty, "Abilities/QueenlyMagesty" },
    { ABIL_EARTH_EATER, EventAddEarthEater, "Abilities/EarthEater" },
    { ABIL_MYCELIUM_MIGHT, EventAddMyceliumMight, "Abilities/MyceliumMight" },
    { ABIL_HOSPITALITY, EventAddHospitality, "Abilities/Hospitality" },
    { ABIL_MIND_S_EYE, EventAddMindsEye, "Abilities/MindsEye" },
    { ABIL_EMBODY_ASPECT_TEAL , EventAddEmbodyAspectTeal, "Abilities/EmbodyAspect" },
    { ABIL_EMBODY_ASPECT_HEARTHFLAME , EventAddEmbodyAspectHearthflame, "Abilities/EmbodyAspect" },
    { ABIL_EMBODY_ASPECT_WELLSPRING , EventAddEmbodyAspectWellspring, "Abilities/EmbodyAspect" },
    { ABIL_EMBODY_ASPECT_CORNERSTONE , EventAddEmbodyAspectCornerstorne, "Abilities/EmbodyAspect" },
    { ABIL_TOXIC_CHAIN, EventAddToxicChain, "Abilities/ToxicChain" },
    { ABIL_SUPERSWEET_SYRUP, EventAddSupersweetSyrup, "Abilities/SupersweetSyrup" },
    { ABIL_TERA_SHIFT, EventAddTeraShift, "Abilities/TeraShift" },
    { ABIL_TERA_SHELL, EventAddTeraShell, "Abilities/TeraShell" },
    { ABIL_TERAFORM_ZERO, EventAddTeraformZero, "Abilities/TeraformZero" },
    { ABIL_POISON_PUPPETEE, EventAddPoisonPuppeteer, "Abilities/PoisonPuppeteer" },
};

extern "C" BattleEventItem * GetAbiltiyEvent(BattleMon * battleMon, ABILITY ability, AbilityEventAddFunc func) {
    u32 handlerAmount = 0;
    BattleEventHandlerTableEntry* battleHandlerItems = func(&handlerAmount);

    BattleEventPriority mainPrio = GetHandlerMainPriority(&handlerAmount);
    u32 subPrio = AbilityEvent_GetSubPriority(battleMon);
    u32 pokemonSlot = BattleMon_GetID(battleMon);
    return BattleEvent_AddItem(EVENTITEM_ABILITY, ability, mainPrio, subPrio, pokemonSlot, battleHandlerItems, handlerAmount);
}

extern "C" BattleEventItem* THUMB_BRANCH_AbilityEvent_AddItem(BattleMon* battleMon) {
    ABILITY ability = BattleMon_GetValue(battleMon, VALUE_ABILITY);

    // Check for new or overriden abilities
    for (u32 abilityEvent = 0; abilityEvent < ARRAY_COUNT(abilityEventAddTableExt); ++abilityEvent) {
        
        AbilityEventAddTableExt* abilityAddEvent = &abilityEventAddTableExt[abilityEvent];
        if (ability == abilityAddEvent->ability) {
            // Load the DLL with the code for the ability if needed
            if (!LoadDll(abilityAddEvent->dllName))
                return 0;

            return GetAbiltiyEvent(battleMon, ability, abilityAddEvent->func);
        }
    }
    // Check for vanilla abilities
    AbilityEventAddTable* abilityEventAddTable = (AbilityEventAddTable*)0x21D7F38;
    for (u32 abilityEvent = 0; abilityEvent < 158; ++abilityEvent) {

        AbilityEventAddTable* abilityAddEvent = &abilityEventAddTable[abilityEvent];
        if (ability == abilityAddEvent->ability) {
            return GetAbiltiyEvent(battleMon, ability, abilityAddEvent->func);
        }
    }
    return 0;
}

ABILITY moldBreakerAffectedAbilitiesExt[]{
    ABIL_BATTLE_ARMOR,
    ABIL_STURDY,
    ABIL_DAMP,
    ABIL_LIMBER,
    ABIL_SAND_VEIL,
    ABIL_VOLT_ABSORB,
    ABIL_WATER_ABSORB,
    ABIL_OBLIVIOUS,
    ABIL_INSOMNIA,
    ABIL_IMMUNITY,
    ABIL_FLASH_FIRE,
    ABIL_SHIELD_DUST,
    ABIL_OWN_TEMPO,
    ABIL_SUCTION_CUPS,
    ABIL_WONDER_GUARD,
    ABIL_LEVITATE,
    ABIL_CLEAR_BODY,
    ABIL_LIGHTNING_ROD,
    ABIL_ILLUMINATE,
    ABIL_INNER_FOCUS,
    ABIL_MAGMA_ARMOR,
    ABIL_WATER_VEIL,
    ABIL_SOUNDPROOF,
    ABIL_THICK_FAT,
    ABIL_KEEN_EYE,
#if GEN9_ILLUMINATE
    ABIL_ILLUMINATE,
#endif
    ABIL_HYPER_CUTTER,
    ABIL_STICKY_HOLD,
    ABIL_MARVEL_SCALE,
    ABIL_VITAL_SPIRIT,
    ABIL_WHITE_SMOKE,
    ABIL_SHELL_ARMOR,
    ABIL_TANGLED_FEET,
    ABIL_MOTOR_DRIVE,
    ABIL_SNOW_CLOAK,
    ABIL_HEATPROOF,
    ABIL_SIMPLE,
    ABIL_DRY_SKIN,
    ABIL_LEAF_GUARD,
    ABIL_UNAWARE,
    ABIL_FILTER,
    ABIL_STORM_DRAIN,
    ABIL_SOLID_ROCK,
    ABIL_FLOWER_GIFT,
    ABIL_CONTRARY,
    ABIL_FRIEND_GUARD,
    ABIL_HEAVY_METAL,
    ABIL_LIGHT_METAL,
    ABIL_MULTISCALE,
    ABIL_TELEPATHY,
    ABIL_OVERCOAT,
    ABIL_BIG_PECKS,
    ABIL_WONDER_SKIN,
    ABIL_MAGIC_BOUNCE,
    ABIL_SAP_SIPPER,
    ABIL_AROMA_VEIL,
    ABIL_FLOWER_VEIL,
    ABIL_FUR_COAT,
    ABIL_BULLETPROOF,
    ABIL_SWEET_VEIL,
    ABIL_AURA_BREAK,
    ABIL_WATER_BUBBLE,
    ABIL_DISGUISE,
    ABIL_QUEENLY_MAGESTY,
    ABIL_FLUFFY,
    ABIL_DAZZLING,
    ABIL_MIRROR_ARMOR,
    ABIL_PUNK_ROCK,
    ABIL_ICE_SCALES,
    ABIL_ICE_FACE,
    ABIL_PASTEL_VEIL,
    ABIL_THERMAL_EXCHANGE,
    ABIL_PURIFYING_SALT,
    ABIL_WELL_BAKED_BODY,
    ABIL_WIND_RIDER,
    ABIL_GUARD_DOG,
    ABIL_GOOD_AS_GOLD,
    ABIL_VESSEL_OF_RUIN,
    ABIL_TABLETS_OF_RUIN,
    ABIL_ARMOR_TAIL,
    ABIL_EARTH_EATER,
    ABIL_MIND_S_EYE,
    ABIL_TERA_SHELL,
};
extern "C" b32 THUMB_BRANCH_IsMoldBreakerAffectedAbility(ABILITY ability) {
    return SEARCH_ARRAY(moldBreakerAffectedAbilitiesExt, ability);
}

// Symbiosis - Store temporary items
extern "C" BattleEventItem * THUMB_BRANCH_ItemEvent_AddTempItem(BattleMon * battleMon, ITEM itemID)
{
    if (!itemID) {
        return 0;
    }

    BattleEventItem* itemEvents = ItemEvent_AddItemCore(battleMon, itemID);
    if (!itemEvents) {
        return 0;
    }

    BattleEventItem_SetTempItemFlag(itemEvents);
    BattleEventItem_SetWorkValue(itemEvents, 6, 1);

    g_BattleField->tempItem = itemID; // store the temp item ID
    return itemEvents;
}

MOVE_ID movesThatIgnoreParentalBond[]{
    MOVE_FLING,
    MOVE_SELF_DESTRUCT,
    MOVE_EXPLOSION,
    MOVE_FINAL_GAMBIT,
    MOVE_UPROAR,
    MOVE_ROLLOUT,
    MOVE_ICE_BALL,
    MOVE_ENDEAVOR,
};
extern "C" b32 MoveIgnoresParentalBond(MOVE_ID moveID) { 
    return SEARCH_ARRAY(movesThatIgnoreParentalBond, moveID); }
extern "C" b32 ParentalBondCheck(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* attackingMon, PokeSet* targetSet) {
    if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_PARENTAL_BOND) {
        return 0;
    }

    u32 targetCount = PokeSet_GetCount(targetSet);
    // Only when targeting 1 Pokémon will Parental Bond work
    if (targetCount && targetCount == 1) {

        MoveParam* moveParam;
        ServerEvent_GetMoveParam(serverFlow, moveID, attackingMon, moveParam);
        // Self-targeting and side moves don't work
        if (moveParam->targetType == TARGET_USER ||
            moveParam->targetType == TARGET_FIELD ||
            moveParam->targetType == TARGET_SIDE_ENEMY ||
            moveParam->targetType == TARGET_SIDE_ALLY) {
            return 0;
        }

        // Charge up moves never work
        if (getMoveFlag(moveParam->moveID, FLAG_REQUIRES_CHARGE)) {
            return 0;
        }

        // There are specific moves that don't work
        if (MoveIgnoresParentalBond(moveParam->moveID)) {
            return 0;
        }

        return 1;
    }
    return 0;
}
extern "C" void THUMB_BRANCH_LINK_ServerControl_DamageRoot_0x36(ServerFlow* serverFlow, BattleMon* attackingMon, MOVE_ID moveID, HitCheckParam* hitCheckParam) {
    ServerEvent_CheckMultihitHits(serverFlow, attackingMon, moveID, hitCheckParam);
    
    // Reset any substitute damage recorded
    sys_memset(g_BattleField->actionSubstituteDamage, 0, BATTLE_MAX_SLOTS * sizeof(u32));

    // Reset Parental Bond flag
    BattleField_ResetParentalBondFlag();
    if (!serverFlow->hitCheckParam->multiHitMove && ParentalBondCheck(serverFlow, moveID, attackingMon, serverFlow->setTargetOriginal)) {
        // I use [serverFlow->setTargetOriginal] so that Parental Bond takes into account 
        // all original targets even if they had Immunities of if the move missed
        serverFlow->hitCheckParam->countMax = 2;
        serverFlow->hitCheckParam->checkEveryTime = 0;
        serverFlow->hitCheckParam->multiHitMove = 1;

        // Set the Parental Bond flag so that the second hit has to deal less damage, used in [HandlerParentalBondPower]
        BattleField_SetParentalBondFlag();
    }
}

// Generic Ability Change ability check
ABILITY abilityFailChangeGeneric[] = {
    ABIL_MULTITYPE,
    ABIL_ZEN_MODE,
    ABIL_STANCE_CHANGE,
    ABIL_SHIELDS_DOWN,
    ABIL_SCHOOLING,
    ABIL_DISGUISE,
    ABIL_BATTLE_BOND,
    ABIL_COMATOSE,
    ABIL_RKS_SYSTEM,
    ABIL_GULP_MISSILE,
    ABIL_ICE_FACE,
    ABIL_ZERO_TO_HERO,
    ABIL_COMMANDER,
    ABIL_TERA_SHIFT,
};
extern "C" b32 AbilityFailChangeGeneric(ABILITY ability) { 
    return SEARCH_ARRAY(abilityFailChangeGeneric, ability); }
extern "C" b32 THUMB_BRANCH_j_j_Tables_IsAbilityMultitype(ABILITY ability) {
    return AbilityFailChangeGeneric(ability);
}

// Trace opposing ability check
ABILITY abilityFailChange[] = {
    ABIL_TRACE,
    ABIL_FORECAST,
    ABIL_MULTITYPE,
    ABIL_FLOWER_GIFT,
    ABIL_ZEN_MODE,
    ABIL_ILLUSION,
    ABIL_IMPOSTER,
    ABIL_WONDER_GUARD,
    ABIL_STANCE_CHANGE,
    ABIL_SHIELDS_DOWN,
    ABIL_SCHOOLING,
    ABIL_DISGUISE,
    ABIL_BATTLE_BOND,
    ABIL_POWER_CONSTRUCT,
    ABIL_COMATOSE,
    ABIL_RECEIVER,
    ABIL_POWER_OF_ALCHEMY,
    ABIL_RKS_SYSTEM,
    ABIL_GULP_MISSILE,
    ABIL_ICE_FACE,
    ABIL_NEUTRALIZING_GAS,
    ABIL_HUNGER_SWITCH,
    ABIL_ZERO_TO_HERO,
    ABIL_COMMANDER,
    ABIL_PROTOSYNTHESIS,
    ABIL_QUARK_DRIVE,
    ABIL_ORICHALCUM_PULSE,
    ABIL_HADRON_ENGINE,
    ABIL_TERA_SHIFT,
    ABIL_TERA_SHELL,
    ABIL_TERAFORM_ZERO,
};
extern "C" b32 AbilityFailChange(ABILITY ability) { 
    return SEARCH_ARRAY(abilityFailChange, ability); }
// Trace ability checks
extern "C" b32 THUMB_BRANCH_j_j_IsTraceFailAbility(ABILITY ability) {
    if (ability == ABIL_WONDER_GUARD ||
        ability == ABIL_ORICHALCUM_PULSE ||
        ability == ABIL_HADRON_ENGINE) {
        return 0;
    }
    else {
        return AbilityFailChange(ability);
    }
}
extern "C" b32 THUMB_BRANCH_j_j_IsTraceFailAbility0(ABILITY ability) {
    if (ability == ABIL_WONDER_GUARD ||
        ability == ABIL_ORICHALCUM_PULSE ||
        ability == ABIL_HADRON_ENGINE) {
        return 0;
    }
    else {
        return AbilityFailChange(ability);
    }
}
// Entrainment opposing ability check
extern "C" b32 THUMB_BRANCH_j_j_j_IsTraceFailAbility1(ABILITY ability) {
    // Entrainment can override Wonder Guard
    if (ability == ABIL_WONDER_GUARD) {
        return 0;
    }

    return AbilityFailChangeGeneric(ability);
}
// Role Play opposing ability check
extern "C" b32 THUMB_BRANCH_j_j_IsRolePlayFailAbility(ABILITY ability) {
    return AbilityFailChange(ability);
}

// Skill Swap current ability check
ABILITY abilityFailSwap[] = {
    ABIL_WONDER_GUARD,
    ABIL_MULTITYPE,
    ABIL_ILLUSION,
    ABIL_ZEN_MODE,
    ABIL_STANCE_CHANGE,
    ABIL_SHIELDS_DOWN,
    ABIL_SCHOOLING,
    ABIL_DISGUISE,
    ABIL_BATTLE_BOND,
    ABIL_COMATOSE,
    ABIL_RECEIVER,
    ABIL_POWER_OF_ALCHEMY,
    ABIL_RKS_SYSTEM,
    ABIL_GULP_MISSILE,
    ABIL_ICE_FACE,
    ABIL_NEUTRALIZING_GAS,
    ABIL_HUNGER_SWITCH,
    ABIL_ZERO_TO_HERO,
    ABIL_COMMANDER,
    ABIL_PROTOSYNTHESIS,
    ABIL_QUARK_DRIVE,
    ABIL_ORICHALCUM_PULSE,
    ABIL_HADRON_ENGINE,
    ABIL_TERA_SHIFT,
    ABIL_TERA_SHELL,
    ABIL_TERAFORM_ZERO,
};
extern "C" b32 AbilityFailSwap(ABILITY ability) { 
    return SEARCH_ARRAY(abilityFailSwap, ability); }
extern "C" b32 THUMB_BRANCH_j_CheckSkillSwapFailAbility(ABILITY ability) {
    return AbilityFailSwap(ability);
}
// Skill Swap opposing ability check
extern "C" b32 THUMB_BRANCH_j_CheckSkillSwapFailAbility0(ABILITY ability) {
    return AbilityFailSwap(ability);
}

extern "C" u32 MultiplyValueByRatio(u32 value, u32 ratio);
// Mega Launcher - Added move ID to check move flags
extern "C" u32 THUMB_BRANCH_ServerEvent_CalcMoveHealAmount(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* currentMon)
{
    u32 finalHeal = 0;

    u32 healParam = PML_MoveGetParam(moveID, MVDATA_HEAL);
    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(currentMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_RATIO, 0);
    // Added move ID
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveID);
    BattleEvent_CallHandlers(serverFlow, EVENT_RECOVER_HP);
    u32 ratio = BattleEventVar_GetValue(VAR_RATIO);
    BattleEventVar_Pop();
    u32 healAmount = BattleMon_GetValue(currentMon, VALUE_MAX_HP);

    if (ratio)
        finalHeal = fixed_round(healAmount, ratio);
    else
        finalHeal = MultiplyValueByRatio(healAmount, healParam);

    if (!finalHeal)
        return 1;
    if (finalHeal > healAmount)
        return healAmount;
    return finalHeal;
}

// Neutralizing Gas - Added event when an ability stops being nullyfied (EVENT_AFTER_ABILITY_CHANGE)
extern "C" void THUMB_BRANCH_ServerControl_CureCondition(ServerFlow * serverFlow, BattleMon * battleMon, CONDITION condition, ConditionData * prevCondition) {
    if (condition) {
        u32 pokemonSlot = BattleMon_GetID(battleMon);
        if (prevCondition) {
            *prevCondition = BattleMon_GetMoveCondition(battleMon, condition);
        }
        if (condition >= CONDITION_CONFUSION) {
            BattleMon_CureMoveCondition(battleMon, condition);
            if (condition == CONDITION_GASTROACID) {
                // Call server event when ability nullify is over
                ServerEvent_AbilityNullifyCured(serverFlow, battleMon);
            }
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_CureMoveCondition, pokemonSlot, condition);
        }
        else {
            CureStatusCondition(battleMon);
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_CureStatusCondition, pokemonSlot);
            if (PosPoke_GetPokeExistPos(&serverFlow->posPoke, pokemonSlot) != NULL_BATTLE_POS) {
                ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_StatusIcon, pokemonSlot, 0);
            }
        }
    }
}

// Emergency Exit - Add event when being damaged by simple damage
extern "C" b32 THUMB_BRANCH_ServerControl_SimpleDamageCore(ServerFlow * serverFlow, BattleMon * battleMon, int damage, HandlerParam_StrParams * str) {
    int damageToDeal = -damage;
    if (!damageToDeal) {
        return 0;
    }

    ServerDisplay_SimpleHP(serverFlow, battleMon, damageToDeal, 1);
    TurnFlag_Set(battleMon, TURNFLAG_DAMAGED);
    if (str) {
        BattleHandler_SetString(serverFlow, str);
        BattleHandler_StrClear(str);
    }

    ServerControl_CheckItemReaction(serverFlow, battleMon, 1);
    // Call to new event
    ServerEvent_SimpleDamageReaction(serverFlow, battleMon, damage);
    if (ServerControl_CheckFainted(serverFlow, battleMon)) {
        ServerControl_CheckMatchup(serverFlow);
    }

    return 1;
}

// Emergency Exit - Store damage taken by the substitute to acuratelly calculate HP when hit
extern "C" b32 THUMB_BRANCH_BattleMon_AddSubstituteDamage(BattleMon * battleMon, u32 * damage) {
    b32 result;

    u32 substituteHP = battleMon->substituteHP;
    if (substituteHP > *damage) {
        battleMon->substituteHP = substituteHP - *damage;
        result = 0;
    }
    else {
        *damage = substituteHP;
        battleMon->substituteHP = 0;
        result = 1;
    }

    u32 substituteDamage = BattleField_GetSubstituteDamage(battleMon->battleSlot) + *damage;
    BattleField_SetSubstituteDamage(battleMon->battleSlot, substituteDamage);
    return result;
}

// Diguise & Ice Face - Only remove the damage if there are no more hits remaining
extern "C" void THUMB_BRANCH_ServerControl_ApplyDamageToRecover(ServerFlow* serverFlow, BattleMon* attackingMon, MoveParam* moveParam, PokeSet* targetSet) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(targetSet); currentMon; currentMon = PokeSet_SeekNext(targetSet)) {
        
        u32 checkDamageFlag = ServerEvent_CheckDamageToRecover(serverFlow, attackingMon, currentMon, moveParam);
        if (checkDamageFlag) {
            MoveAnimCtrl* moveAnimCtrl = serverFlow->moveAnimCtrl;
            if ((moveAnimCtrl->flags & 1) == 0) {
                moveAnimCtrl->flags = moveAnimCtrl->flags & 0xFE | 1;
            }

            u32 HEID = HEManager_PushState(&serverFlow->HEManager);
            ServerEvent_ApplyDamageToRecover(serverFlow, attackingMon, currentMon);
            HEManager_PopState(&serverFlow->HEManager, HEID);

            switch (checkDamageFlag)
            {
            case 1:
                PokeSet_Remove(targetSet, currentMon);
                break;
            case 2:
                --serverFlow->hitCheckParam->countMax;
                if (!serverFlow->hitCheckParam->countMax) {
                    PokeSet_Remove(targetSet, currentMon);
                }
                break;
            }
        }
    }
}

// Comatose & Neutralizing Gas - Return permanent conditions
extern "C" b32 THUMB_BRANCH_BattleMon_CheckIfMoveCondition(BattleMon* battleMon, CONDITION condition) {
    if (condition == CONDITION_SLEEP && BattleMon_GetValue(battleMon, VALUE_EFFECTIVE_ABILITY) == ABIL_COMATOSE) {
        return 1;
    }

    if (condition == CONDITION_GASTROACID && 
        BattleField_GetNeutralizingGasMons() &&
        !AbilityCantBeNeutralized(BattleMon_GetValue(battleMon, VALUE_ABILITY))) {
        return 1;
    }

    return (battleMon->conditions[condition].status) != 0;
}

// Comatose - Hex is not boosted against a Comatose Pokémon
extern "C" void THUMB_BRANCH_HandlerHex(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {

        u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
        BattleMon* defendingMon = Handler_GetBattleMon(serverFlow, defendingSlot);
        if (BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_PARALYSIS) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_SLEEP) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_FREEZE) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_BURN) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_POISON) ||
            BattleMon_CheckIfMoveCondition(defendingMon, CONDITION_PARALYSIS)) {

            u32 power = BattleEventVar_GetValue(VAR_MOVE_POWER);
            BattleEventVar_RewriteValue(VAR_MOVE_POWER, power * 2);
        }
    }
}

// Comatose - Comatose sleep state should not be healed by items
extern "C" void THUMB_BRANCH_CommonStatusReaction(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, CONDITION condition) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (currentMon->currentAbility == ABIL_COMATOSE) {
        return;
    }
    
    CONDITION conditionCpy = condition;
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)
        && (!BattleEventVar_GetValueIfExist(VAR_ITEM_REACTION, &conditionCpy) || conditionCpy == CONDITION_FREEZE || conditionCpy == CONDITION_NONE)
        && CommonConditionCodeMatch(serverFlow, pokemonSlot, condition)) {
        ItemEvent_PushRun(item, serverFlow, pokemonSlot);
    }
}

// Dancer - Dancer Move-Lock moves don't Move-Lock the user
extern "C" void THUMB_BRANCH_HandlerThrash(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Ignore this effect if used by Dancer
        if (CheckExtraActionFlag()) {
            MoveEvent_ForceRemoveItemFromBattleMon(currentMon, item->subID);
            return;
        }
        if (!BattleMon_CheckIfMoveCondition(currentMon, CONDITION_MOVELOCK) && !work[6]) {
            u32 maxTurns = (BattleRandom(2u) + 2);
        
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->condition = CONDITION_MOVELOCK;
            addCondition->condData = Condition_MakeTurnParam(maxTurns, item->subID);
            addCondition->almost = 0;
            addCondition->pokeID = pokemonSlot;
            BattleHandler_PopWork(serverFlow, addCondition);
            work[6] = 1;
            *work = maxTurns;
        }
    }
}

// Dancer - Dancer Move-Lock moves don't add to the non-Dancer Move-Lock count
extern "C" void THUMB_BRANCH_HandlerThrashEnd(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {

    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && work[6]) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        b32 finished = 0;
        // Don't count Dancer uses of the move
        if (*work && !CheckExtraActionFlag()) {
            --*work;
        }
        if (!BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG)) {
            HandlerParam_CureCondition* cureCondition;
            cureCondition = (HandlerParam_CureCondition*)BattleHandler_PushWork(serverFlow, EFFECT_CURE_STATUS, pokemonSlot);
            cureCondition->condition = CONDITION_MOVELOCK;
            cureCondition->pokeCount = 1;
            cureCondition->pokeID[0] = pokemonSlot;
            BattleHandler_PopWork(serverFlow, cureCondition);
        
            finished = 1;
        }
        if (!*work)
        {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->condition = CONDITION_CONFUSION;
            MakeCondition(CONDITION_CONFUSION, currentMon, &addCondition->condData);
            addCondition->reserved = 1;
            addCondition->pokeID = pokemonSlot;
            BattleHandler_StrSetup(&addCondition->exStr, 2u, 360u);
            BattleHandler_AddArg(&addCondition->exStr, pokemonSlot);
            BattleHandler_PopWork(serverFlow, addCondition);

            finished = 1;
        }
        if (finished) {
            MoveEvent_ForceRemoveItemFromBattleMon(currentMon, item->subID);
        }
    }
}

// Stalwart - Swap targets when the enemies swap position
extern "C" b32 THUMB_BRANCH_BattleHandler_SwapPoke(ServerFlow* serverFlow, HandlerParam_SwapPoke* params) {
    if (params->pokeID1 == params->pokeID2) {
        return 0;
    }
    u32 clientID1 = MainModule_PokeIDToClientID(params->pokeID1);
    if (clientID1 != MainModule_PokeIDToClientID(params->pokeID2)) {
        return 0;
    }

    BattleMon* battleMon1 = PokeCon_GetBattleMon(serverFlow->pokeCon, params->pokeID1);
    BattleMon* battleMon2 = PokeCon_GetBattleMon(serverFlow->pokeCon, params->pokeID2);
    if (BattleMon_IsFainted(battleMon1) || BattleMon_IsFainted(battleMon2)) {
        return 0;
    }

    BattleParty* battleParty = PokeCon_GetBattleParty(serverFlow->pokeCon, clientID1);
    int partyMon1 = BattleParty_FindPartyMon(battleParty, battleMon1);
    int partyMon2 = BattleParty_FindPartyMon(battleParty, battleMon2);
    if (partyMon1 < 0 || partyMon2 < 0) {
        return 0;
    }
    ServerControl_MoveCore(serverFlow, clientID1, partyMon1, partyMon2, 0);
    BattleHandler_SetString(serverFlow, &params->exStr);
    ServerControl_AfterMove(serverFlow, clientID1, partyMon1, partyMon2);

    // Added an event
    ServerEvent_SwapPoke(serverFlow, battleMon1, battleMon2);
    return 1;
}

// Stalwart - Ignore move redirection
extern "C" b32 THUMB_BRANCH_CommonRedirectMove(ServerFlow* serverFlow, u32 pokemonSlot, u32* work, POKE_TYPE pokeType) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    if (attackingSlot != pokemonSlot && pokeType == BattleEventVar_GetValue(VAR_MOVE_TYPE)) {

        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        if (attackingMon) {
            ABILITY ability = BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY);
            if (AbilityIgnoresBait(ability))
                return 0;
        }

        MOVE_ID moveID = BattleEventVar_GetValue(VAR_MOVE_ID);
        if (!Handler_IsTargetInRange(serverFlow, attackingSlot, pokemonSlot, moveID) && !IsComboMove(moveID)) {

            u32 defendingSlot = BattleEventVar_GetValue(VAR_DEFENDING_MON);
            if (BattleEventVar_RewriteValue(VAR_DEFENDING_MON, pokemonSlot) && defendingSlot != pokemonSlot)
            {
                return 1;
            }
        }
    }
    return 0;
}

// Ripen - Double PP restore
extern "C" u32 CommonLeppaBerryLastMoveID(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" u32 CommonLeppaBerryEnableMoveID(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" void THUMB_BRANCH_HandlerLeppaBerryCommon(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, b32 isTemp) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);

    u32 moveSlot = CommonLeppaBerryLastMoveID(serverFlow, pokemonSlot);
    if (moveSlot == 4) {

        moveSlot = CommonLeppaBerryEnableMoveID(serverFlow, pokemonSlot);
        if (moveSlot == 4 && isTemp) {
            moveSlot = 0;

            u32 moveCount = BattleMon_GetMoveCount(currentMon);
            if (moveCount) {
                while (!Move_GetMissingPP(currentMon, moveSlot)) {
                    if (++moveSlot >= moveCount) {
                        return;
                    }
                }
            }
        }
    }

    if (moveSlot < 4)
    {
        HandlerParam_PP* pp;
        pp = (HandlerParam_PP*)BattleHandler_PushWork(serverFlow, EFFECT_RESTORE_PP, pokemonSlot);
        pp->volume = 10;
        // Double the PP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
            pp->volume *= 2;
        }
        pp->pokeID = pokemonSlot;
        pp->moveSlot = moveSlot;
        pp->flags = pp->flags & 0xFE | 1;
        BattleHandler_StrSetup(&pp->exStr, 2u, 911u);
        BattleHandler_AddArg(&pp->exStr, pokemonSlot);
        BattleHandler_AddArg(&pp->exStr, item->subID);
        MOVE_ID moveID = Move_GetID(currentMon, moveSlot);
        BattleHandler_AddArg(&pp->exStr, moveID);
        BattleHandler_PopWork(serverFlow, pp);
    }
}

// Ripen - Double heal
extern "C" void THUMB_BRANCH_HandlerOranBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        HandlerParam_RecoverHP* recoverHP;
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
            recoverHP->recoverHP *= 2;
        }
        BattleHandler_StrSetup(&recoverHP->exStr, 2u, 908u);
        BattleHandler_AddArg(&recoverHP->exStr, pokemonSlot);
        BattleHandler_AddArg(&recoverHP->exStr, item->subID);
        BattleHandler_PopWork(serverFlow, recoverHP);
    }
}

// Ripen - Double heal
extern "C" void THUMB_BRANCH_HandlerSitrusBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        u32 maxHP = BattleMon_GetValue(currentMon, VALUE_MAX_HP);
        u32 healPercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
            healPercent *= 2;
        }

        HandlerParam_RecoverHP* recoverHP;
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = maxHP * healPercent / 100u;
        BattleHandler_StrSetup(&recoverHP->exStr, 2u, 908u);
        BattleHandler_AddArg(&recoverHP->exStr, pokemonSlot);
        BattleHandler_AddArg(&recoverHP->exStr, item->subID);
        BattleHandler_PopWork(serverFlow, recoverHP);
    }
}

// Ripen - Double heal
extern "C" s8 doesNatureAffectStat(PartyPkm* pPkm, u32 stat);
extern "C" void THUMB_BRANCH_SAFESTACK_CommonPinchBerry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u32 natureType) {
    if (!*work && pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        u32 healPercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        
        HandlerParam_RecoverHP* recoverHP;
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = DivideMaxHPZeroCheck(currentMon, healPercent);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
            recoverHP->recoverHP *= 2;
        }
        BattleHandler_StrSetup(&recoverHP->exStr, 2u, 908u);
        BattleHandler_AddArg(&recoverHP->exStr, pokemonSlot);
        BattleHandler_AddArg(&recoverHP->exStr, item->subID);
        BattleHandler_PopWork(serverFlow, recoverHP);

        PartyPkm* partyPkm = BattleMon_GetSrcData(currentMon);
        if (doesNatureAffectStat(partyPkm, natureType) == -1)
        {
            HandlerParam_AddCondition* addCondition;
            addCondition = (HandlerParam_AddCondition*)BattleHandler_PushWork(serverFlow, EFFECT_ADD_CONDITION, pokemonSlot);
            addCondition->pokeID = pokemonSlot;
            addCondition->condition = CONDITION_CONFUSION;
            MakeCondition(CONDITION_CONFUSION, currentMon, &addCondition->condData);
            BattleHandler_PopWork(serverFlow, addCondition);
        }
        *work = 1;
    }
}

// Ripen - Double stat boost
extern "C" void THUMB_BRANCH_CommonStatBoostBerry(ServerFlow* serverFlow, u32 pokemonSlot, StatStage stat, u8 volume) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        HandlerParam_ChangeStatStage* statChange;
        statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
        statChange->stat = stat;
        statChange->volume = volume;
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        // Double the stat boost if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
            statChange->volume *= 2;
        }
        statChange->pokeCount = 1;
        statChange->pokeID[0] = pokemonSlot;
        BattleHandler_PopWork(serverFlow, statChange);
    }
}

// Ripen - Double heal
extern "C" void THUMB_BRANCH_HandlerEnigmaBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID))
    {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        u32 healPercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
        // Double the HP gained if Ripen is active
        if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
            healPercent *= 2;
        }
        
        HandlerParam_RecoverHP* recoverHP; 
        recoverHP = (HandlerParam_RecoverHP*)BattleHandler_PushWork(serverFlow, EFFECT_RECOVER_HP, pokemonSlot);
        recoverHP->pokeID = pokemonSlot;
        recoverHP->recoverHP = DivideMaxHPZeroCheck(currentMon, healPercent);
        BattleHandler_PopWork(serverFlow, recoverHP);
    }
}

// Ripen - Double damage reduction
extern "C" void THUMB_BRANCH_SAFESTACK_CommonResistBerry(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work, u8 pokeType, b32 skipEffectivenessCheck) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_DEFENDING_MON)
        && pokeType == BattleEventVar_GetValue(VAR_MOVE_TYPE)
        && (skipEffectivenessCheck || BattleEventVar_GetValue(VAR_TYPE_EFFECTIVENESS) > 3)) {

        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        if (!BattleMon_IsSubstituteActive(currentMon)) {

            u32 ratio = 2048;
            // Double the resistence if Ripen is active
            if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
                ratio /= 2;
            }

            BattleEventVar_MulValue(VAR_RATIO, ratio);
            if (!Handler_IsSimulationMode(serverFlow)) {
                *work = 1;
            }
        }
    }
}

// Ripen - Double damage
extern "C" void THUMB_BRANCH_HandlerJabocaRowapBerryUse(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID) && *work) {
        u32 attackingSlot = *work - 1;
        BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
        if (!BattleMon_IsFainted(attackingMon)) {
            u32 damagePercent = CommonGetItemParam(item, ITSTAT_USE_PARAM);
            BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
            // Double the damage if Ripen is active
            if (BattleMon_GetValue(currentMon, VALUE_EFFECTIVE_ABILITY) == ABIL_RIPEN) {
                damagePercent /= 2;
            }
        
            HandlerParam_Damage* damage;
            damage = (HandlerParam_Damage*)BattleHandler_PushWork(serverFlow, EFFECT_DAMAGE, pokemonSlot);
            damage->pokeID = attackingSlot;
            damage->damage = DivideMaxHPZeroCheck(attackingMon, damagePercent);
            BattleHandler_StrSetup(&damage->exStr, 2u, 1031u);
            BattleHandler_AddArg(&damage->exStr, damage->pokeID);
            BattleHandler_AddArg(&damage->exStr, pokemonSlot);
            BattleHandler_AddArg(&damage->exStr, item->subID);
            BattleHandler_PopWork(serverFlow, damage);
        }
    }
}

// Ice Face - Add move ID parameter to check for contact
extern "C" u32 THUMB_BRANCH_ServerEvent_CheckDamageToRecover(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam) {
    BattleEventVar_Push();
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveParam->moveType);
    // Add move category
    BattleEventVar_SetConstValue(VAR_MOVE_CATEGORY, moveParam->category);
    BattleEventVar_SetRewriteOnceValue(VAR_GENERAL_USE_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_DAMAGE_TO_RECOVER);
    u32 generalFlag = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
    BattleEventVar_Pop();
    return generalFlag;
}

// Neutralizing Gas - Add server event that activates before every other switch-in event
extern "C" void THUMB_BRANCH_ServerEvent_AfterSwitchInPrevious(ServerFlow* serverFlow) {
    // Add an event that triggers before every other switch in event
    ServerEvent_SwitchInPriority(serverFlow);

    BattleEventVar_Push();
    BattleEvent_CallHandlers(serverFlow, EVENT_SWITCH_IN_PREVIOUS);
    BattleEventVar_Pop();
}

// Unseen Fist - Bypass Wide Guard and Quick Guard
extern "C" b32 CheckUnseenFist(ServerFlow* serverFlow) {
    u32 attackingSlot = BattleEventVar_GetValue(VAR_ATTACKING_MON);
    BattleMon* attackingMon = Handler_GetBattleMon(serverFlow, attackingSlot);
    if (attackingMon &&
        BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) == ABIL_UNSEEN_FIST) {
        return true;
    }
    return false;
}

// As One - Pass ability flag data to the server function
extern "C" b32 THUMB_BRANCH_BattleHandler_AbilityPopupAdd(ServerFlow* serverFlow, HandlerParam_Header* params) {
    u32 battleSlot = params->flags << 19 >> 27;
    if (!DoesBattleMonExist(&serverFlow->posPoke, battleSlot)) {
        return 0;
    }

    // Add the forced ability to the server call
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_AbilityPopupIn, battleSlot | BattleField_GetAsOnePopUpFlags());
    // Clear the variable for the client
    BattleField_SetAsOnePopUpFlags(0);
    return 1;
}

// As One - Pass ability flag data to the ability popup
extern "C" u32 THUMB_BRANCH_ServerCommand_AbilityPopupIn(BtlClientWk* client, u32* state, u32* data) {
    u32 battleSlot = *data & 0x1F;
    u32 battlePos = MainModule_PokeIDToPokePos(client->mainModule, client->pokeCon, battleSlot);
    if (*state) {
        if (*state != 1) {
            return 1;
        }

        if (!BattleViewCmd_AbilityPopup_DisplayWait(client->btlvCore, battlePos)) {
            return 0;
        }
    }
    else {
        if (IsChapterSkipMode(client)) {
            return 1;
        }

        // Set the forced ability for the server
        BattleField_SetAsOnePopUpFlags(*data & 0xE0);
        BattleViewCmd_AbilityPopup_DisplayStart(client->btlvCore, battlePos, 1);
    }
    ++(*state);
    return 0;
}

// As One - Ability popup change
extern "C" u32 THUMB_BRANCH_LINK_AbilityPopup_DisplayStart_0x18(BattleMon* battleMon, BattleMonValue value) {
    ABILITY ability = BattleMon_GetValue(battleMon, value);

    if (BattleField_GetAsOnePopUpFlags()) {
        if (BattleField_GetAsOnePopUpFlags() == AS_ONE_FIRST_ABILITY_FLAG) {
            ability = ABIL_UNNERVE;
        } 
        else if (BattleField_GetAsOnePopUpFlags() == AS_ONE_SECOND_ABILITY_FLAG) {
            if (ability == ABIL_AS_ONE_ICE) {
                ability = ABIL_CHILLING_NEIGH;
            }
            else if (ability == ABIL_AS_ONE_SHADOW) {
                ability = ABIL_GRIM_NEIGH;
            }
        }

        // Clear the variable for the server
        BattleField_SetAsOnePopUpFlags(0);
    }

    return ability;
}

// Commander - Add special hide avoid code
extern "C" void THUMB_BRANCH_ServerFlow_CheckPokeHideAvoid(ServerFlow* serverFlow, u16* moveID, BattleMon* attackingMon, PokeSet* targetSet) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(targetSet); currentMon; currentMon = PokeSet_SeekNext(targetSet)) {

        // Always miss if the Pokémon is invulnerable because of Commander
        b32 isCommander = (!BattleMon_CheckIfMoveCondition(currentMon, CONDITION_CHARGELOCK) &&
            BattleMon_GetHideCondition(currentMon) == CONDITIONFLAG_SHADOWFORCE);
        if (isCommander) {
            PokeSet_Remove(targetSet, currentMon);

            // Skip message when a non-targeted move is avoided
            MoveTarget moveTarget = (MoveTarget)PML_MoveGetParam((MOVE_ID)*moveID, MVDATA_TARGET);
            if (moveTarget <= TARGET_ENEMY_SELECT || !isCommander) {
                ServerDisplay_MoveAvoid(serverFlow, currentMon);
            }

            continue;
        }
        

        if (!ServerControl_IsGuaranteedHit(serverFlow, attackingMon, currentMon)) {
            if (ServerEvent_CheckHiding(serverFlow, attackingMon, currentMon)) {
                PokeSet_Remove(targetSet, currentMon);
                ServerDisplay_MoveAvoid(serverFlow, currentMon);
            }
        }
    }
}

// Commander - Make No-Action flag permanent while paired
extern "C" void THUMB_BRANCH_LINK_ServerFlow_ClearPokeNoActionFlag_0x34(ServerFlow* serverFlow, BattleMon* battleMon, CONDITION_FLAG conditionFlag) {
    b32 isCommander = (!BattleMon_CheckIfMoveCondition(battleMon, CONDITION_CHARGELOCK) &&
        BattleMon_GetHideCondition(battleMon) == CONDITIONFLAG_SHADOWFORCE);
    if (!isCommander) {
        ServerDisplay_ResetConditionFlag(serverFlow, battleMon, conditionFlag);
    }
}

// Commander - Make force switches fail while paired
extern "C" u32 THUMB_BRANCH_ServerEvent_CheckForceSwitch(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon) {
    BattleEventVar_Push();
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_FORCE_SWITCH);
    Condition_CheckForceSwitchFail(serverFlow, defendingMon);
    u32 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();
    // Fail to force a switch if the Pokémon is a Dondozo paired by Commander
    if (BattleField_IsDondozoPaired(defendingMon->battleSlot)) {
        failFlag = 1;
    }
    return failFlag;
}

#endif // EXPAND_ABILITIES

#if GEN7_INFILTRATOR

extern "C" void THUMB_BRANCH_SAFESTACK_ServerControl_SubstituteExclude(ServerFlow * serverFlow, MoveParam * moveParams, BattleMon * attackingMon, PokeSet * targetSet, int isDmgMove) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* currentMon = PokeSet_SeekNext(targetSet); currentMon; currentMon = PokeSet_SeekNext(targetSet)) {

        if (attackingMon != currentMon &&
            BattleMon_IsSubstituteActive(currentMon) &&
            !isDmgMove &&
            !getMoveFlag(moveParams->moveID, FLAG_BYPASSES_SUBSTITUTE)) {

            if (BattleMon_GetValue(attackingMon, VALUE_EFFECTIVE_ABILITY) != ABIL_INFILTRATOR) {
                PokeSet_Remove(targetSet, currentMon);
            }
        }
    }
}

#endif // GEN7_INFILTRATOR

#if EXPAND_ABILITIES || GRASS_IMMUNE_TO_POWDER

#if EXPAND_ABILITIES
// Unseen Fist - Modified CheckProtectBreak server event
extern "C" b32 ServerEvent_CheckProtectBreakExt(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon, MOVE_ID moveID)
{
    BattleEventVar_Push();
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetValue(VAR_ATTACKING_MON, attackingSlot);
    // Added the defending slot for fun, it's used in Unseen Fist but it's not needed
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetValue(VAR_DEFENDING_MON, defendingSlot);
    // Added the Move ID so that Unseen Fist can check if it is contact
    BattleEventVar_SetValue(VAR_MOVE_ID, moveID);
    BattleEventVar_SetValue(VAR_GENERAL_USE_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_PROTECT_BREAK);
    b32 protectBroken = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
    BattleEventVar_Pop();
    return protectBroken;
}
#endif
extern "C" void THUMB_BRANCH_SAFESTACK_flowsub_CheckNoEffect_Protect(ServerFlow * serverFlow, u16 * moveID, BattleMon * attackingMon, PokeSet * targetSet, int dmgAffRec) {
    PokeSet_SeekStart(targetSet);
    for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
        if (!ServerControl_IsGuaranteedHit(serverFlow, attackingMon, targetMon)
            && ServerControl_CheckNoEffectCore(serverFlow, moveID, attackingMon, targetMon, dmgAffRec, EVENT_REDIRECT_TARGETEND)) {
            PokeSet_Remove(targetSet, targetMon);
        }
    }
    PokeSet_SeekStart(targetSet);
    for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
        if (ServerControl_CheckNoEffectCore(serverFlow, moveID, attackingMon, targetMon, dmgAffRec, EVENT_NOEFFECT_CHECK)) {
            PokeSet_Remove(targetSet, targetMon);
        }
    }
    if (getMoveFlag(*moveID, FLAG_BLOCKED_BY_PROTECT)) {
        PokeSet_SeekStart(targetSet);
        for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
            if (BattleMon_GetTurnFlag(targetMon, TURNFLAG_PROTECT)
#if EXPAND_ABILITIES
                && !ServerEvent_CheckProtectBreakExt(serverFlow, attackingMon, targetMon, *moveID)) {
#else
                && !ServerEvent_CheckProtectBreak(serverFlow, attackingMon)) {
#endif
                PokeSet_Remove(targetSet, targetMon);
                u32 targetSlot = BattleMon_GetID(targetMon);
                ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, 523, targetSlot, 0xFFFF0000);
            }
        }
    }
    PokeSet_SeekStart(targetSet);
    for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
#if GRASS_IMMUNE_TO_POWDER
        if (getMoveFlag(*moveID, FLAG_POWDER) && BattleMon_HasType(targetMon, TYPE_GRASS)) {
            // If a Grass Pokémon is hit by a non-self-targeting move
            u32 targetSlot = BattleMon_GetID(targetMon);
            if (targetSlot != BattleMon_GetID(attackingMon)) {

                // Display "no effect" message
                u32 HEID = HEManager_PushState(&serverFlow->HEManager);
                if (BattleHandler_StrIsEnabled(&serverFlow->strParam)) {
                    BattleHandler_SetString(serverFlow, &serverFlow->strParam);
                    BattleHandler_StrClear(&serverFlow->strParam);
                }
                else if (!BattleHandler_Result(serverFlow)) {
                    ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, 210, targetSlot, 0xFFFF0000);
                }
                HEManager_PopState(&serverFlow->HEManager, HEID);

                // Remove from the target list
                PokeSet_Remove(targetSet, targetMon);
                continue;
            }
        }
#endif
        if (ServerControl_CheckNoEffectCore(serverFlow, moveID, attackingMon, targetMon, dmgAffRec, EVENT_ABILITY_CHECK_NO_EFFECT)) {
            PokeSet_Remove(targetSet, targetMon);
        }
    }
}

#endif // EXPAND_ABILITIES || GRASS_IMMUNE_TO_POWDER

#if EXPAND_ABILITIES || ADD_MEGA_EVOLUTION

// Mega-Evolution - Reset the Mega-Evolution related values
// Stakeout - Store first turn Pokémon to be able to check if a Pokémon switches in the firt turn
extern "C" void THUMB_BRANCH_LINK_ServerFlow_SetupBeforeFirstTurn_0x6E(ServerFlow * serverFlow, u32 clientID, 
    u32 switchInSlot, u32 switchOutSlot) {

#if ADD_MEGA_EVOLUTION
    // Reset the Mega-Evolution flag at the start of the battle
    megaEvolveFlag = UNTOGGLED_MEGA;
#endif

    ServerControl_SwitchInCore(serverFlow, clientID, switchInSlot, switchOutSlot);

#if EXPAND_ABILITIES
    // Store the starting Pokémon in the firstTurnMons array
    BattleParty* party = PokeCon_GetBattleParty(serverFlow->pokeCon, clientID);
    BattleMon* switchInMon = BattleParty_GetPartyMember(party, switchInSlot);
    for (u8 i = 0; i < 6; ++i) {
        if (g_BattleField->firstTurnMons[i] == BATTLE_MAX_SLOTS) {
            g_BattleField->firstTurnMons[i] = switchInMon->battleSlot;
            break;
        }
    }
#endif
}

// Mega-Evolution - Reset the ability when returning to base form
extern "C" void THUMB_BRANCH_BattleMon_UpdateData(BattleMon * battleMon, bool resetForm)
{
    PartyPkm* partyPkm = battleMon->partySrc;
    PokeParty_SetParam(battleMon->partySrc, PF_Experience, battleMon->experience);
    PokeParty_SetParam(partyPkm, PF_NowHP, battleMon->currentHP);

    u32 status = 0;
    if (battleMon->currentHP)
        status = BattleMon_GetStatus(battleMon);
    PokeParty_SetParam(partyPkm, PF_StatusCond, status);

    BattleMon_SetMovesAndPP(battleMon);

    u8 form = battleMon->form;
    if ((battleMon->flags & 0x20) != 0 || resetForm)
    {
        form = battleMon->flags & 0x1F;
        // Reset to the base ability
        PokeParty_SetParam(battleMon->partySrc, PF_Ability, battleMon->ability);
    }

    PokeParty_SetParam(partyPkm, PF_Forme, form);
    PokeParty_RecalcStats(partyPkm);
    PokeParty_SetParam(partyPkm, PF_Item, battleMon->heldItem);
}

// Mega-Evolution - Use the third function param added in BattleUpgrade.S
// As One - Activate Unnerve code
extern "C" void THUMB_BRANCH_ServerControl_UnnerveAction(ServerFlow * serverFlow, BattleMon * battleMon, ABILITY ability) {
    if (ability == ABIL_UNNERVE 
#if EXPAND_ABILITIES
        || ability == ABIL_AS_ONE_ICE
        || ability == ABIL_AS_ONE_SHADOW
#endif
        ) {

        FRONT_POKE_SEEK_WORK frontSet[4];
        FRONT_POKE_SEEK_InitWork(frontSet, serverFlow);

        u32 battleSlot = BattleMon_GetID(battleMon);
        BattleMon* frontMon;
        while (FRONT_POKE_SEEK_GetNext(frontSet, serverFlow, &frontMon)) {
            u32 frontSlot = BattleMon_GetID(frontMon);
            if (!MainModule_IsAllyMonID(battleSlot, frontSlot)) {
                ServerControl_CheckItemReaction(serverFlow, frontMon, 0);
            }
        }
    }
}

#endif // EXPAND_ABILITIES || ADD_MEGA_EVOLUTION

#if EXPAND_ABILITIES || ADD_MEGA_EVOLUTION || EXPAND_MOVES

// Mega-Evolution - Prevent Mega-Evolution form changes to be reverted when switching
// Abilities - Prevent certain form changes to be reverted when switching
// Moves - Reset the extra typing when switching out
ABILITY abilityDontChangeFormOnSwitchOut[] = {
    ABIL_BATTLE_BOND,
    ABIL_DISGUISE,
    ABIL_ICE_FACE,
    ABIL_POWER_CONSTRUCT,
    ABIL_ZERO_TO_HERO,
    ABIL_TERA_SHIFT,
    ABIL_TERA_SHELL,
    ABIL_TERAFORM_ZERO,
};
extern "C" b32 AbilityDontChangeFormOnSwitchOut(ABILITY ability) {
    return SEARCH_ARRAY(abilityDontChangeFormOnSwitchOut, ability);
}
extern "C" void THUMB_BRANCH_BattleMon_ClearForSwitchOut(BattleMon* battleMon) {
    sys_memset(battleMon->turnFlag, 0, 2u);
    BattleMon_ClearTransformChange(battleMon);
    MoveWork_ClearSurface(battleMon);
    BattleMon_ClearUsedMoveFlag(battleMon);
    ClearCounter(battleMon);
    BattleMon_ClearComboMoveData(battleMon);
    BattleMon_IllusionBreak(battleMon);
    if (!BattleMon_GetConditionFlag(battleMon, CONDITIONFLAG_BATONPASS))
    {
        BattleMon_RemoveSubstitute(battleMon);
        ClearMoveStatusWork(battleMon, 0);
        ResetStatStages(&battleMon->statStageParam);
        sys_memset(battleMon->conditionFlag, 0, 2u);
    }

    // Don't reset battle-permanent form changes
#if ADD_MEGA_EVOLUTION
    u8 megaevolution = CanMegaEvolve(battleMon);
#else
    u8 megaevolution = 0;
#endif

#if EXPAND_ABILITIES
    if (!AbilityDontChangeFormOnSwitchOut(battleMon->currentAbility) &&
        (!megaevolution || megaevolution != battleMon->form)) {
        battleMon->form = battleMon->flags & 0x1F;
        battleMon->currentAbility = battleMon->ability;
    }
    else
    {
        // Reset to the ability of the current form if the Pokémon doesn't revert to base
        battleMon->currentAbility = PokeParty_GetParam(battleMon->partySrc, PF_Ability, 0);
    }
#else
    battleMon->form = battleMon->flags & 0x1F;
    battleMon->currentAbility = battleMon->ability;
#endif

#if EXPAND_MOVES
    BattleField_SetExtraType(battleMon->battleSlot, TYPE_NULL);
#endif

    PokeParty_SetParam(battleMon->partySrc, PF_Forme, battleMon->form);
    PokeParty_RecalcStats(battleMon->partySrc);
}

#endif // EXPAND_ABILITIES || ADD_MEGA_EVOLUTION || MOVE_EXPANSION

#if EXPAND_ABILITIES || EXPAND_MOVES

// Called when a held item is consumed [ServerControl_ChangeHeldItem]
extern "C" void ServerEvent_ConsumeItem(ServerFlow* serverFlow, u32 currentSlot, ITEM itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_CallHandlers(serverFlow, EVENT_CONSUME_ITEM);
    BattleEventVar_Pop();
}
// Called after using a held item [ServerEvent_EquipItem]
extern "C" void ServerEvent_UseItemAfter(ServerFlow* serverFlow, u32 currentSlot, ITEM itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_CallHandlers(serverFlow, EVENT_USE_ITEM_AFTER);
    BattleEventVar_Pop();
}
// Called after using a temp item [ServerEvent_EquipTempItem]
extern "C" void ServerEvent_UseTempItemAfter(ServerFlow* serverFlow, u32 currentSlot, u32 attackingSlot, ITEM itemID) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetValue(NEW_VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_ForceCallHandlers(serverFlow, EVENT_USE_TEMP_ITEM_AFTER);
    BattleEventVar_Pop();
}

// Cheek Pouch & Symbiosis - Add event right after consuming an item
// Belch - Records if a berry has been consumed
#if EXPAND_MOVES
extern "C" int THUMB_BRANCH_BattleHandler_ConsumeItem(ServerFlow* serverFlow, HandlerParam_ConsumeItem* params) {
    BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->header.flags << 19 >> 27);
    if (!params->dontUse) {
        ServerDisplay_UseHeldItem(serverFlow, battleMon);
        BattleHandler_SetString(serverFlow, &params->exStr);
    }
    // pass the dontUse value to discriminate for Belch
    ServerControl_ChangeHeldItem(serverFlow, battleMon, ITEM_NULL, 1 + params->dontUse);
    return 1;
}
#endif
extern "C" void THUMB_BRANCH_ServerControl_ChangeHeldItem(ServerFlow* serverFlow, BattleMon* battleMon, ITEM itemID, b32 consumeItem)
{
    u32 battleSlot = BattleMon_GetID(battleMon);
    ITEM usedItem = BattleMon_GetHeldItem(battleMon);

    u32 HEID = HEManager_PushState(&serverFlow->HEManager);
    ServerEvent_ItemSetDecide(serverFlow, battleMon, itemID);
    HEManager_PopState(&serverFlow->HEManager, HEID);

    if (!itemID) {
        ServerDisplay_SetConditionFlag(serverFlow, battleMon, CONDITIONFLAG_NULL);
    }
    ItemEvent_RemoveItem(battleMon);
    ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_SetItem, battleSlot, itemID);
    BattleMon_SetItem(battleMon, itemID);
    if (itemID) {
        ItemEvent_AddItem(battleMon);
    }

    HEID = HEManager_PushState(&serverFlow->HEManager);
    ServerEvent_ItemRewriteDone(serverFlow, battleMon);
    HEManager_PopState(&serverFlow->HEManager, HEID);

    if (consumeItem) {
        BattleMon_ConsumeItem(battleMon, usedItem);
        ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_ConsumeItem, battleSlot, usedItem);
        ServerDisplay_SetTurnFlag(serverFlow, battleMon, TURNFLAG_ITEMCONSUMED);

        HEID = HEManager_PushState(&serverFlow->HEManager);
        ServerEvent_ConsumeItem(serverFlow, battleSlot, usedItem); // Added consume item event
        HEManager_PopState(&serverFlow->HEManager, HEID);

#if EXPAND_MOVES
        if (consumeItem != 2 && PML_ItemIsBerry(itemID)) {
            BattleField_SetConsumedBerryFlag(battleSlot);
        }
#endif
    }
}

extern "C" void THUMB_BRANCH_ServerEvent_EquipItem(ServerFlow* serverFlow, BattleMon* currentMon) {
    ITEM itemID = BattleMon_GetHeldItem(currentMon);

    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(currentMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_CallHandlers(serverFlow, EVENT_USE_ITEM);
    BattleEventVar_Pop();

    // Call the use item after event after using the item
    ServerEvent_UseItemAfter(serverFlow, currentSlot, itemID);
}
// Cheek Pouch - Add event right after equiping a temporary item
// Belch - Records if a berry has been consumed as a temporary item
extern "C" void THUMB_BRANCH_ServerEvent_EquipTempItem(ServerFlow* serverFlow, BattleMon* currentMon, u32 attackingSlot)
{
    ITEM itemID = g_BattleField->tempItem;
    g_BattleField->tempItem = ITEM_NULL;

    u32 currentSlot = BattleMon_GetID(currentMon);

    // Call the use item event before using the item
    ServerEvent_UseTempItemBefore(serverFlow, currentSlot, attackingSlot, itemID);

    BattleEventVar_Push();
    BattleEventVar_SetValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetValue(VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetValue(VAR_ITEM, itemID);
    BattleEvent_ForceCallHandlers(serverFlow, EVENT_USE_ITEM_TEMP);
    BattleEventVar_Pop();

    // Call the use item event after using the item
    ServerEvent_UseTempItemAfter(serverFlow, currentSlot, attackingSlot, itemID);
}

extern "C" EFFECTIVENESS THUMB_BRANCH_SAFESTACK_ServerEvent_CheckDamageEffectiveness(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, PokeType moveType, u8 pokemonType) {
    BattleEventVar_Push();
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    u32 defendingSlot = BattleMon_GetID(defendingMon);
    BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot);
    BattleEventVar_SetConstValue(VAR_POKE_TYPE, pokemonType);
    BattleEventVar_SetConstValue(VAR_MOVE_TYPE, moveType);
    BattleEventVar_SetRewriteOnceValue(VAR_NO_TYPE_EFFECTIVENESS, 0);
    BattleEventVar_SetRewriteOnceValue(VAR_SET_TYPE_EFFECTIVENESS, DONT_OVERRIDE_EFFECTIVENESS);
    Condition_CheckUnaffectedByType(serverFlow, defendingMon);
    BattleEvent_CallHandlers(serverFlow, EVENT_CHECK_TYPE_EFFECTIVENESS);
    b32 overrideImmunity = BattleEventVar_GetValue(VAR_NO_TYPE_EFFECTIVENESS);
    u32 overrideEffectiveness = BattleEventVar_GetValue(VAR_SET_TYPE_EFFECTIVENESS);
    BattleEventVar_Pop();
        
    EFFECTIVENESS effectiveness = GetTypeEffectiveness(moveType, pokemonType);
    if (effectiveness == EFFECTIVENESS_IMMUNE &&
        overrideImmunity) {
        effectiveness = EFFECTIVENESS_1;
    }

    if (effectiveness != EFFECTIVENESS_IMMUNE) {
        switch (overrideEffectiveness) {
        case DONT_OVERRIDE_EFFECTIVENESS:
            break;
        case OVERRIDE_EFFECTIVENESS_1_2:
            return EFFECTIVENESS_1_2;
        case OVERRIDE_EFFECTIVENESS_1:
            return EFFECTIVENESS_1;
        case OVERRIDE_EFFECTIVENESS_2:
            return EFFECTIVENESS_2;
        default: // COMPOUND_EFFECTIVENESS + (Effectiveness to compound)
            effectiveness = GetTypeEffectivenessMultiplier(effectiveness, (overrideEffectiveness - COMPOUND_EFFECTIVENESS));
            break;
        }
    }

    return effectiveness;
}

// Toxic Debris & Ceaseless Edge - Added toxic spikes animation when activated
extern "C" b32 SideEvent_AddItem(u32 side, SIDE_EFFECT sideEffect, ConditionData condData);
extern "C" b32 THUMB_BRANCH_BattleHandler_AddSideEffect(ServerFlow * serverFlow, HandlerParam_AddSideEffect * params) {
    u8 side = params->side;
    ConditionData condData = params->condData;
    u32 pokemonSlot = params->header.flags << 19 >> 27;

    ServerEvent_CheckSideEffectParam(serverFlow, pokemonSlot, params->sideEffect, side, &condData);
    if (!SideEvent_AddItem(params->side, params->sideEffect, condData)) {
        return 0;
    }
    BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, params->header.flags << 19 >> 27);
    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupAdd(serverFlow, battleMon);
    }

    // If the side effect is triggered by secondary effects
    if (((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) ||
        ((params->header.flags & SIDE_HANDLER_MOVE_POPUP_FLAG) != 0)) {
        u32 pokePos = MainModule_PokeIDToPokePos(serverFlow->mainModule, serverFlow->pokeCon, pokemonSlot);
        switch (params->sideEffect) {
        case SIDEEFF_SPIKES:
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_MoveAnim, pokePos, 0, MOVE_SPIKES, 0, 0);
            break;
        case SIDEEFF_TOXIC_SPIKES:
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_MoveAnim, pokePos, 0, MOVE_TOXIC_SPIKES, 0, 0);
            break;
        case SIDEEFF_STEALTH_ROCK:
            ServerDisplay_AddCommon(serverFlow->serverCommandQueue, SCID_MoveAnim, pokePos, 0, MOVE_STEALTH_ROCK, 0, 0);
            break;
        }
    }

    BattleHandler_SetString(serverFlow, &params->exStr);
    if ((params->header.flags & HANDLER_ABILITY_POPUP_FLAG) != 0) {
        ServerDisplay_AbilityPopupRemove(serverFlow, battleMon);
    }
    return 1;
}

// Liquid Voice - Added move ID to check for move flags
// Ion Deluge (and any other move type changing move) - Made so the move type can be changed more than once
extern "C" void THUMB_BRANCH_ServerEvent_GetMoveParam(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* battleMon, MoveParam* moveParam) {
    BattleEventVar_Push();

    u32 currentSlot = BattleMon_GetID(battleMon);
    POKE_TYPE moveType = PML_MoveGetType(moveID);
    POKE_TYPE pokeType = BattleMon_GetPokeType(battleMon);
    MoveCategory category = PML_MoveGetCategory(moveID);
    u32 targetType = PML_MoveGetParam(moveID, MVDATA_TARGET);

    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    // Added move ID
    BattleEventVar_SetConstValue(VAR_MOVE_ID, moveID);
#if EXPAND_MOVES
    BattleEventVar_SetValue(VAR_MOVE_TYPE, moveType);
#else
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_TYPE, moveType);
#endif
    BattleEventVar_SetValue(VAR_USER_TYPE, pokeType);
    BattleEventVar_SetValue(VAR_MOVE_CATEGORY, category);
    BattleEventVar_SetValue(VAR_TARGET_TYPE, targetType);
    BattleEventVar_SetRewriteOnceValue(VAR_NO_TYPE_EFFECTIVENESS, 0);

    BattleEvent_CallHandlers(serverFlow, EVENT_MOVE_PARAM);

    moveParam->moveID = moveID;
    moveParam->originalMoveID = moveID;
    moveParam->moveType = BattleEventVar_GetValue(VAR_MOVE_TYPE);
    moveParam->userType = BattleEventVar_GetValue(VAR_USER_TYPE);
    moveParam->category = BattleEventVar_GetValue(VAR_MOVE_CATEGORY);
    moveParam->targetType = BattleEventVar_GetValue(VAR_TARGET_TYPE);
    moveParam->flags = 0;
    if (BattleEventVar_GetValue(VAR_NO_TYPE_EFFECTIVENESS)) {
        moveParam->moveType = TYPE_NULL;
    }

    BattleEventVar_Pop();
}

#endif // EXPAND_ABILITIES || EXPAND_MOVES

#if EXPAND_ITEMS

// Event called when a move misses due to accuracy
// [ServerFlow_CheckMoveAvoid]
extern "C" void ServerEvent_AccuracyMiss(ServerFlow* serverFlow, BattleMon* attackingMon) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    u32 attackingSlot = BattleMon_GetID(attackingMon);
    BattleEventVar_SetConstValue(NEW_VAR_ATTACKING_MON, attackingSlot);
    BattleEvent_CallHandlers(serverFlow, EVENT_ACCURACY_MISS);
    BattleEventVar_Pop();
}
// Called when before an ability change, it allows cancelling the change 
// [BattleHandler_ChangeAbility] [ServerDisplay_SkillSwap] [HandlerMoldBreakerSkipCheck]
extern "C" u8 ServerEvent_ChangeAbilityCheckFail(ServerFlow * serverFlow, u32 pokemonSlot) {
    BattleEventVar_Push();
    SET_UP_NEW_EVENT;
    BattleEventVar_SetConstValue(NEW_VAR_MON_ID, pokemonSlot);
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_ABILITY_CHANGE_CHECK_FAIL);
    u8 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();

    return failFlag;
}

// PROTECTIVE PADS
// Implemented adding checks to contact situations, mostly ability handlers
// [MakesContact]

// HEAVY-DUTY BOOTS
// Implemented by adding checks to the entry hazards handlers

struct ItemEventAddTableExt
{
    ITEM itemID;
    ItemEventAddFunc func;
    const char* dllName;
};
ItemEventAddTableExt itemEventAddTableExt[] = {
    { ITEM_WEAKNESS_POLICY, EventAddWeaknessPolicy, "Items/WeaknessPolicy" },
    { ITEM_ASSAULT_VEST, EventAddAssaultVest, "Items/AssaultVest" },
    { ITEM_PIXIE_PLATE, EventAddFairyFeather, "Items/FairyFeather" },
    { ITEM_LUMINOUS_MOSS, EventAddLuminousMoss, "Items/LuminousMoss" },
    { ITEM_SNOWBALL, EventAddSnowball, "Items/Snowball" },
    { ITEM_SAFETY_GOGGLES, EventAddSafetyGoggles, "Items/SafetyGoggles" },
    { ITEM_ROSELI_BERRY, EventAddRoseliBerry , "Items/RoseliBerry" },
    { ITEM_KEE_BERRY, EventAddKeeBerry, "Items/KeeBerry" },
    { ITEM_MARANGA_BERRY, EventAddMarangaBerry, "Items/MarangaBerry" },
    { ITEM_FAIRY_GEM, EventAddFairyGem, "Items/FairyGem" },
    { ITEM_ADRENALINE_ORB, EventAddAdrenalineOrb, "Items/AdrenalineOrb" },
    { ITEM_THROAT_SPRAY, EventAddThroatSpray, "Items/ThroatSpray" },
    { ITEM_EJECT_PACK, EventAddEjectPack, "Items/EjectPack" },
    { ITEM_BLUNDER_POLICY, EventAddBlunderPolicy, "Items/BlunderPolicy" },
    { ITEM_ROOM_SERVICE, EventAddRoomSevice, "Items/RoomSevice" },
    { ITEM_UTILITY_UMBRELLA, EventAddUtilityUmbrella, "Items/UtilityUmbrella" },
    { ITEM_ABILITY_SHIELD, EventAddAbilityShield, "Items/AbilityShield" },
    { ITEM_CLEAR_AMULET, EventAddClearAmulet, "Items/ClearAmulet" },
    { ITEM_LOADED_DICE, EventAddLoadedDice, "Items/LoadedDice" },
    { ITEM_MIRROR_HERB, EventAddMirrorHerb, "Items/MirrorHerb" },
    { ITEM_PUNCHING_GLOVE, EventAddIronFist, nullptr },
    { ITEM_COVERT_CLOAK, EventAddCovertCloak, "Items/CovertCloak" },
    { ITEM_FAIRY_FEATHER, EventAddFairyFeather, "Items/FairyFeather" },
    { ITEM_TERRAIN_EXTENDER, EventAddTerrainExtender, "Items/TerrainExtender" },
    { ITEM_ELECTRIC_SEED, EventAddElectricSeed, "Items/TerrainSeeds" },
    { ITEM_GRASSY_SEED, EventAddGrassySeed, "Items/TerrainSeeds" },
    { ITEM_MISTY_SEED, EventAddMistySeed, "Items/TerrainSeeds" },
    { ITEM_PSYCHIC_SEED, EventAddPsychicSeed, "Items/TerrainSeeds" },
};

extern "C" BattleEventItem* GetItemEvent(BattleMon* battleMon, ITEM itemID, ItemEventAddFunc func) {
    u32 speed = BattleMon_GetRealStat(battleMon, VALUE_SPEED_STAT);
    u32 battleSlot = BattleMon_GetID(battleMon);

    u32 handlerAmount = 0;
    BattleEventHandlerTableEntry* handlers = func(&handlerAmount);
    return BattleEvent_AddItem(EVENTITEM_ITEM, itemID, EVENTPRI_ITEM_DEFAULT, speed, battleSlot, handlers, handlerAmount);
}

extern "C" BattleEventItem* THUMB_BRANCH_ItemEvent_AddItemCore(BattleMon * battleMon, ITEM itemID) {
    // Check for new or overriden items
    for (u32 itemEvent = 0; itemEvent < ARRAY_COUNT(itemEventAddTableExt); ++itemEvent) {

        ItemEventAddTableExt* itemAddEvent = &(itemEventAddTableExt[itemEvent]);
        if (itemID == itemAddEvent->itemID) {
            // Load the DLL with the code for the ability if needed
            if (!LoadDll(itemAddEvent->dllName))
                return 0;

            return GetItemEvent(battleMon, itemID, itemAddEvent->func);
        }
    }
    // Check for vanilla items
    ItemEventAddTable* itemEventAddTable = (ItemEventAddTable*)0x21D8F68;
    for (u32 itemEvent = 0; itemEvent < 172; ++itemEvent) {

        ItemEventAddTable* itemAddEvent = &(itemEventAddTable[itemEvent]);
        if (itemID == itemAddEvent->itemID) {
            return GetItemEvent(battleMon, itemID, itemAddEvent->func);
        }
    }
    return 0;
}

// Heavy-Duty Boots - Disable entry hazards effects
extern "C" b32 CheckDisableEntryHazards(BattleMon* battleMon) {
    if (battleMon &&
        BattleMon_GetUsableItem(battleMon) == ITEM_HEAVY_DUTY_BOOTS) {
        return 1;
    }
    return 0;
}

// Blunder Policy - Add event after accuracy miss
extern "C" void THUMB_BRANCH_SAFESTACK_ServerFlow_CheckMoveAvoid(ServerFlow* serverFlow, MoveParam* moveParam, BattleMon* attackingMon, PokeSet* targetSet) {
    if (PokeSet_GetCountMax(targetSet) != 1 || PokeSet_Get(targetSet, 0) != attackingMon) {
        
        PokeSet_SeekStart(targetSet);
        for (BattleMon* targetMon = PokeSet_SeekNext(targetSet); targetMon; targetMon = PokeSet_SeekNext(targetSet)) {
            if (!ServerEvent_SkipAvoidCheck(serverFlow, attackingMon, targetMon, moveParam)
                && !ServerEvent_CheckHit(serverFlow, attackingMon, targetMon, moveParam)) {

                PokeSet_Remove(targetSet, targetMon);
                ServerDisplay_MoveAvoid(serverFlow, targetMon);

                ServerEvent_AccuracyMiss(serverFlow, attackingMon);
            }
        }
    }
}

// Utility Umbrella - Disable weather related abilities and moves on the holder
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerChlorophyll_0x14(ServerFlow * serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), ServerEvent_GetWeather(serverFlow));
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerSwiftSwim_0x14(ServerFlow * serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), ServerEvent_GetWeather(serverFlow));
}

// WARNING HandlerDrySkinWeather is used in BattleUpgrade.S
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerDrySkinWeather_0x1C(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), BattleEventVar_GetValue(VAR_WEATHER));
}

extern "C" void THUMB_BRANCH_CommonFlowerGiftFormChange(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32 newForm, u8 flag) {
    BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
    if (BattleMon_GetUsableItem(currentMon) == ITEM_UTILITY_UMBRELLA) {
        newForm = 0;
    }
    if (newForm != BattleMon_GetValue(currentMon, VALUE_FORM)) {
        HandlerParam_ChangeForm* changeForm;
        changeForm = (HandlerParam_ChangeForm*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_FORM, pokemonSlot);
        changeForm->pokeID = pokemonSlot;
        changeForm->newForm = newForm;
        changeForm->header.flags = changeForm->header.flags & 0xFF7FFFFF | (flag << 23) & 0xFFFFFF;
        BattleHandler_StrSetup(&changeForm->exStr, 2u, 222u);
        BattleHandler_AddArg(&changeForm->exStr, pokemonSlot);
        BattleHandler_PopWork(serverFlow, changeForm);
    }
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerFlowerGiftPower_0x14(ServerFlow* serverFlow, u32 pokemonSlot) {
    return Handler_CheckWeather(serverFlow, pokemonSlot, ServerEvent_GetWeather(serverFlow));
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerFlowerGiftSpecialDefense_0x14(ServerFlow* serverFlow, u32 pokemonSlot) {
    return Handler_CheckWeather(serverFlow, pokemonSlot, ServerEvent_GetWeather(serverFlow));
}

extern "C" u32 THUMB_BRANCH_LINK_HandlerForecastSwitchIn_0xA(ServerFlow* serverFlow, u32 nullVar, u32 pokemonSlot) {
    return Handler_CheckWeather(serverFlow, pokemonSlot, ServerEvent_GetWeather(serverFlow));
}
extern "C" u32 THUMB_BRANCH_LINK_HandlerForecastWeather_0xE(ServerFlow* serverFlow, u32 nullVar, u32 pokemonSlot) {
    return Handler_CheckWeather(serverFlow, pokemonSlot, ServerEvent_GetWeather(serverFlow));
}

extern "C" WEATHER THUMB_BRANCH_LINK_HandlerHydration_0x14(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), ServerEvent_GetWeather(serverFlow));
}

extern "C" WEATHER THUMB_BRANCH_LINK_HandlerLeafGuard_0x14(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON), ServerEvent_GetWeather(serverFlow));
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerLeafGuardYawnCheck_0x12(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), ServerEvent_GetWeather(serverFlow));
}

extern "C" void THUMB_BRANCH_HandlerRainDish(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (Handler_CheckWeather(serverFlow, pokemonSlot, BattleEventVar_GetValue(VAR_WEATHER)) == WEATHER_RAIN) {
        CommonWeatherRecoveryAbility(serverFlow, pokemonSlot, WEATHER_RAIN);
    }
}

// WARNING HandlerSolarPowerWeather is used in BattleUpgrade.S
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerSolarPowerWeather_0x14(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), BattleEventVar_GetValue(VAR_WEATHER));
}
// WARNING HandlerSolarPowerPower is used in BattleUpgrade.S
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerSolarPowerPower_0x14(u32 pokemonSlot, ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, pokemonSlot, ServerEvent_GetWeather(serverFlow));
}

extern "C" WEATHER THUMB_BRANCH_LINK_HandlerMorningSun_0x14(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), ServerEvent_GetWeather(serverFlow));
}

extern "C" b32 PML_MoveIsAlwaysHit(MOVE_ID moveID);
extern "C" b32 THUMB_BRANCH_ServerEvent_BypassAccuracyCheck(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MOVE_ID moveID) {
    b32 isAlwaysHit = PML_MoveIsAlwaysHit(moveID);
    if (!isAlwaysHit) {
        BattleEventVar_Push();
        u32 attackingSlot = BattleMon_GetID(attackingMon);
        BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
        // Added the defending slot to the handler to allow
        // thunder & hurricane accuracy with Utility Umbrella
        u32 defendingSlot = BattleMon_GetID(defendingMon);
        BattleEventVar_SetConstValue(VAR_DEFENDING_MON, defendingSlot); 
        BattleEventVar_SetRewriteOnceValue(VAR_GENERAL_USE_FLAG, 0);
        BattleEvent_CallHandlers(serverFlow, EVENT_BYPASS_ACCURACY_CHECK);
        isAlwaysHit = BattleEventVar_GetValue(VAR_GENERAL_USE_FLAG);
        BattleEventVar_Pop();
    }
    return isAlwaysHit;
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerThunderRainCheck_0x12(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON), ServerEvent_GetWeather(serverFlow));
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerThunderSunCheck_0x12(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_DEFENDING_MON), ServerEvent_GetWeather(serverFlow));
}

extern "C" WEATHER THUMB_BRANCH_LINK_HandlerSolarBeamSunCheck_0x12(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON), ServerEvent_GetWeather(serverFlow));
}

extern "C" WEATHER THUMB_BRANCH_LINK_HandlerGrowth_0x12(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON), ServerEvent_GetWeather(serverFlow));
}

extern "C" WEATHER THUMB_BRANCH_LINK_HandlerWeatherBallType_0x14(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_MON_ID), ServerEvent_GetWeather(serverFlow));
}
extern "C" WEATHER THUMB_BRANCH_LINK_HandlerWeatherBallPower_0x12(ServerFlow* serverFlow) {
    return Handler_CheckWeather(serverFlow, BattleEventVar_GetValue(VAR_ATTACKING_MON), ServerEvent_GetWeather(serverFlow));
}

// Ability Shield - Added events for stopping ability modifications

extern "C" u32 AbilityChangeBlockCheck(ServerFlow * serverFlow, BattleMon * battleMon) {
    u32 HEID = HEManager_PushState(&serverFlow->HEManager);
    if (ServerEvent_ChangeAbilityCheckFail(serverFlow, battleMon->battleSlot))
        return ABIL_MULTITYPE;
    HEManager_PopState(&serverFlow->HEManager, HEID);
    return BattleMon_GetValue(battleMon, VALUE_ABILITY);
}

// WARNING BattleHandler_ChangeAbility is used in BattleUpgrade.S
extern "C" u32 THUMB_BRANCH_LINK_BattleHandler_ChangeAbility_0x14(BattleMon* battleMon, ServerFlow* serverFlow) {
    return AbilityChangeBlockCheck(serverFlow, battleMon);
}

// WARNING ServerDisplay_SkillSwap is used in BattleUpgrade.S
extern "C" u32 THUMB_BRANCH_LINK_ServerDisplay_SkillSwap_0x16(BattleMon* battleMon, ServerFlow* serverFlow) {
    return AbilityChangeBlockCheck(serverFlow, battleMon);
}
extern "C" u32 THUMB_BRANCH_LINK_ServerDisplay_SkillSwap_0x20(BattleMon* battleMon, ServerFlow* serverFlow) {
    return AbilityChangeBlockCheck(serverFlow, battleMon);
}

extern "C" b32 IsMoldBreakerAffectedAbility(ABILITY ability);
extern "C" b32 THUMB_BRANCH_SAFESTACK_HandlerMoldBreakerSkipCheck(BattleEventItem* item, ServerFlow* serverFlow, u32 factorType, u32 eventType, u32 subID) {
    if (factorType == 4 && IsMoldBreakerAffectedAbility(subID)) {
        u32 defendingSlot = serverFlow->setTarget->getIdx;
        if (defendingSlot < BATTLE_MAX_SLOTS && 
            ServerEvent_ChangeAbilityCheckFail(serverFlow, defendingSlot)) {
            return 0;
        }
        return 1;
    }
    return 0;
}

// Loaded Dice - Made avoid flag rewritable more than once
extern "C" void THUMB_BRANCH_LINK_ServerEvent_CheckMultihitHits_0x5A() {
    BattleEventVar_SetValue(VAR_AVOID_FLAG, 0);
}

#endif // EXPAND_ITEMS

#if EXPAND_ITEMS || EXPAND_MOVES

// Assault Vest - Blocks the use of Status moves
// Belch - Blocks its use if a berry has not been consumed
extern "C" b32 THUMB_BRANCH_SAFESTACK_IsUnselectableMove(BtlClientWk* client, BattleMon* battleMon, MOVE_ID moveID, Btlv_StringParam* strparam) {
    if (moveID != MOVE_STRUGGLE) {

        if (BattleMon_GetUsableItem(battleMon) && BattleMon_CheckIfMoveCondition(battleMon, CONDITION_CHOICELOCK)) {
            ConditionData choiceLock = BattleMon_GetMoveCondition(battleMon, CONDITION_CHOICELOCK);
            MOVE_ID choicedMove = Condition_GetParam(choiceLock);
            if (Move_IsUsable(battleMon, choicedMove) && choicedMove != moveID) {
                if (strparam) {
                    Btlv_StringParam_Setup(strparam, 1, 99);
                    ITEM heldItem = BattleMon_GetHeldItem(battleMon);
                    Btlv_StringParam_AddArg(strparam, heldItem);
                    Btlv_StringParam_AddArg(strparam, choicedMove);
                }
                return 1;
            }
        }

        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_ENCORE)) {
            ConditionData encore = BattleMon_GetMoveCondition(battleMon, CONDITION_ENCORE);
            MOVE_ID previousMove = Condition_GetParam(encore);
            if (moveID != previousMove) {
                if (strparam) {
                    Btlv_StringParam_Setup(strparam, 1, 100);
                    u32 battleSlot = BattleMon_GetID(battleMon);
                    Btlv_StringParam_AddArg(strparam, battleSlot);
                    Btlv_StringParam_AddArg(strparam, previousMove);
                }
                return 1;
            }
        }

        if (!PML_MoveIsDamaging(moveID)) {
#if EXPAND_ITEMS
            ITEM heldItem = BattleMon_GetHeldItem(battleMon);
            if (heldItem == ITEM_ASSAULT_VEST) {
                if (strparam) {
                    Btlv_StringParam_Setup(strparam, 1, BATTLE_ASSAULTVEST_MSGID);
                    u32 battleSlot = BattleMon_GetID(battleMon);
                    Btlv_StringParam_AddArg(strparam, battleSlot);
                    Btlv_StringParam_AddArg(strparam, heldItem);
                }
                return 1;
            }
#endif
            if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_TAUNT)) {
                if (strparam) {
                    Btlv_StringParam_Setup(strparam, 2, 571);
                    u32 battleSlot = BattleMon_GetID(battleMon);
                    Btlv_StringParam_AddArg(strparam, battleSlot);
                }
                return 1;
            }
        }

        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_TORMENT) && moveID == BattleMon_GetPreviousMove(battleMon)) {
            if (strparam) {
                Btlv_StringParam_Setup(strparam, 2, 580);
                u32 battleSlot = BattleMon_GetID(battleMon);
                Btlv_StringParam_AddArg(strparam, battleSlot);
                Btlv_StringParam_AddArg(strparam, moveID);
            }
            return 1;
        }

        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_DISABLE)
            && moveID == BattleMon_GetConditionAffectedMove(battleMon, CONDITION_DISABLE)
            && moveID != MOVE_STRUGGLE) {
            if (strparam) {
                Btlv_StringParam_Setup(strparam, 2, 595);
                u32 battleSlot = BattleMon_GetID(battleMon);
                Btlv_StringParam_AddArg(strparam, battleSlot);
                Btlv_StringParam_AddArg(strparam, moveID);
            }
            return 1;
        }

        if (BattleMon_CheckIfMoveCondition(battleMon, CONDITION_HEALBLOCK) && 
            getMoveFlag(moveID, FLAG_HEALING)) {
            if (strparam) {
                Btlv_StringParam_Setup(strparam, 2, 890);
                u32 battleSlot = BattleMon_GetID(battleMon);
                Btlv_StringParam_AddArg(strparam, battleSlot);
                Btlv_StringParam_AddArg(strparam, moveID);
            }
            return 1;
        }

        if (BattleField_CheckEffect(FLDEFF_IMPRISON) && 
            BattleField_CheckImprison(client->pokeCon, battleMon, moveID)) {
            if (strparam) {
                Btlv_StringParam_Setup(strparam, 2, 589);
                u32 battleSlot = BattleMon_GetID(battleMon);
                Btlv_StringParam_AddArg(strparam, battleSlot);
                Btlv_StringParam_AddArg(strparam, moveID);
            }
            return 1;
        }

        if (BattleField_CheckEffect(FLDEFF_GRAVITY) &&
            getMoveFlag(moveID, FLAG_GROUNDED_BY_GRAVITY)) {
            if (strparam) {
                Btlv_StringParam_Setup(strparam, 2, 1086);
                u32 battleSlot = BattleMon_GetID(battleMon);
                Btlv_StringParam_AddArg(strparam, battleSlot);
                Btlv_StringParam_AddArg(strparam, moveID);
            }
            return 1;
        }
#if EXPAND_MOVES
        if (moveID == MOVE_BELCH) {
            u32 battleSlot = BattleMon_GetID(battleMon);
            if (!BattleField_CheckConsumedBerryFlag(battleSlot)) {
                if (strparam) {
                    Btlv_StringParam_Setup(strparam, 1, BATTLE_BELCH_MSGID);
                    Btlv_StringParam_AddArg(strparam, battleSlot);
                }
                return 1;
            }
        }
#endif
    }
    return 0;
}

#endif // EXPAND_ITEMS || EXPAND_MOVES

#if ADD_MEGA_EVOLUTION || (EXPAND_ABILITIES && EXPAND_ITEMS)

#if ADD_MEGA_EVOLUTION
extern "C" ITEM GetMegaStone(SPECIES species) {
    for (u32 idx = 0; idx < ARRAY_COUNT(megaevolutions); ++idx)
    {
        MegaEvolution* mega = &(megaevolutions[idx]);
        if (mega->species == species)
            return mega->item;
    }
    return ITEM_NULL;
}
#endif

extern "C" b32 Tables_IsArceusPlate(ITEM itemID);
extern "C" b32 Tables_IsGenesectDrive(ITEM itemID);

// WARNING HandlerTrick is used in BattleUpgrade.S
// WARNING HandlerBestow is used in BattleUpgrade.S
// WARNING HandlerCommon_CheckUnchangeableItem is used in BattleUpgrade.S
// WARNING HandlerCommon_CheckIfCanStealPokeItem is used in BattleUpgrade.S
extern "C" b32 THUMB_BRANCH_HandlerCommon_IsUnremovableItem(BattleMon* battleMon, ITEM itemID) {
    if (battleMon->species == PKM_GIRATINA && itemID == ITEM_GRISEOUS_ORB) {
        return 1;
    }

    if (battleMon->species == PKM_ARCEUS && Tables_IsArceusPlate(itemID)) {
        return 1;
    }

    if (battleMon->species == PKM_GENESECT && Tables_IsGenesectDrive(itemID)) {
        return 1;
    }

#if ADD_MEGA_EVOLUTION
    ITEM megaStone = GetMegaStone(battleMon->species);
    if (megaStone != ITEM_NULL && megaStone == itemID) {
        return 1;
    }
#endif

#if EXPAND_ABILITIES && EXPAND_ITEMS
    if ((battleMon->ability == ABIL_PROTOSYNTHESIS || battleMon->ability == ABIL_QUARK_DRIVE) && 
        itemID == ITEM_BOOSTER_ENERGY) {
        return 1;
    }
#endif

    return 0;
}

#endif // ADD_MEGA_EVOLUTION || (EXPAND_ABILITIES && EXPAND_ITEMS)

#if EXPAND_MOVES

extern "C" void GetSideBattleSlots(ServerFlow* serverFlow, u32 side, u8 battleSlots[3]) {
    for (u32 allyIdx = 0; allyIdx < 3; ++allyIdx)
        battleSlots[allyIdx] = BATTLE_MAX_SLOTS;

    u32 count = 0;
    for (u8 i = 0; i < 24; ++i) {
        BattleMon* battleMon = PokeCon_GetBattleMon(serverFlow->pokeCon, i);
        if (battleMon) {

            u32 battleSlot = BattleMon_GetID(battleMon);
            if (side == GetSideFromMonID(battleSlot)) {
                battleSlots[count] = battleSlot;
                ++count;
            }
        }
    }
}

struct MoveEventAddTableExt
{
    MOVE_ID moveID;
    MoveEventAddFunc func;
    const char* dllName;
};
MoveEventAddTableExt moveEventAddTableExt[] = {
    {MOVE_RAPID_SPIN, EventAddRapidSpinUpdated, "Moves/RapidSpin"},
    {MOVE_DEFOG, EventAddDefogUpdated, "Moves/Defog"},
    {MOVE_FLYING_PRESS, EventAddFlyingPress, "Moves/FlyingPress"},
    {MOVE_MAT_BLOCK, EventAddMatBlock, "Moves/MatBlock"},
    {MOVE_ROTOTILLER, EventAddRototiller, "Moves/Rototiller"},
    {MOVE_STICKY_WEB, EventAddStickyWeb, "Moves/StickyWeb"},
    {MOVE_FELL_STINGER, EventAddFellStinger, "Moves/FellStinger"},
    {MOVE_PHANTOM_FORCE, EventAddShadowForce, nullptr},
    {MOVE_TRICK_OR_TREAT, EventAddTrickOrTreat, "Moves/ExtraType"},
    {MOVE_ION_DELUGE, EventAddIonDeluge, "Moves/IonDeluge"},
    {MOVE_FOREST_S_CURSE, EventAddForestsCurse, "Moves/ExtraType"},
    {MOVE_FREEZE_DRY, EventAddFreezeDry, "Moves/FreezeDry"},
    {MOVE_PARTING_SHOT, EventAddPartingShot, "Moves/PartingShot"},

    {MOVE_PLASMA_FIST, EventAddPlasmaFist, "Moves/IonDeluge"},

    {MOVE_INFERNAL_PARADE, EventAddHex, nullptr},
    {MOVE_CEASELESS_EDGE, EventAddCeaselessEdge, "Moves/CeaselessEdge"},
};

struct PosEffectEventAddTableExt
{
    POS_EFFECT posEffect;
    PosEffectEventAddFunc func;
    const char* dllName;
};
PosEffectEventAddTableExt posEffectEventAddTableExt[] = {
    {POSEFF_ION_DELUGE, EventAddPosIonDeluge, "Moves/IonDeluge"},
};

extern "C" void CMD_ACT_MoveAnimStart(BtlvScu * btlvScu, u32 attckViewPos, u32 defViewPos, u16 moveID, u32 moveTarget, u8 effectIndex, u8 zero);
extern "C" void THUMB_BRANCH_SAFESTACK_BattleViewCmd_MoveEffect_Start(BtlvCore * btlCore, u32 attackingPos, u32 targetPos, u16 moveID, u32 moveTarget, u32 effectIndex, u8 zero) {
    u32 attckViewPos = MainModule_BattlePosToViewPos(btlCore->mainModule, attackingPos);

    u32 defViewPos;
    if (targetPos == NULL_BATTLE_POS) {
        defViewPos = 255;
    }
    else {
        defViewPos = MainModule_BattlePosToViewPos(btlCore->mainModule, targetPos);
    }
    if (moveID >= FIRST_BATTLE_ANIMATION_ID)
        moveID += BATTLE_ANIMATIONS_COUNT;
    CMD_ACT_MoveAnimStart(btlCore->btlvScu, attckViewPos, defViewPos, moveID, moveTarget, effectIndex, zero);
}

extern "C" bool GetMoveEvent(BattleMon * battleMon, MOVE_ID moveID, u32 speed, MoveEventAddFunc func) {
    u32 battleSlot = BattleMon_GetID(battleMon);
    u8 alreadyRegistered = 0;
    if (!MoveEvent_CanEffectBeRegistered(battleSlot, moveID, &alreadyRegistered)) {
        return false;
    }

    u32 handlerAmount = 0;
    BattleEventHandlerTableEntry* handlers = func(&handlerAmount);
    if (handlers == nullptr) {
        return false;
    }

    BattleEvent_AddItem(EVENTITEM_MOVE, moveID, EVENTPRI_MOVE_DEFAULT, speed, battleSlot, handlers, handlerAmount);
    return true;
}

extern "C" bool THUMB_BRANCH_MoveEvent_AddItem(BattleMon * battleMon, MoveID moveID, u32 speed) {
    // Check for new or overriden moves
    for (u32 moveEvent = 0; moveEvent < ARRAY_COUNT(moveEventAddTableExt); ++moveEvent) {

        MoveEventAddTableExt* moveAddEvent = &(moveEventAddTableExt[moveEvent]);
        if (moveID == moveAddEvent->moveID) {
            // Load the DLL with the code for the move if needed
            if (!LoadDll(moveAddEvent->dllName))
                return 0;

            return GetMoveEvent(battleMon, moveID, speed, moveAddEvent->func);
        }
    }
    
    // Check for vanilla moves
    MoveEventAddTable* moveEventAddTable = (MoveEventAddTable*)0x21DA0F4;
    for (u32 moveEvent = 0; moveEvent < 258; ++moveEvent) {

        MoveEventAddTable* moveAddEvent = &(moveEventAddTable[moveEvent]);
        if (moveID == moveAddEvent->moveID) {
            return GetMoveEvent(battleMon, moveID, speed, moveAddEvent->func);
        }
    }
    return 0;
}

extern "C" BattleEventPriority PosEffect_GetPriority(u32* handlerAmount) {

    u16 eventPriority = ((*handlerAmount & 0xFFFF0000) >> 16);
    if (eventPriority != 0) {
        *handlerAmount &= 0x0000FFFF;
        return (BattleEventPriority)eventPriority;
    }
    return EVENTPRI_POS_DEFAULT;
}
extern "C" u32 PosEffect_SetPriority(BattleEventPriority priority, u32 handlerCount) {
    u32 value = handlerCount & 0x0000FFFF;
    value |= (priority & 0x0000FFFF) << 16;
    return value;
}

extern "C" BattleEventItem* GetPosEffectEvent(POS_EFFECT posEffect, u32 targetPos, PosEffectEventAddFunc func, u8 workCount, u32* work) {
    if (!PosEffectEvent_CanEffectBeRegistered(posEffect, targetPos)) {
        return nullptr;
    }

    u32 handlerAmount = 0;
    BattleEventHandlerTableEntry* handlers = func(&handlerAmount);
    if (handlers == nullptr) {
        return nullptr;
    }

    BattleEventPriority mainPrio = PosEffect_GetPriority(&handlerAmount);
    BattleEventItem* battleEvent = BattleEvent_AddItem(EVENTITEM_POS, posEffect, mainPrio, 0, targetPos, handlers, handlerAmount);
    if (battleEvent) {
        for (u32 workIdx = 0; workIdx < workCount; ++workIdx) {
            BattleEventItem_SetWorkValue(battleEvent, workIdx, work[workIdx]);
        }
    }

    return battleEvent;
}

extern "C" BattleEventItem* THUMB_BRANCH_SAFESTACK_PosEffectEvent_AddItem(POS_EFFECT posEffect, u32 targetPos, u32 pokemonSlot, u32* work, u8 workCount) {
    // Check for new or overriden pos effects
    for (u32 posEvent = 0; posEvent < ARRAY_COUNT(posEffectEventAddTableExt); ++posEvent) {

        PosEffectEventAddTableExt* posAddEvent = &(posEffectEventAddTableExt[posEvent]);
        if (posEffect == posAddEvent->posEffect) {
            // Load the DLL with the code for the pos effect if needed
            if (!LoadDll(posAddEvent->dllName))
                return nullptr;
            return GetPosEffectEvent(posEffect, targetPos, posAddEvent->func, workCount, work);
        }
    }

    // Check for vanilla moves
    PosEffectEventAddTable* posEventAddTable = (PosEffectEventAddTable*)0x689D850;
    for (u32 posEvent = 0; posEvent < 5; ++posEvent) {

        PosEffectEventAddTable* posAddEvent = &(posEventAddTable[posEvent]);
        if (posEffect == posAddEvent->posEffect) {
            return GetPosEffectEvent(posEffect, targetPos, posAddEvent->func, workCount, work);
        }
    }
    return nullptr;
}

extern "C" void HandlerPosTurnCheckDone(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (BattleEventVar_GetValue(VAR_MON_ID) == BATTLE_MAX_SLOTS) {
        BattleEventItem_Remove(item);
    }
}

// Protect Moves
extern "C" void THUMB_BRANCH_HandlerProtectStart(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_ATTACKING_MON)) {
        BattleMon* currentMon = Handler_GetBattleMon(serverFlow, pokemonSlot);
        MOVE_ID previousMove = BattleMon_GetPreviousMoveID(currentMon);
        if (!getMoveFlag(previousMove, FLAG_PROTECT)) {
            HandlerParam_SetCounter* setCounter;
            setCounter = (HandlerParam_SetCounter*)BattleHandler_PushWork(serverFlow, EFFECT_COUNTER, pokemonSlot);
            setCounter->pokeID = pokemonSlot;
            setCounter->counterID = COUNTER_PROTECT;
            setCounter->value = 0;
            BattleHandler_PopWork(serverFlow, setCounter);
        }
    }
}

// Trick-or-Treat & Forest's Curse - Report the extra type
extern "C" void splitTypeCore(BattleMon* battleMon, u8* type1, u8* type2);
extern "C" b32 THUMB_BRANCH_BattleMon_HasType(BattleMon* battleMon, POKE_TYPE type) {
    if (type != TYPE_NULL) {
        u8 type1 = (u8)TYPE_NULL;
        u8 type2 = (u8)TYPE_NULL;
        splitTypeCore(battleMon, &type1, &type2);
        if (type1 == type || type2 == type ||
            BattleField_GetExtraType(battleMon->battleSlot) == type) {
            return 1;
        }
    }
    return 0;
}

// Trick-or-Treat & Forest's Curse - Calculate triple weakness/resistance with extra type
u8 effectivenessMultiplier[] = { 0, 2, 4, 8, 16, 32 };
extern "C" EFFECTIVENESS THUMB_BRANCH_GetTypeEffectivenessMultiplier(EFFECTIVENESS effectiveness1, EFFECTIVENESS effectiveness2) {
    u32 multiplier = effectivenessMultiplier[effectiveness1] * effectivenessMultiplier[effectiveness2] / 4;
    switch (multiplier)
    {
    case 0: return EFFECTIVENESS_IMMUNE;
    case 2: return EFFECTIVENESS_1_8;
    case 4: return EFFECTIVENESS_1_4;
    case 8: return EFFECTIVENESS_1_2;
    case 32: return EFFECTIVENESS_2;
    case 64: return EFFECTIVENESS_4;
    case 128: return EFFECTIVENESS_8;
    }
    return EFFECTIVENESS_1;
}

// Trick-or-Treat & Forest's Curse - Add extra type to the effectiveness calculation
extern "C" EFFECTIVENESS THUMB_BRANCH_LINK_ServerEvent_CheckMoveDamageEffectiveness_0x32(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, PokeType moveType) {

    u16 pokeType = BattleMon_GetPokeType(defendingMon);
    POKE_TYPE type = PokeTypePair_GetType1(pokeType);
    EFFECTIVENESS effectiveness = ServerEvent_CheckDamageEffectiveness(serverFlow, attackingMon,
        defendingMon, moveType, type);

    POKE_TYPE extraType = BattleField_GetExtraType(defendingMon->battleSlot);
    if (extraType != TYPE_NULL) {
        EFFECTIVENESS extraEffectiveness = ServerEvent_CheckDamageEffectiveness(serverFlow, attackingMon,
            defendingMon, moveType, extraType);
        effectiveness = GetTypeEffectivenessMultiplier(effectiveness, extraEffectiveness);
    }

    return effectiveness;
}

// Trick-or-Treat & Forest's Curse - Fix message for triple weakness/resistance with extra type
extern "C" void THUMB_BRANCH_ServerDisplay_DamageEffectiveness(ServerFlow* serverFlow, EFFECTIVENESS effectiveness) {
    if (effectiveness < EFFECTIVENESS_1 || effectiveness == EFFECTIVENESS_1_8) {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, 79, 0xFFFF0000);
    }
    else {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, 78, 0xFFFF0000);
    }
}
extern "C" void DisplayMultiEffectiveMessage(ServerFlow* serverFlow, u32 hitMonCount, EFFECTIVENESS* effectiveRecord, BattleMon** battleMons, u32 effectivenessCount, u16 msgID) {
    u8 hitSlots[3];
    sys_memset(hitSlots, BATTLE_MAX_SLOTS, 3);
    u32 hitCount = 0;
    for (u32 idx = 0; idx < hitMonCount; ++idx) {
        if (effectiveRecord[idx] > EFFECTIVENESS_1 &&
            effectiveRecord[idx] != EFFECTIVENESS_1_8) {
            hitSlots[hitCount] = BattleMon_GetID(battleMons[idx]);
            ++hitCount;
        }
    }

    switch (effectivenessCount)
    {
    case 1u:
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, msgID, hitSlots[0], 0xFFFF0000);
        break;
    case 2u:
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, msgID + 3, hitSlots[0], hitSlots[1], 0xFFFF0000);
        break;
    case 3u:
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_SetMessage, msgID + 6, hitSlots[0], hitSlots[1], hitSlots[2], 0xFFFF0000);
        break;
    }
}
extern "C" void THUMB_BRANCH_SAFESTACK_ServerDisplay_MoveEffectivenessMessage(ServerFlow* serverFlow, u32 hitMonCount, EFFECTIVENESS* effectiveRecord, BattleMon** battleMons, b32 multipleTargets) {
    u32 notEffective = 0;
    u32 superEffective = 0;
    for (u32 effectiveIdx = 0; effectiveIdx < hitMonCount; ++effectiveIdx)
    {
        EFFECTIVENESS effectiveness = effectiveRecord[effectiveIdx];
        if (effectiveness < EFFECTIVENESS_1 || effectiveness == EFFECTIVENESS_1_8) {
            ++notEffective;
        }
        else if (effectiveness > EFFECTIVENESS_1) {
            ++superEffective;
        }
    }

    if (multipleTargets)
    {
        if (superEffective) {
            DisplayMultiEffectiveMessage(serverFlow, hitMonCount, effectiveRecord, battleMons, superEffective, 6);
        }
        if (notEffective) {
            DisplayMultiEffectiveMessage(serverFlow, hitMonCount, effectiveRecord, battleMons, notEffective, 15);
        }
    }
    else if (superEffective) {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, 78, 0xFFFF0000);
    }
    else if (notEffective) {
        ServerDisplay_AddMessageImpl(serverFlow->serverCommandQueue, SCID_MessageStandard, 79, 0xFFFF0000);
    }
}

#endif // EXPAND_MOVES

#if EXPAND_ABILITIES || EXPAND_ITEMS || EXPAND_MOVES

// STAT BOOST FLAGS - Allows to distinguish if a stat boost
// was caused by Intimidate, Mirror Armor, Opportunist or Parting Shot
extern "C" b32 THUMB_BRANCH_SAFESTACK_ServerEvent_CheckStatStageChangeSuccess(ServerFlow* serverFlow, BattleMon* affectedMon, StatStage statStage, u32 attackingSlot, int volume, u32 moveSerial) {
    BattleEventVar_Push();
    u32 affectedSlot = BattleMon_GetID(affectedMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, affectedSlot);
    BattleEventVar_SetConstValue(VAR_ATTACKING_MON, attackingSlot);
    BattleEventVar_SetConstValue(VAR_MOVE_EFFECT, statStage);
    BattleEventVar_SetConstValue(VAR_VOLUME, volume);
    BattleEventVar_SetValue(VAR_INTIMIDATE_FLAG, (moveSerial & STAT_CHANGE_INTIMIDATE_FLAG) != 0); // Intimidate Flag
    BattleEventVar_SetValue(VAR_MIRROR_ARMOR_FLAG, (moveSerial & STAT_CHANGE_MIRROR_ARMOR_FLAG) != 0); // Mirror Armor Flag
    BattleEventVar_SetValue(VAR_OPPORTUNIST_FLAG, (moveSerial & STAT_CHANGE_OPPORTUNIST_FLAG) != 0); // Opportunist Flag
    BattleEventVar_SetValue(VAR_PARTING_SHOT_FLAG, (moveSerial & STAT_CHANGE_PARTING_SHOT_FLAG) != 0); // Parting Shot Flag
    BattleEventVar_SetConstValue(VAR_MOVE_SERIAL, moveSerial & 0x0FFFFFFF);
    BattleEventVar_SetRewriteOnceValue(VAR_MOVE_FAIL_FLAG, 0);
    BattleEvent_CallHandlers(serverFlow, EVENT_STAT_STAGE_CHANGE_LAST_CHECK);
    u32 failFlag = BattleEventVar_GetValue(VAR_MOVE_FAIL_FLAG);
    BattleEventVar_Pop();

    if (!failFlag) {
        return 1;
    }
    return 0;
}
extern "C" void THUMB_BRANCH_ServerEvent_StatStageChangeFail(ServerFlow* serverFlow, BattleMon* currentMon, u32 moveSerial) {
    BattleEventVar_Push();
    u32 currentSlot = BattleMon_GetID(currentMon);
    BattleEventVar_SetConstValue(VAR_MON_ID, currentSlot);
    BattleEventVar_SetConstValue(VAR_INTIMIDATE_FLAG, (moveSerial & STAT_CHANGE_INTIMIDATE_FLAG) != 0); // Intimidate Flag
    BattleEventVar_SetConstValue(VAR_MIRROR_ARMOR_FLAG, (moveSerial & STAT_CHANGE_MIRROR_ARMOR_FLAG) != 0); // Mirror Armor Flag
    BattleEventVar_SetConstValue(VAR_OPPORTUNIST_FLAG, (moveSerial & STAT_CHANGE_OPPORTUNIST_FLAG) != 0); // Opportunist Flag
    BattleEventVar_SetConstValue(VAR_PARTING_SHOT_FLAG, (moveSerial & STAT_CHANGE_PARTING_SHOT_FLAG) != 0); // Parting Shot Flag
    BattleEventVar_SetConstValue(VAR_MOVE_SERIAL, moveSerial & 0x0FFFFFFF);
    BattleEvent_CallHandlers(serverFlow, EVENT_STAT_STAGE_CHANGE_FAIL);
    BattleEventVar_Pop();
}
extern "C" void THUMB_BRANCH_HandlerIntimidate(BattleEventItem* item, ServerFlow* serverFlow, u32 pokemonSlot, u32* work) {
    if (pokemonSlot == BattleEventVar_GetValue(VAR_MON_ID)) {
        u16 frontPokeExist = Handler_GetExistFrontPokePos(serverFlow, pokemonSlot);
        u8* frontSlots = Handler_GetTempWork(serverFlow);
        u32 frontCount = Handler_ExpandPokeID(serverFlow, frontPokeExist | 0x100, frontSlots);
        if (frontCount) {
            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_ADD, pokemonSlot);

            HandlerParam_ChangeStatStage* statChange;
            statChange = (HandlerParam_ChangeStatStage*)BattleHandler_PushWork(serverFlow, EFFECT_CHANGE_STAT_STAGE, pokemonSlot);
            statChange->stat = STATSTAGE_ATTACK;
            statChange->volume = -1;
            statChange->moveAnimation = 1;
            statChange->pokeCount = frontCount;
            for (u8 frontCurrent = 0; frontCurrent < frontCount; ++frontCurrent) {
                statChange->pokeID[frontCurrent] = frontSlots[frontCurrent];
            }
            // Set the intimidate flag
            statChange->pad = STAT_CHANGE_INTIMIDATE_FLAG;
            BattleHandler_PopWork(serverFlow, statChange);

            BattleHandler_PushRun(serverFlow, EFFECT_ABILITY_POPUP_REMOVE, pokemonSlot);
        }
    }
}

#endif // EXPAND_ABILITIES || EXPAND_ITEMS || EXPAND_MOVES

#if 0 // EXPAND_FIELD_EFFECTS

extern "C" void THUMB_BRANCH_LINK_BattleEventItem_Remove_0x44(BattleEventItem * item) {
    switch (item->factorType)
    {
#if EXPAND_MOVES
    case EVENTITEM_MOVE:
        for (u32 moveIdx = 0; moveIdx < ARRAY_COUNT(moveEventAddTableExt); ++moveIdx) {

            MoveEventAddTableExt* moveEvent = &(moveEventAddTableExt[moveIdx]);
            if (item->subID == moveEvent->moveID) {
                FreeDll(moveEvent->dllName);
                break;
            }
        }
        break;
#endif
    case EVENTITEM_POS:
        break;
    case EVENTITEM_SIDE:
        for (u32 effectIdx = 0; effectIdx < ARRAY_COUNT(sideEffectEventAddTableExt); ++effectIdx) {

            SideEffectEventAddTableExt* sideEffectEvent = &(sideEffectEventAddTableExt[effectIdx]);
            if (item->subID == sideEffectEvent->sideEffect) {
                FreeDll(sideEffectEvent->dllName);
                break;
            }
        }
        break;
    case EVENTITEM_FIELD:
        for (u32 effectIdx = 0; effectIdx < ARRAY_COUNT(fieldEffectEventAddTableExt); ++effectIdx) {

            FieldEffectEventAddTableExt* fieldEffectEvent = &(fieldEffectEventAddTableExt[effectIdx]);
            if (item->subID == fieldEffectEvent->fieldEffect) {
                FreeDll(fieldEffectEvent->dllName);
                break;
            }
        }
        break;
#if EXPAND_ABILITIES
    case EVENTITEM_ABILITY:
        for (u32 abilityIdx = 0; abilityIdx < ARRAY_COUNT(abilityEventAddTableExt); ++abilityIdx) {

            AbilityEventAddTableExt* abilityEvent = &(abilityEventAddTableExt[abilityIdx]);
            if (item->subID == abilityEvent->ability) {
                FreeDll(abilityEvent->dllName);
                break;
            }
        }
        break;
#endif
#if EXPAND_ITEMS
    case EVENTITEM_ITEM:
        for (u32 itemIdx = 0; itemIdx < ARRAY_COUNT(itemEventAddTableExt); ++itemIdx) {

            ItemEventAddTableExt* itemEvent = &(itemEventAddTableExt[itemIdx]);
            if (item->subID == itemEvent->itemID) {
                FreeDll(itemEvent->dllName);
                break;
            }
        }
        break;
#endif
    default:
        break;
    }
    BattleEvent_PopItem(item);
}

#endif // EXPAND_FIELD_EFFECTS

// WARNING BattleHandler_ChangeAbility is used in BattleUpgrade.S