#ifndef __HANDLER_PARAMS_H
#define __HANDLER_PARAMS_H

#include "defs.h"

#include "stat_stage.h"
#include "conditions.h"
#include "field_effects.h"
#include "side_effects.h"
#include "turn_flag.h"
#include "counters.h"

typedef u32 ABILITY;

struct ServerFlow;

enum BattleHandlerEffect : u32
{
    EFFECT_USE_HELD_ITEM = 0x0,
    EFFECT_USE_HELD_ITEM_ANIMATION = 0x1,
    EFFECT_ABILITY_POPUP_ADD = 0x2,
    EFFECT_ABILITY_POPUP_REMOVE = 0x3,
    EFFECT_MESSAGE = 0x4,
    EFFECT_RECOVER_HP = 0x5,
    EFFECT_DRAIN = 0x6,
    EFFECT_DAMAGE = 0x7,
    EFFECT_CHANGE_HP = 0x8,
    EFFECT_RESTORE_PP = 0x9,
    EFFECT_DECREMENT_PP = 0xA,
    EFFECT_CURE_STATUS = 0xB,
    EFFECT_ADD_CONDITION = 0xC,
    EFFECT_CHANGE_STAT_STAGE = 0xE,
    EFFECT_SET_STAT_STAGE = 0xF,
    EFFECT_RESET_STAT_STAGE = 0x10,
    EFFECT_SET_STAT = 0x11,
    EFFECT_RESTORE_STAT_STAGE = 0x12,
    EFFECT_FAINT = 0x13,
    EFFECT_CHANGE_TYPE = 0x14,
    EFFECT_SET_TURN_FLAG = 0x15,
    EFFECT_RESET_TURN_FLAG = 0x16,
    EFFECT_SET_CONDITION_FLAG = 0x17,
    EFFECT_RESET_CONDITION_FLAG = 0x18,
    EFFECT_ADD_SIDE_EFFECT = 0x19,
    EFFECT_REMOVE_SIDE_EFFECT = 0x1A,
    EFFECT_ADD_FIELD_EFFECT = 0x1B,
    EFFECT_REMOVE_FIELD_EFFECT = 0x1C,
    EFFECT_CHANGE_WEATHER = 0x1D,
    EFFECT_ADD_POS_EFFECT = 0x1E,
    EFFECT_CHANGE_ABILITY = 0x1F,
    EFFECT_SET_HELD_ITEM = 0x20,
    EFFECT_CHECK_HELD_ITEM = 0x21,
    EFFECT_FORCE_USE_HELD_ITEM = 0x22,
    EFFECT_CONSUME_ITEM = 0x23,
    EFFECT_SWAP_ITEM = 0x24,
    EFFECT_UPDATE_MOVE = 0x25,
    EFFECT_COUNTER = 0x26,
    EFFECT_DELAY_MOVE_DAMAGE = 0x27,
    EFFECT_QUIT_BATTLE = 0x28,
    EFFECT_SWITCH = 0x29,
    EFFECT_BATON_PASS = 0x2A,
    EFFECT_FLINCH = 0x2B,
    EFFECT_REVIVE = 0x2C,
    EFFECT_SET_WEIGHT = 0x2D,
    EFFECT_FORCE_SWITCH = 0x2E,
    EFFECT_INTERRUPT_ACTION = 0x2F,
    EFFECT_INTERRUPT_MOVE = 0x30,
    EFFECT_SEND_LAST = 0x31,
    EFFECT_SWAP_POKE = 0x32,
    EFFECT_TRANSFORM = 0x33,
    EFFECT_ILLUSION_BREAK = 0x34,
    EFFECT_GRAVITY_CHECK = 0x35,
    EFFECT_CANCEL_SEMI_INVULN = 0x36,
    EFFECT_ADD_ANIMATION = 0x37,
    EFFECT_REMOVE_MESSAGE_WINDOW = 0x38,
    EFFECT_CHANGE_FORM = 0x39,
    EFFECT_SET_ANIMATION_ID = 0x3A,
    EFFECT_PLAY_ANIMATION = 0x3B,
};

struct HandlerParam_StrParams
{
    u16 ID;
    u16 flags;
    u32 subProcID;
    u32 args[8];
};

struct HandlerParam_Header
{
    u32 flags;
};

struct HandlerParam_InterruptPoke
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_SendLast
{
    HandlerParam_Header header;
    u8 pokeID;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_Message
{
    HandlerParam_Header header;
    HandlerParam_StrParams str;
};

struct HandlerParam_CureCondition
{
    HandlerParam_Header header;
    CONDITION condition;
    u8 pokeID[12];
    u8 pokeCount;
    u8 msgDisable;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_RecoverHP
{
    HandlerParam_Header header;
    u16 recoverHP;
    u8 pokeID;
    u8 failCheckThru;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_AddFieldEffect
{
    HandlerParam_Header header;
    FIELD_EFFECT effect;
    ConditionData condData;
    u8 addDependPoke;
    u8 field_D;
    u8 field_E;
    u8 field_F;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_RemoveFieldEffect
{
    HandlerParam_Header header;
    FIELD_EFFECT effect;
};

struct SWAN_ALIGNED(4) HandlerParam_ChangeType
{
    HandlerParam_Header header;
    u16 pokeType;
    u8 pokeID;
    u8 field_7;
};

struct HandlerParam_SwapItem
{
    HandlerParam_Header header;
    u8 pokeID;
    u8 incRecordCount;
    HandlerParam_StrParams exStr;
    HandlerParam_StrParams exSubStr1;
    HandlerParam_StrParams exSubStr2;
};

struct HandlerParam_ChangeStatStage
{
    HandlerParam_Header header;
    StatStage stat;
    u32 pad;
    s8 volume;
    u8 pad2;
    u8 moveAnimation;
    u8 pokeCount;
    u8 pokeID[6];
    HandlerParam_StrParams exStr;
};

struct HandlerParam_ChangeForm
{
    HandlerParam_Header header;
    u8 pokeID;
    u8 newForm;
    u8 dontResetOnSwitch;
    u8 pad;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_AddCondition
{
    HandlerParam_Header header;
    CONDITION condition;
    ConditionData condData;
    u8 almost;
    u8 reserved;
    u8 overwriteMode;
    u8 pokeID;
    u8 overwriteMode2;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_Damage
{
    HandlerParam_Header header;
    u16 damage;
    u8 pokeID;
    u8 flags;
    u16 effectNo;
    u8 posFrom;
    u8 posTo;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_ChangeAbility
{
    HandlerParam_Header header;
    u16 ability;
    u8 pokeID;
    u8 sameAbilityEffective;
    u8 skipSwitchInEvent;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_ChangeWeather
{
    HandlerParam_Header header;
    u8 weather;
    u8 turn;
    u8 airLock;
    u8 field_7;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_Switch
{
    HandlerParam_Header header;
    HandlerParam_StrParams preStr;
    HandlerParam_StrParams exStr;
    u8 pokeID;
    u8 intrDisable;
};

struct HandlerParam_SetItem
{
    HandlerParam_Header header;
    u16 itemID;
    u8 pokeID;
    u8 clearConsume;
    u8 clearConsumeOtherPoke;
    u8 clearConsumePokeID;
    u8 callConsumedEvent;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_SwapPoke
{
    HandlerParam_Header header;
    u8 pokeID1;
    u8 pokeID2;
    HandlerParam_StrParams exStr;
};

struct HandlerParam_PP
{
    HandlerParam_Header header;
    u8 volume;
    u8 pokeID;
    u8 moveSlot;
    u8 flags;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_RemoveSideEffect
{
    HandlerParam_Header header;
    u8 flags[3];
    u8 side;
};

struct SWAN_ALIGNED(4) HandlerParam_SetConditionFlag
{
    HandlerParam_Header header;
    CONDITION_FLAG flag;
    u8 pokeID;
};

struct HandlerParam_ActivateItem
{
    HandlerParam_Header header;
    u8 pokeID;
    u8 ateBerry;
    u16 itemID;
};

struct HandlerParam_AddSideEffect
{
    HandlerParam_Header header;
    SIDE_EFFECT sideEffect;
    ConditionData condData;
    u8 side;
    HandlerParam_StrParams exStr;
};

struct SWAN_ALIGNED(4) HandlerParam_SetTurnFlag
{
    HandlerParam_Header header;
    TurnFlag flag;
    u8 pokeID;
};

struct SWAN_ALIGNED(2) HandlerParam_SetCounter
{
    HandlerParam_Header header;
    u8 pokeID;
    COUNTER counterID;
    u8 value;
};

struct SWAN_ALIGNED(4) HandlerParam_ConsumeItem
{
    HandlerParam_Header header;
    u32 dontUse;
    HandlerParam_StrParams exStr;
};

extern "C" HandlerParam_Header* BattleHandler_PushWork(ServerFlow* serverFlow, BattleHandlerEffect battleHandler, u32 currentSlot);
extern "C" void BattleHandler_PopWork(ServerFlow* serverFlow, void* handlerParms);
extern "C" void BattleHandler_PushRun(ServerFlow* serverFlow, BattleHandlerEffect effect, u32 pokemonSlot);
extern "C" void BattleHandler_StrSetup(HandlerParam_StrParams* params, u8 NARCID, u16 msgID);
extern "C" void BattleHandler_AddArg(HandlerParam_StrParams* params, u32 value);
extern "C" b32 BattleHandler_SetString(ServerFlow* serverFlow, HandlerParam_StrParams* params);
extern "C" void BattleHandler_StrClear(HandlerParam_StrParams* strParams);
extern "C" b32 BattleHandler_StrIsEnabled(HandlerParam_StrParams* strParam);
extern "C" u32 BattleHandler_Result(ServerFlow* serverFlow);

extern "C" b32 Handler_GetFightEnableBenchPokeNum(ServerFlow* serverFlow, u32 pokemonSlot);
extern "C" b32 Handler_CheckReservedMemberChangeAction(ServerFlow* serverFlow);

#endif // __HANDLER_PARAMS_H
