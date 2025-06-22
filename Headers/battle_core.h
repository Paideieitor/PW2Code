#ifndef __BATTLE_CORE_H
#define __BATTLE_CORE_H

#include "battle_client.h"
#include "battle_setup.h"

struct GFLFont;
struct PokeParty;
struct PalAnm;
struct TCBManagerEx;
struct BtlvScu;
struct BtlvScd;

struct BtlvInput_MoveSelect_2
{
	BattleMon* activeMons[3];
	u32 field_C[3][4];
};

struct SWAN_PACKED SWAN_ALIGNED(1) BottomScreenMenuData
{
	u32 field_0;
	PokeParty* pokeParty;
	PokeParty* allyPokeParty;
	GFLFont* font;
	u16 heapID;
	u16 field_12;
	BattleStyle battleStyle;
	u32 isBagDisabled;
	u8 field_1C;
	u8 selectedPokeIndex[2];
	u8 field_1F;
	u8 selectedMon;
	u8 infoPokeIndex;
	u8 numPartyMembers;
	u8 field_23;
	u16 selectedItem;
	u16 isSelectedMonTrapped;
	TCBManager* tcbManager;
	PalAnm* palAnm;
	u32 IsFinished;
	u32 field_34;
	u32 field_38;
	u32 field_3C;
	u32 field_40;
	u32 field_44;
	u8 field_48[3];
	u8 moveInfoMoveIndex;
	u8 field_4C;
};

struct BtlvInput_VsRecorder
{
	u32 field_0;
	u32 wordSetNumber;
	u32 field_8;
	u32 field_C;
};

struct SWAN_PACKED SWAN_ALIGNED(2) BtlvCore
{
	MainModule* mainModule;
	BtlClientWk* client;
	PokeCon* pokeCon;
	u8 myID;
	u8 gap_D[3];
	u32 field_10;
	bool(*func14)(BtlvCore*, u32*);
	u32 field_18;
	u32(*func_1C)(u32*, BtlvCore*);
	u32(*func_20)(u32*, BtlvCore*);
	BtlvCore* btlCore_24;
	u32 field_28;
	BtlvInput_MoveSelect_2 field_2C;
	u8 gap_68[68];
	StrBuf* strBuf;
	GFLFont* font1;
	GFLFont* font2;
	BattleActionParam* actionParam;
	BattleMon* activeMon;
	u32 activeMonID;
	u32 field_C4;
	u32 field_C8;
	GFLFont* itemSelectFont;
	u32 field_D0;
	u16 itemSelectHeapID;
	u16 field_D6;
	u32 field_D8;
	u8 field_DC;
	u8 field_DD;
	u16 selectedItem;
	u8 field_E0;
	u8 field_E1;
	u8 field_E2;
	u8 field_E3;
	u32 field_E4;
	u32 field_E8;
	u32 field_EC;
	u8 ballCatchMode;
	u8 isWildBattle;
	u8 field_F2;
	u8 field_F3;
	u32 field_F4;
	u32 field_F8;
	u32 field_FC;
	u32 field_100;
	u32 field_104;
	u32 isOnlineBattle;
	BottomScreenMenuData bottomScreenMenuData;
	u8 field_159;
	u8 gap15A[2];
	GFLFont* field_15C;
	u16 heapID_160;
	u8 field_162;
	u8 field_163;
	PokeParty* pokeParty_164;
	u32 field_168;
	u32 field_16C;
	u32 field_170;
	u32 field_174;
	GFLFont* bottomScreenMenuFont;
	u16 bottomScreenMenuHeapID;
	u8 field_17E;
	SWAN_ALIGNED(2) u32 field_180;
	u32 field_184[4];
	u32 field_194;
	u32 field_198;
	PokeSelectResult* pokeSelectResult;
	BtlvInput_VsRecorder btlvInput_VsRecorder;
	BattleMon** party;
	u8 bottomScreenMenuSeq;
	u8 field_1B5;
	u8 field_1B6;
	u8 field_1B7;
	u8 field_1B8;
	u8 field_1B9;
	u8 field_1BA;
	u8 field_1BB;
	TCBManagerEx* callbackMgr;
	BtlvScu* btlvScu;
	BtlvScd* btlvScd;
	u16 heapID;
};

#endif // __BATTLE_CORE_H
