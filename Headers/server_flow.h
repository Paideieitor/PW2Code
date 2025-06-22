#ifndef __SERVER_FLOW_H
#define __SERVER_FLOW_H

#include "defs.h"

#include "main_module.h"
#include "action_order.h"
#include "handler_params.h"
#include "field_effects.h"
#include "battle_events.h"

typedef u32 MOVE_ID;
typedef u32 WEATHER;
typedef u32 SIDE_EFFECT;

struct ServerCommandQueue;
struct ArcTool;

enum TurnFlag : u32
{
    TURNFLAG_ACTIONSTART = 0x0,
    TURNFLAG_ACTIONDONE = 0x1,
    TURNFLAG_DAMAGED = 0x2,
    TURNFLAG_MOVEPROCDONE = 0x3,
    TURNFLAG_FLINCH = 0x4,
    TURNFLAG_FOCUSPUNCHREADY = 0x5,
    TURNFLAG_FOCUSPUNCHFAIL = 0x6,
    TURNFLAG_PROTECT = 0x7,
    TURNFLAG_ITEMCONSUMED = 0x8,
    TURNFLAG_CANTUSEITEM = 0x9,
    TURNFLAG_COMBOMOVEREADY = 0xA,
    TURNFLAG_HIDEMOVEOFF = 0xB,
    TURNFLAG_MOVED = 0xC,
    TURNFLAG_TURNCHECKCONDITIONPASSED = 0xD,
    TURNFLAG_ACCURACYUP = 0xE,
    TURNFLAG_USINGFLING = 0xF,
};

struct SWAN_ALIGNED(4) m_record
{
    u32 turn;
    u16 moveID;
    u8 pokeID;
    u8 effective;
};

struct MoveRecord
{
    u32 ptr;
    m_record record[120];
};

struct FaintRecordUnit
{
    u8 count;
    u8 expChecked[24];
    u8 faintPokeID[24];
};

struct FaintRecord
{
    FaintRecordUnit turnRecord[4];
};

struct SWAN_ALIGNED(4) PokeSet
{
    BattleMon* battleMon[6];
    u16 damage[6];
    u16 substituteDamage[6];
    u8 damageType[6];
    u16 sortWork[6];
    u8 count;
    u8 countMax;
    u8 getIdx;
    u8 targetPosCount;
};

struct MoveAnimCtrl
{
    u16 moveID;
    u8 attackerPos;
    u8 targetPos;
    u8 effectIndex;
    u8 flags;
    u16 subEff;
};

enum MoveTarget : u32
{
    TARGET_OTHER_SELECT = 0x0,
    TARGET_ALLY_USER_SELECT = 0x1,
    TARGET_ALLY_SELECT = 0x2,
    TARGET_ENEMY_SELECT = 0x3,
    TARGET_OTHER_ALL = 0x4,
    TARGET_ENEMY_ALL = 0x5,
    TARGET_ALLY_ALL = 0x6,
    TARGET_USER = 0x7,
    TARGET_ALL = 0x8,
    TARGET_ENEMY_RANDOM = 0x9,
    TARGET_FIELD = 0xA,
    TARGET_SIDE_ENEMY = 0xB,
    TARGET_SIDE_ALLY = 0xC,
    TARGET_UNKNOWN = 0xD,
};

struct MoveParam
{
    u16 moveID;
    u16 originalMoveID;
    u16 userType;
    u8 moveType;
    u8 damageType;
    u32 category;
    u32 targetType;
    u32 flags;
};

struct HitCheckParam
{
    u8 countMax;
    u8 count;
    u8 checkEveryTime;
    u8 multiHitMove;
    u8 putAnimCmd;
    u8 multiHitEffectiveness;
};

struct SWAN_PACKED SWAN_ALIGNED(1) MoveStealParam
{
    u8 stealCount;
    u8 stealPokeID[6];
    u8 targetPos[6];
    u8 targetPokeID[6];
};

struct SWAN_PACKED SWAN_ALIGNED(1) calc_damage_record
{
    u16 damage;
    u16 pokeData; // CalcDmgPokeData
};

struct SWAN_PACKED SWAN_ALIGNED(2) CALC_DAMAGE_REC
{
    u8 real_hit_count;
    u8 substitute_hit_count;
    u8 total_hit_count;
    u8 unkn;
    calc_damage_record record[6];
};

struct POKESET_STACK_UNIT
{
    PokeSet targetOriginal;
    PokeSet target;
    PokeSet ally;
    PokeSet enemy;
    PokeSet damaged;
    PokeSet stealTarget;
    PokeSet psetTemp;
    MoveAnimCtrl moveAnimCtrl;
    MoveParam moveParam;
    MoveParam moveParamOriginal;
    HitCheckParam hitCheck;
    MoveStealParam moveStealParam;
    MoveStealParam magicCoatParam;
    CALC_DAMAGE_REC calcDamageAlly;
    CALC_DAMAGE_REC calcDamageEnemy;
    u8 defaultTargetPos;
    u8 field_28D;
    u8 field_28E;
    u8 field_28F;
};

struct PosPokeState
{
    u8 enable;
    u8 clientID;
    u8 existPokeID;
};

struct PosPoke
{
    PosPokeState state[6];
    u8 lastPos[24];
};

struct SWAN_ALIGNED(2) CalcExpWork
{
    u32 level;
    u8 isBoosted;
    u8 hp;
    u8 atk;
    u8 def;
    u8 spatk;
    u8 spdef;
    u8 spe;
};

enum ActionIgnoreReason : u32
{
    REASON_NONE = 0x0,
    REASON_OTHER_MOVE = 0x1,
    REASON_FALL_ASLEEP = 0x2,
    REASON_CONFUSION = 0x3,
    REASON_OVERLEVELED = 0x4,
    REASON_ASLEEP = 0x5,
};

struct EffectivenessCounter
{
    u16 noEffectHits;
    u16 superEffectiveHits;
    u16 notVeryEffectiveHits;
    u16 opponentNoEffectHits;
    u16 opponentSuperEffectiveHits;
    u16 opponentNotVeryEffectiveHits;
};

struct SWAN_ALIGNED(8) ServerFlow
{
    BtlServerWk* server;
    MainModule* mainModule;
    PokeCon* pokeCon;
    ServerCommandQueue* serverCommandQueue;
    u32 turnCount;
    u32 flowResult;
    u32 heapHandle;
    MoveRecord moveRecord;
    FaintRecord faintRecord;
    ArcTool* evolutionDataPtr;
    MoveAnimCtrl* moveAnimCtrl;
    MoveStealParam* moveStealParam;
    MoveStealParam* magicCoatParam;
    HitCheckParam* hitCheckParam;
    EscapeInfo escapeInfo;
    u32 field_4C0;
    u32 field_4C4;
    u32 field_4C8;
    u16 field_4CC;
    u16 field_4CE;
    u32 field_4D0;
    u8 field_4D4[672];
    u32 simulationCounter;
    u32 moveSerial;
    u8 cmdBuildStep;
    u8 field_77D;
    u8 turnCheckSeq;
    u8 defaultTargetPos;
    u16 heapID;
    u8 numActOrder;
    u8 numEndActOrder;
    u8 field_784;
    u8 revivedPokeCount;
    u8 field_786;
    u8 field_787;
    u8 field_788;
    u8 thruDeadMsgPokeID;
    u8 field_78A;
    u8 gap78B;
    u8 field_78C;
    u8 field_78D;
    u8 field_78E;
    u8 field_78F;
    u8 field_790;
    u8 revivePokeID[24];
    u8 pokeInFlag[24];
    u8 field_7C1[24];
    u8 switchCount[4];
    u8 field_7DD;
    u8 field_7DE;
    u8 field_7DF;
    ActionOrderWork actionOrderWork[6];
    ActionOrderWork tempActionOrderWork;
    PokeSet* setTargetOriginal;
    PokeSet* setTarget;
    PokeSet* setAlly;
    PokeSet* setEnemy;
    PokeSet* pokeSetDamaged;
    PokeSet* setStealTarget;
    PokeSet* pokeSetTemp;
    CALC_DAMAGE_REC* calcDamageAlly;
    CALC_DAMAGE_REC* calcDamageEnemy;
    POKESET_STACK_UNIT pokesetUnit[7];
    u32 pokesetStackPtr;
    PokeSet currentpokeSet;
    MoveParam* moveParam;
    MoveParam* moveParamOriginal;
    PosPoke posPoke;
    u8 field_1AE2;
    u8 field_1AE3;
    HandlerParam_StrParams strParam;
    CalcExpWork* levelUpInfo;
    u8 gap1B10[68];
    u32 field_1B54;
    u8 gap1B58[316];
    u8 field_1C88;
    u8 field_1C89;
    u8 field_1C8A;
    u8 field_1C97;
    u8 gap1C98[224];
    u32 HEManager;
    u8 gap1D7C[8];
    u8 field_1D84;
    u8 field_1D85;
    u8 field_1D86;
    u8 field_1D87;
    u8 field_1D88;
    u8 field_1D89;
    u8 field_1D8A;
    u8 gap1D8B[481];
    u8 field_1F6C;
    u8 gap1F6D[11];
    u16 prevUsedMove;
    u8 field_1F7A;
    u8 field_1F7B;
    ActionIgnoreReason actionIgnoreReason;
    EffectivenessCounter effectivenessCounter;
    u32 dmgAffRec;
    u8 gap1F90[8];
    u8 field_1F98;
    u8 gap1F99[83];
    u8 field_1FEC[4];
    u8 TempWork;
    u8 field_1FF1;
    u8 gap1FF2[318];
    u32 field_2130;
    u8 gap2134[8];
    u8 field_213C;
    u8 field_213D;
    u8 field_213E;
    u8 field_213F;
};

struct EffectivenessRecorder
{
    u32 effectiveness[24];
};

struct FRONT_POKE_SEEK_WORK
{
    u8 clientIdx;
    u8 pokeIdx;
    u8 endFlag;
    u8 unk;
};

extern "C" u32 ServerFlow_ReqChangePokeForServer(ServerFlow * serverFlow, u16 * a2);
extern "C" b32 ServerFlow_IsCompetitorScenarioMode(ServerFlow* serverFlow);
extern "C" u32 ActionOrder_Proc(ServerFlow* serverFlow, ActionOrderWork* actionOrder);

extern "C" b32 ServerControl_TurnCheck(ServerFlow* serverFlow);
extern "C" b32 ServerControl_CheckMatchup(ServerFlow* serverFlow);
extern "C" void ServerControl_CheckActivation(ServerFlow* serverFlow);
extern "C" u32 ServerControl_CheckExpGet(ServerFlow* serverFlow);
extern "C" b32 ServerControl_CheckFloating(ServerFlow* serverFlow, BattleMon* battleMon, b32 checkFlyingType);
extern "C" u32 ServerControl_FieldEffectCore(ServerFlow * serverFlow, FIELD_EFFECT fieldEffect, ConditionData condData, u32 addDependMons);
extern "C" void ServerControl_SortBySpeed(ServerFlow* serverFlow, PokeSet* pokeSet);
extern "C" void ServerControl_CheckItemReaction(ServerFlow* serverFlow, BattleMon* battleMon, u32 reaction);
extern "C" b32 ServerControl_HideTurnCancel(ServerFlow* serverFlow, BattleMon* battleMon, CONDITION_FLAG conditionFlag);
extern "C" void ServerControl_CureCondition(ServerFlow* serverFlow, BattleMon* battleMon, CONDITION condition, ConditionData* prevCondData);
extern "C" void ServerControl_DamageProcessingStart(ServerFlow* serverFlow, BattleMon* battleMon, MoveParam* moveParam);
extern "C" void ServerControl_ApplyDamageToRecover(ServerFlow * serverFlow, BattleMon * attackingMon, MoveParam * moveParam, PokeSet * targetSet);
extern "C" u32 ServerControl_DamageMultiCount(ServerFlow * serverFlow, MoveParam * moveParam, BattleMon * attackingMon, PokeSet * targetSet, u32 * effectiveness);
extern "C" void ServerControl_CalcRecoil(ServerFlow* serverFlow, BattleMon* battleMon, MOVE_ID moveID, u32 damage);
extern "C" u32 ServerControl_DamageSingleCount(ServerFlow * serverFlow, MoveParam * moveParams, BattleMon * attackingMon, PokeSet * targetSet, u32 * effectiveness, u32 * animBuffer, u32 delayMove);
extern "C" void ServerControl_DamageProcessingEnd(ServerFlow* serverFlow, MoveParam* moveParam, BattleMon* attackingMon, PokeSet* targetSet, u32 damage, u32 delayMove);
extern "C" void ServerControl_ChangeWeatherAfter(ServerFlow* serverFlow, WEATHER weather);
extern "C" b32 ServerControl_ChangeWeatherCheck(ServerFlow* serverFlow, WEATHER weather, u32 turns);
extern "C" b32 ServerControl_CheckFainted(ServerFlow* serverFlow, BattleMon* battleMon);
extern "C" void ServerControl_SwitchInCore(ServerFlow* serverFlow, u32 clientID, u32 switchInSlot, u32 switchOutSlot);
extern "C" u32 ServerControl_Fight(ServerFlow* serverFlow, BattleMon* attackingMon, BattleAction_Fight* actionParams, u32 speed);
extern "C" void ServerControl_CheckMoveExeSleepCure(ServerFlow* serverFlow, BattleMon* attackingMon);
extern "C" b32 ServerControl_CheckMoveExeFreezeThaw(ServerFlow* serverFlow, BattleMon* attackingMon, MOVE_ID moveID);
extern "C" void ServerControl_MoveCore(ServerFlow* serverFlow, u32 clientID, int partyMon1, int partyMon2, b32 a5);
extern "C" void ServerControl_AfterMove(ServerFlow* serverFlow, u32 clientID, int partyMon1, int partyMon2);
extern "C" b32 ServerControl_IsGuaranteedHit(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon);
extern "C" b32 ServerControl_CheckNoEffectCore(ServerFlow* serverFlow, u16* moveID, BattleMon* attackingMon, BattleMon* defendingMon, int dmgAffRec, BattleEventType eventType);
extern "C" b32 ServerControl_HideTurnStart(ServerFlow * serverFlow, BattleMon * attackingMon, u32 attackingPos, PokeSet * targetSet, u16 moveID);
extern "C" CONDITION_FLAG ServerControl_ChargeUpLockClear(ServerFlow * serverFlow, BattleMon * attackingMon);

enum ServerCommandID : u32
{
    SCID_MessageStandard = 0x5A,
    SCID_MessageStandardSE = 0x5C,
    SCID_SetMessage = 0x5B,
    SCID_SetMessageSE = 0x5D,
    SCID_MoveMessage = 0x59,
    SCID_MoveAnim = 0x30,
    SCID_Hide = 0x31,
    SCID_MoveDamage = 0x32,
    SCID_MoveDamage_MultiHit = 0x33,
    SCID_OHKOMove = 0x34,
    SCID_StatusIcon = 0x35,
    SCID_ConfusionDamage = 0x36,
    SCID_Faint = 0x39,
    SCID_Revive = 0x3A,
    SCID_SwitchOutMessage = 0x3B,
    SCID_SwitchOut = 0x3C,
    SCID_SwitchIn_0 = 0x3D,
    SCID_RaiseStat = 0x37,
    SCID_LowerStat = 0x38,
    SCID_sub_21B7E00 = 0x3E,
    SCID_WeatherStart = 0x3F,
    SCID_WeatherEnd = 0x40,
    SCID_SimpleHP = 0x41,
    SCID_UseItem = 0x42,
    SCID_AbilityPopupIn = 0x57,
    SCID_AbilityPopupOut = 0x58,
    SCID_SubtractHP = 0x1,
    SCID_AddHP = 0x2,
    SCID_HPZero = 0x3,
    SCID_SubtractPP = 0x4,
    SCID_SubtractPPOrg = 0x5,
    SCID_MoveUsed = 0x6,
    SCID_AddPP = 0x7,
    SCID_AddPPOrg = 0x8,
    SCID_StatStageUp = 0x9,
    SCID_StatStageDown = 0xA,
    SCID_StatStageSetAll = 0xB,
    SCID_StatStageRecover = 0xC,
    SCID_StatStageReset = 0xD,
    SCID_AddCritStage = 0xE,
    SCID_SetMoveCondition = 0xF,
    SCID_CureStatusCondition = 0x10,
    SCID_CureMoveCondition = 0x11,
    SCID_SwitchIn = 0x12,
    SCID_SetRealStat = 0x13,
    SCID_SetWeight = 0x14,
    SCID_ChangeType = 0x16,
    SCID_MoveConditionTurnCheck = 0x15,
    SCID_ConsumeItem = 0x17,
    SCID_UpdateUsedMove = 0x18,
    SCID_SetContinuationFlag = 0x19,
    SCID_ResetContinuationFlag = 0x1A,
    SCID_SetTurnFlag = 0x1B,
    SCID_ResetTurnFlag = 0x1C,
    SCID_ChangeAbility = 0x1D,
    SCID_SetItem = 0x1E,
    SCID_UpdateMoveID = 0x1F,
    SCID_SwitchOutClear = 0x20,
    SCID_AddFieldEffect = 0x21,
    SCID_AddFieldEffectDepend = 0x22,
    SCID_RemoveFieldEffectDepend = 0x23,
    SCID_RemoveFieldEffect = 0x24,
    SCID_SetPokeCounter = 0x25,
    SCID_BatonPass = 0x26,
    SCID_CreateSubstitute = 0x27,
    SCID_RemoveSubstitute = 0x28,
    SCID_sub_21B9088 = 0x29,
    SCID_SetIllusion = 0x2A,
    SCID_ClearConsumedItem = 0x2B,
    SCID_CureMoveConditionDependPoke = 0x2C,
    SCID_AddMoveDamage = 0x2D,
    SCID_TurnCheck = 0x2E,
    SCID_TurnCheckField = 0x2F,
    SCID_sub_21B7A44 = 0x43,
    SCID_sub_21B7AA0 = 0x44,
    SCID_Experience = 0x45,
    SCID_BallThrow = 0x46,
    SCID_sub_21B857C = 0x47,
    SCID_sub_21B85E4 = 0x48,
    SCID_ChangeAbility_0 = 0x49,
    SCID_SwapAbility = 0x4A,
    SCID_IllusionBreak = 0x4B,
    SCID_SimpleAnim = 0x4C,
    SCID_AnimByPos = 0x4D,
    SCID_AnimByVector = 0x4E,
    SCID_ChangeForm = 0x4F,
    SCID_Move = 0x50,
    SCID_SubstituteCreate = 0x51,
    SCID_SubstituteDelete = 0x52,
    SCID_Transform = 0x53,
    SCID_SubstituteDamage = 0x54,
    SCID_PlayWinBGM = 0x55,
    SCID_HideMessageWindow = 0x56,
};

extern "C" void ServerDisplay_AddCommon(ServerCommandQueue* serverCommandQueue, ServerCommandID a2, ...);
extern "C" void ServerDisplay_AddMessageImpl(ServerCommandQueue* serverCommandQueue, ServerCommandID a2, u16 msgID, ...);
extern "C" u32 ServerDisplay_IllusionSet(ServerFlow* serverFlow, u16* a2);
extern "C" void ServerDisplay_AbilityPopupAdd(ServerFlow* serverFlow, BattleMon* battleMon);
extern "C" void ServerDisplay_AbilityPopupRemove(ServerFlow* serverFlow, BattleMon* battleMon);
extern "C" void ServerDisplay_SetConditionFlag(ServerFlow* serverFlow, BattleMon* battleMon, CONDITION_FLAG flag);
extern "C" void ServerDisplay_SetTurnFlag(ServerFlow* serverFlow, BattleMon* battleMon, TurnFlag flag);
extern "C" u32 ServerDisplay_MoveAnimation(ServerFlow* serverFlow, MOVE_ID moveID, MoveAnimCtrl* moveAnimCtrl, u16 animBuffer);
extern "C" void ServerDisplay_SimpleHP(ServerFlow* serverFlow, BattleMon* currentMon, int damage, b32 sendCommand);
extern "C" void ServerDisplay_SkillSwap(ServerFlow* serverFlow, BattleMon* attackingMon, PokeSet* targetSet);
extern "C" void ServerDisplay_AddCondition(ServerFlow * serverFlow, BattleMon * battleMon, MoveCondition condition, ConditionData condData);
extern "C" void ServerDisplay_MoveFail(ServerFlow * serverFlow);
extern "C" void ServerDisplay_MoveAvoid(ServerFlow* serverFlow, BattleMon* attackingMon);
extern "C" void ServerDisplay_ResetConditionFlag(ServerFlow* serverFlow, BattleMon* battleMon, CONDITION_FLAG conditionFlag);

extern "C" BattleMon* PokeCon_GetBattleMon(PokeCon* pokeCon, u32 index);
extern "C" BattleParty* PokeCon_GetBattleParty(PokeCon* pokeCon, u32 idx);

extern "C" void PokeSet_SeekStart(PokeSet* pokeSet);
extern "C" BattleMon* PokeSet_SeekNext(PokeSet* pokeSet);
extern "C" void PokeSet_Clear(PokeSet* pokeSet);
extern "C" u32 PokeSet_GetCount(PokeSet* pokeSet);
extern "C" u32 PokeSet_GetCountMax(PokeSet* pokeSet);
extern "C" void PokeSet_Remove(PokeSet* pokeSet, BattleMon* battleMon);
extern "C" BattleMon* PokeSet_Get(PokeSet* pokeSet, u32 idx);

extern "C" BattleMon* Handler_GetBattleMon(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" u32 Handler_PokeIDToPokePos(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" u32 Handler_PokePosToPokeID(ServerFlow* serverFlow, u32 pokePos);
extern "C" u32 Handler_ExpandPokeID(ServerFlow* serverFlow, u16 pokePos, u8* battleSlots);
extern "C" u32 Handler_ReqMoveTargetAuto(ServerFlow * serverFlow, u32 attackingSlot, MOVE_ID moveID);
extern "C" b32 Handler_CheckMatchup(ServerFlow* serverFlow);
extern "C" b32 HandlerCommon_CheckIfCanStealPokeItem(ServerFlow* serverFlow, u32 thiefSlot, u32 targetSlot);
extern "C" b32 HandlerCommon_CheckTargetMonID(u32 pokemonSlot);
extern "C" b32 Handler_IsMonSwitchOutInterrupted(ServerFlow* serverFlow);
extern "C" b32 Handler_IsTargetInRange(ServerFlow* serverFlow, u32 attackingSlot, u32 defendingSlot, MOVE_ID moveID);
extern "C" b32 Handler_IsSimulationMode(ServerFlow* serverFlow);
extern "C" b32 Handler_IsSideEffectActive(ServerFlow* serverFlow, u32 side, SIDE_EFFECT sideEffect);
extern "C" u32 Handler_GetExistFrontPokePos(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" u8* Handler_GetTempWork(ServerFlow* serverFlow);

extern "C" u32 AddConditionCheckFailOverwrite(ServerFlow * serverFlow, BattleMon * defendingMon, CONDITION condition, ConditionData condData, u8 overrideMode);
extern "C" u32 AddConditionCheckFailStandard(ServerFlow * serverFlow, BattleMon * defendingMon, u32 failStatus, CONDITION condition);
extern "C" void Condition_CheckForceSwitchFail(ServerFlow * serverFlow, BattleMon * battleMon);
extern "C" b32 CommonConditionCodeMatch(ServerFlow* serverFlow, u32 pokemonSlot, CONDITION condition);
extern "C" b32 CommonDamageRecoverCheck(ServerFlow* serverFlow, u32 pokemonSlot, POKE_TYPE pokeType);
extern "C" void CommonTypeRecoverHP(ServerFlow* serverFlow, u32 pokemonSlot, u32 hearPercent);
extern "C" void CommonWeatherRecoveryAbility(ServerFlow* serverFlow, u32 pokemonSlot, WEATHER weather);

extern "C" b32 AbilityEvent_RollEffectChance(ServerFlow* serverFlow, u32 effectChance);

extern "C" u32 HEManager_PushState(u32* HEManager);
extern "C" void HEManager_PopState(u32* HEManager, u32 HEID);

extern "C" u32 HitCheck_IsMultiHitMove(HitCheckParam* hitCheck);

extern "C" u32 PosPoke_GetPokeExistPos(PosPoke* posPoke, u32 battleSlot);
extern "C" b32 DoesBattleMonExist(PosPoke* posPoke, u32 battleSlot);
extern "C" u8 GetSideFromOpposingMonID(u32 battleSlot);
extern "C" void Condition_CheckUnaffectedByType(ServerFlow* serverFlow, BattleMon* battleMon);

extern "C" void TurnFlag_Set(BattleMon* battleMon, TurnFlag flag);
extern "C" void Turnflag_Clear(BattleMon* battleMon, TurnFlag flag);
extern "C" b32 BattleMon_GetTurnFlag(BattleMon* battleMon, TurnFlag turnFlag);

extern "C" void FaintRecord_Add(FaintRecord * faintRecord, u8 battleSlot);

extern "C" void FRONT_POKE_SEEK_InitWork(FRONT_POKE_SEEK_WORK* frontSet, ServerFlow* serverFlow);
extern "C" b32 FRONT_POKE_SEEK_GetNext(FRONT_POKE_SEEK_WORK* frontSet, ServerFlow* serverFlow, BattleMon** battleMon);

#endif // __SERVER_FLOW_H
