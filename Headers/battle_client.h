#ifndef __BATTLE_CLIENT_H
#define __BATTLE_CLIENT_H

#include "gfl/core/gfl_heap.h"

#include "defs.h"

#include "battle_action.h"

struct MainModule;
struct PokeCon;
struct BattleMon;
struct BtlRecWk;
struct BattleField;
struct BtlvCore;
struct Adapter;
struct ScriptVM;
struct BattleParty;

struct RecPlayerControl
{
	u8 field_0;
	u8 field_1;
	u8 field_2;
	u8 field_3;
	u16 field_4;
	u16 field_6;
	u16 field_8;
	u16 field_A;
	u16 field_C;
};

struct EscapeInfo
{
	u32 count;
	u8 clientID[4];
};

struct Btlv_StringParam
{
	u16 strID;
	u8 wait;
	u8 strTypeAndArgCount;
	u32 args[8];
};

struct PokeSelectParam
{
	BattleParty* party;
	u8 numSelect;
	u8 prohibit[6];
	u8 disabledPutPosSequence;
};

struct PokeSelectResult
{
	u8 selIdx[3];
	u8 outPokeIdx[3];
	u8 count;
	u8 field_7;
	u8 cancel;
};

struct SWAN_PACKED SWAN_ALIGNED(4) BtlClientWk
{
	MainModule* mainModule;
	PokeCon* pokeCon;
	BattleMon* procPoke;
	BattleActionParam* procAction;
	BtlRecWk* recData;
	u32 battleRecReader;
	RecPlayerControl recPlayer;
	SWAN_ALIGNED(4) u32 dword28;
	EscapeInfo escapeInfo;
	BattleField* battleField;
	u8 gap38[24];
	Adapter* adapter;
	BtlvCore* btlvCore;
	Btlv_StringParam strParam;
	u32 field_7C;
	Btlv_StringParam field_80;
	u8 gapA4[4];
	u32 cmdCheckServer;
	BattleMon* Party[6];
	u32 field_C4;
	u32 field_C8;
	u8 field_CC;
	u32(*field_D0)(BtlClientWk*, u32*) SWAN_ALIGNED(4);
	u32 field_D4;
	u32(*actionSelectProc)(BtlClientWk*, u32*);
	u32 actionSelectSeq;
	BattleActionParam* returnDataActionSelect;
	u32 returnDataActionSelectCount;
	u32 field_E8;
	u16 cmdLimitTime;
	u16 wordEE;
	u16 AIBagItems[4];
	ScriptVM* VM;
	u64 rand1;
	u64 rand2;
	u64 rand3;
	u8 field_114[6];
	u8 field_11A[4];
	SWAN_ALIGNED(4) BattleParty* actPokeParty;
	u8 myCoverPosNum;
	u8 procPokeIdx;
	u8 prevPokeIdx;
	u8 firstPokeIdx;
	u8 fStdMsgChanged;
	u8 field_129;
	u8 field_12A;
	SWAN_ALIGNED(2) BattleActionParam actionParam[3];
	u8 field_138[4];
	u32 cmdQueue;
	u32 serverCmdArgs;
	u8 gap144[60];
	u32 serverCmd;
	u32 serverCmd2;
	u32 serverCmdSeq;
	PokeSelectParam pokeSelectParam;
	PokeSelectResult pokeSelectResult;
	u8 field_1A1;
	HeapID TrainerMsgHeapID;
	u8 field_1A4;
	SWAN_ALIGNED(2) u16 field_1A6;
	u16 selectItemWork[3];
	u8 myID;
	u8 field_1AF;
	u8 gap1B0;
	u8 commWaitInfoOn;
	u8 field_1B2;
	u8 field_1B3;
	u8 field_1B4;
	u8 forceQuitActionSelect;
	u8 cmdCheckTimingCode;
	u8 currentActionCount;
	u8 moveInfoPokeIdx;
	u8 moveInfoMoveIdx;
	u8 actSelectFlags;
	u8 myChangePokeCount;
	u8 field_1BC;
	u8 field_1BD;
	u8 myChangePokePos[6];
	u8 field_1C4;
	u8 field_1C5;
	u8 field_1C6;
	u8 field_1C7;
	u8 field_1C8;
	u8 field_1C9;
	u8 field_1CA;
	SWAN_ALIGNED(2) u32 field_1CC;
	u32 field_1D0;
	u8 gap1D4[20];
	u16 field_1E8;
	u16 field_1EA;
	u32 field_1EC;
	u32 field_1F0;
	u8 gap1F4[16];
	u32 field_204;
	u8 gap208[12];
	u32 field_214;
	u8 gap218[4];
	u16 word21C;
	SWAN_ALIGNED(4) u32 dword220;
	u16 field_224[22];
	u8 field_250;
};

extern "C" b32 IsChapterSkipMode(BtlClientWk* btlClientWk);

extern "C" void BattleViewCmd_AddAnim(u32 animID);
extern "C" b32 BattleViewCmd_WaitEffect();
extern "C" void BattleViewCmd_StartMessageStandard(BtlvCore* btlvCore, int msgID, u32 narcIdx);
extern "C" b32 BattleViewCmd_WaitMessage(BtlvCore* btlvCore);
extern "C" b32 BattleViewCmd_AbilityPopup_DisplayWait(BtlvCore* btlvCore, u8 battlePos);
extern "C" void BattleViewCmd_AbilityPopup_DisplayStart(BtlvCore* btlvCore, u32 battlePos, u8 flag);
extern "C" void BattleViewCmd_ChangeForm_Start(BtlvCore * btlvCore, u32 viewPos);
extern "C" b32 BattleViewCmd_ChangeForm_Wait(BtlvCore * btlvCore);

extern "C" int BattleParty_FindPartyMon(BattleParty* battleParty, BattleMon* battleMon);

extern "C" void Btlv_StringParam_Setup(Btlv_StringParam* strParam, u8 narcIdx, u16 msgIdx);
extern "C" void Btlv_StringParam_AddArg(Btlv_StringParam* strParam, u32 value);

#endif // __BATTLE_CLIENT_H
