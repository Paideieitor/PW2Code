#ifndef __CONDITIONS_H
#define __CONDITIONS_H

struct BattleMon;

typedef u32 CONDITION;
enum MoveCondition : u32
{
    CONDITION_NONE = 0x0,
    CONDITION_PARALYSIS = 0x1,
    CONDITION_SLEEP = 0x2,
    CONDITION_FREEZE = 0x3,
    CONDITION_BURN = 0x4,
    CONDITION_POISON = 0x5,
    CONDITION_CONFUSION = 0x6,
    CONDITION_ATTRACT = 0x7,
    CONDITION_BIND = 0x8,
    CONDITION_NIGHTMARE = 0x9,
    CONDITION_CURSE = 0xA,
    CONDITION_TAUNT = 0xB,
    CONDITION_TORMENT = 0xC,
    CONDITION_DISABLE = 0xD,
    CONDITION_YAWN = 0xE,
    CONDITION_HEALBLOCK = 0xF,
    CONDITION_GASTROACID = 0x10,
    CONDITION_FORESIGHT = 0x11,
    CONDITION_LEECHSEED = 0x12,
    CONDITION_EMBARGO = 0x13,
    CONDITION_PERISHSONG = 0x14,
    CONDITION_INGRAIN = 0x15,
    CONDITION_BLOCK = 0x16,
    CONDITION_ENCORE = 0x17,
    CONDITION_ROOST = 0x18,
    CONDITION_MOVELOCK = 0x19,
    CONDITION_CHARGELOCK = 0x1A,
    CONDITION_CHOICELOCK = 0x1B,
    CONDITION_MUSTHIT = 0x1C,
    CONDITION_LOCKON = 0x1D,
    CONDITION_FLOATING = 0x1E,
    CONDITION_KNOCKEDDOWN = 0x1F,
    CONDITION_TELEKINESIS = 0x20,
    CONDITION_SKYDROP = 0x21,
    CONDITION_ACCURACYUP = 0x22,
    CONDITION_AQUARING = 0x23,
    CONDITION_24 = 0x24,
};

typedef u32 CONDITION_FLAG;
enum ConditionFlag : u32
{
    CONDITIONFLAG_ACTIONDONE = 0x0,
    CONDITIONFLAG_NOSWITCH = 0x1,
    CONDITIONFLAG_CHARGE = 0x2,
    CONDITIONFLAG_FLY = 0x3,
    CONDITIONFLAG_DIVE = 0x4,
    CONDITIONFLAG_DIG = 0x5,
    CONDITIONFLAG_SHADOWFORCE = 0x6,
    CONDITIONFLAG_DEFENSECURL = 0x7,
    CONDITIONFLAG_MINIMIZE = 0x8,
    CONDITIONFLAG_FOCUSENERGY = 0x9,
    CONDITIONFLAG_POWERTRICK = 0xA,
    CONDITIONFLAG_MICLEBERRY = 0xB,
    CONDITIONFLAG_NOACTION = 0xC,
    CONDITIONFLAG_FLASHFIRE = 0xD,
    CONDITIONFLAG_BATONPASS = 0xE,
    CONDITIONFLAG_NULL = 0xF,
};

struct ConditionData
{
    u32 status : 3;
    u32 turncount : 3;
    u32 field7 : 3;
    u32 param : 3;
    u32 rest : 20;
};

extern "C" ConditionData MakeBasicStatus(MoveCondition condition);
extern "C" ConditionData Condition_MakeNull();
extern "C" u32 Condition_GetMonID(ConditionData condData);
extern "C" void Condition_SetMonID(ConditionData* condData, u32 pokemonSlot);
extern "C" u32 Condition_GetTurnMax(ConditionData condData);
extern "C" ConditionData Condition_MakeTurn(u32 turns);
extern "C" ConditionData Condition_MakePoke(u32 pokemonSlot);
extern "C" ConditionData Condition_MakeTurnParam(u32 maxTurns, u32 param);
extern "C" ConditionData Condition_MakePermanent();
extern "C" ConditionData MakeCondition(CONDITION condition, BattleMon* battleMon, ConditionData* condData);
extern "C" ConditionData ConditionData_MakePoke(u32 battleSlot);
extern "C" ConditionData Condition_MakeBadlyPoisoned();

#endif // __CONDITIONS_H
