#ifndef __BATTLEFIELD_EXT_H
#define __BATTLEFIELD_EXT_H

#include "settings.h"
#include "Terrains.h"
#include "../BattleEngine.h"

#include "defs.h"

#include "field_effects.h"
#include "conditions.h"
#include "items.h"
#include "abilities.h"

struct BattleFieldExt
{
    WEATHER weather;
    u32 weatherTurns;
    TERRAIN terrain;
    u32 terrainTurns;
    BattleEventItem* battleEventItems[FIELD_EFFECT_AMOUNT];
    ConditionData conditionData[FIELD_EFFECT_AMOUNT];
    u32 turnCount[FIELD_EFFECT_AMOUNT];
    u32 dependPokeID[FIELD_EFFECT_AMOUNT][6];
    u32 dependPokeCount[FIELD_EFFECT_AMOUNT];
    u32 effectEnableFlags[FIELD_EFFECT_AMOUNT];

#if EXPAND_ABILITIES
    // Stores the ID of an item being forcibly used by [BattleHandler_ForceUseHeldItem]
    // Basically stores the data in [ItemEvent_AddTempItem] so [ServerEvent_EquipTempItem] can use it
    ITEM tempItem;

    // Is set in [ServerControl_DamageRoot] whenever a move hits twice bacause of the ability
    u8 parentalBondFlag = 0;

    // Amount of Pokemon with Aura Break
    u8 auraBreakMons;

    // Amount of Pokemon mantaining current extreme weather
    u8 extremeWeatherMons;

    // Stores the damage a Substitute takes each action
    // - Reset in [ServerControl_DamageRoot] & [CommonEmergencyExitCheck]
    // - Set in [BattleMon_AddSubstituteDamage]
    // - Used in [CommonEmergencyExitCheck]
    u32 actionSubstituteDamage[BATTLE_MAX_SLOTS];

    // Tracks which Emergency Exit Pokémon should switch after the move is over 
    // - Used in [CommonEmergencyExitCheck] & [HandlerEmergencyExitSwitch]
    u32 emergencyExitFlag;

    // Stores the Pokémon that start the battle
    // - Set in [ServerFlow_SetupBeforeFirstTurn]
    u8 firstTurnMons[6] = { BATTLE_MAX_SLOTS };

    // Each bit represents a battle slot, 1 means the mon has used Battle Bond and thus can't use it again
    u32 battleBondFlag;

#if GEN9_INTREPID_SWORD
    // Each bit represents a battle slot, 1 means the mon has used Intrepid Sword and thus can't use it again
    u32 intrepidSwordFlag;
#endif
#if GEN9_DAUNTLESS_SHIELD
    // Each bit represents a battle slot, 1 means the mon has used Dauntless Shield and thus can't use it again
    u32 dauntlessShieldFlag;
#endif

    // Amount of Pokemon Neutralizing Gas
    u8 neutralizingGasMons;

    // When a Pokémon has the ability As One this represents the ability that the pop up should display
    // First Ability (Unnerve) -> 1000 0000
    // Second Ability (Chilling/Grim Neigh) -> 0100 0000
    u8 asOneAbilityPopUpFlags = 0;

    // Stores the 2 possible paired Dondozos battle slots for Commander
    u8 pairedDondozos[4];

    // Each bit represents a battle slot, 1 means the mon has forced the activation of Protosynthesis or 
    // Quark Drive usign a Booster Energy
    u32 paradoxAbilityFlag;

    // Each bit represents a battle slot, 1 means the mon has already used Supersweet Syrup
    u32 supersweetSyrupFlag;
#endif // EXPAND_ABILITIES
};

extern "C" BattleFieldExt * BattleField_Init(u16 heapID);
extern "C" void BattleField_InitCore(BattleFieldExt * battleField, WEATHER weather);
extern "C" void BattleField_ClearFactorWork(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect);
extern "C" WEATHER BattleField_GetWeatherCore(BattleFieldExt * battleField);
extern "C" u32 BattleField_GetWeatherTurnsCore(BattleFieldExt * battleField);
extern "C" void BattleField_SetWeatherCore(BattleFieldExt * battleField, WEATHER weather, u32 turns);
extern "C" WEATHER BattleField_TurnCheckWeatherCore(BattleFieldExt * battleField);
extern "C" b32 BattleField_CheckFieldEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect);
extern "C" b32 BattleField_AddDependPokeCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, u32 pokemonSlot);
extern "C" b32 BattleField_AddEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect, ConditionData conditionData, b32 addEvents);
extern "C" b32 BattleField_RemoveEffectCore(BattleFieldExt * battleField, FIELD_EFFECT fieldEffect);
extern "C" void BattleField_RemoveDependPokeEffectCore(BattleFieldExt * battleField, u32 pokemonSlot);
extern "C" b32 BattleField_CheckImprisonCore(BattleFieldExt * battleField, PokeCon * pokeCon, BattleMon * battleMon, MOVE_ID moveID);
extern "C" void BattleField_TurnCheckCore(BattleFieldExt * battleField, void(*callback)(FIELD_EFFECT, ServerFlow*), ServerFlow * serverFlow);
extern "C" WEATHER BattleField_GetWeather();
extern "C" u32 BattleField_GetWeatherTurns();
extern "C" void BattleField_EndWeather(BattleFieldExt * battleField);
extern "C" void BattleField_SetWeather(WEATHER weather, u32 turns);
extern "C" b32 BattleField_CheckEffect(FIELD_EFFECT fieldEffect);

#if EXPAND_ABILITIES

extern "C" b32 BattleField_CheckParentalBondFlag();
extern "C" void BattleField_SetParentalBondFlag();
extern "C" void BattleField_ResetParentalBondFlag();

extern "C" u32 BattleField_GetAuraBreakMons();
extern "C" void BattleField_AddAuraBreakMon();
extern "C" void BattleField_RemoveAuraBreakMon();

extern "C" u32 BattleField_GetExtremeWeatherMons();
extern "C" void BattleField_AddExtremeWeatherMon();
extern "C" void BattleField_RemoveExtremeWeatherMon();
extern "C" void BattleField_ResetExtremeWeatherMons();

extern "C" u32 BattleField_GetSubstituteDamage(u32 battleSlot);
extern "C" void BattleField_SetSubstituteDamage(u32 battleSlot, u32 value);

extern "C" b32 BattleField_CheckEmergencyExitFlag(u32 battleSlot);
extern "C" void BattleField_SetEmergencyExitFlag(u32 battleSlot);
extern "C" void BattleField_ResetEmergencyExitFlag(u32 battleSlot);

extern "C" b32 BattleField_CheckBattleBondFlag(u32 battleSlot);
extern "C" void BattleField_SetBattleBondFlag(u32 battleSlot);

extern "C" void HandlerAddExtremeWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather);
extern "C" void HandlerRemoveExtremeWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather);
extern "C" void HandlerExtremeWeatherImmunity(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather, POKE_TYPE type, u16 msgID);

extern "C" b32 CanWeatherBeChanged(ServerFlow * serverFlow, WEATHER weather, u32 turns, u8 isExtreme);
extern "C" void ChangeWeather(ServerFlow * serverFlow, WEATHER weather, u32 turns, u8 isExtreme);

#if GEN9_INTREPID_SWORD
extern "C" b32 BattleField_CheckIntrepidSwordFlag(u32 battleSlot);
extern "C" void BattleField_SetIntrepidSwordFlag(u32 battleSlot);
#endif

#if GEN9_DAUNTLESS_SHIELD
extern "C" b32 BattleField_CheckDauntlessShieldFlag(u32 battleSlot);
extern "C" void BattleField_SetDauntlessShieldFlag(u32 battleSlot);
#endif

extern "C" u32 BattleField_GetNeutralizingGasMons();
extern "C" void BattleField_AddNeutralizingGasMon();
extern "C" void BattleField_RemoveNeutralizingGasMon();

extern "C" b32 BattleField_GetAsOnePopUpFlags();
extern "C" void BattleField_SetAsOnePopUpFlags(u8 flags);

extern "C" void BattleField_AddPairedDondozo(u32 battleSlot, u32 * index);
extern "C" b32 BattleField_IsDondozoPaired(u32 battleSlot);
extern "C" u8 BattleField_GetDondozoFromPair(u32 pairIndex);

extern "C" b32 BattleField_CheckParadoxAbilityFlag(u32 battleSlot);
extern "C" void BattleField_SetParadoxAbilityFlag(u32 battleSlot);
extern "C" void BattleField_ResetParadoxAbilityFlag(u32 battleSlot);

extern "C" b32 BattleField_CheckSupersweetSyrupFlag(u32 battleSlot);
extern "C" void BattleField_SetSupersweetSyrupFlag(u32 battleSlot);

extern "C" void CommonAddFieldEffectAbilityHandler(ServerFlow * serverFlow, u32 pokemonSlot, FIELD_EFFECT fieldEffect, u16 msgID);
extern "C" void CommonRemoveFieldEffectAbilityHandler(ServerFlow * serverFlow, u32 pokemonSlot, FIELD_EFFECT fieldEffect);

extern "C" b32 SwitchedInThisTurn(ServerFlow * serverFlow, BattleMon * battleMon);

extern "C" void ServerEvent_ConsumeItem(ServerFlow * serverFlow, u32 currentSlot, ITEM itemID);
extern "C" void ServerEvent_UseItemAfter(ServerFlow * serverFlow, u32 currentSlot, ITEM itemID);
extern "C" void ServerEvent_UseTempItemAfter(ServerFlow * serverFlow, u32 currentSlot, u32 attackingSlot, ITEM itemID);
extern "C" u8 ServerEvent_ChangeAbilityCheckFail(ServerFlow * serverFlow, u32 pokemonSlot);
extern "C" void ServerEvent_AbilityNullifyCured(ServerFlow * serverFlow, BattleMon * battleMon);
extern "C" void ServerEvent_SimpleDamageReaction(ServerFlow * serverFlow, BattleMon * battleMon, u32 damage);
extern "C" void ServerEvent_SwapPoke(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon);
extern "C" void ServerEvent_UseTempItemBefore(ServerFlow * serverFlow, u32 currentSlot, u32 attackingSlot, ITEM itemID);
extern "C" void ServerEvent_SwitchInPriority(ServerFlow * serverFlow);

#endif // EXPAND_ABILITIES

extern "C" WEATHER Handler_CheckWeather(ServerFlow * serverFlow, u32 pokemonSlot, WEATHER weather);

#endif // __BATTLEFIELD_EXT_H
