#ifndef __BATTLE_SETUP_H
#define __BATTLE_SETUP_H

#include "defs.h"

#include "battle_mon.h"
#include "trainer_data.h"

#include "system/game_data.h"

enum BtlType : u32
{
	BtlType_Wild = 0x0,
	BtlType_Trainer = 0x1,
	BtlType_Facility = 0x2,
	BtlType_Online = 0x3,
	BtlType_Demo = 0x4,
	BtlType_Max = 0x5,
};

enum BattleStyle : u32
{
	BTL_STYLE_SINGLE = 0x0,
	BTL_STYLE_DOUBLE = 0x1,
	BTL_STYLE_TRIPLE = 0x2,
	BTL_STYLE_ROTATION = 0x3,
};

struct SWAN_ALIGNED(2) BattleFieldStatus
{
	u32 battleBGID;
	u32 battlePedestalID;
	u8 btlWeather;
	u8 season;
	u16 zoneID;
	u8 hour;
	u8 minute;
	u8 field_E;
	u8 field_F;
};

struct TrainerBattleSetup
{
	u32 trainerID;
	u32 trainerClass;
	u32 trainerAIBits;
	u16 trainerItems[4];
	StrBuf* trainerName;
	u16 field_18[4];
	u16 field_20[4];
};

struct SWAN_ALIGNED(4) BtlSetup
{
	BtlType btlType;
	BattleStyle btlStyle;
	BattleFieldStatus fieldStatus;
	u16 battleBGMID;
	u16 winBGMID;
	u32 field_1C;
	u8 commMode;
	u8 field_21;
	u8 field_22;
	u8 field_23;
	PokeParty* parties[4];
	u32 playerInfo;
	u32 field_38;
	u32 field_3C;
	u32 field_40;
	u8 field_44[4];
	TrainerBattleSetup* trainerSetups[4];
	u32 field_58;
	u32 field_5C;
	u32 field_60;
	u32 field_64;
	u32 field_68;
	u32 field_6C;
	GameData* gameData;
	TrainerData* trainerData;
	BagSaveData* itemWork;
	u32 field_7C;
	u32 dex;
	u32 recordData;
	u32 field_88;
	u32 field_8C;
	u32 field_90;
	u8 field_94;
	u8 field_95;
	u8 field_96;
	u8 field_97;
	u8 badgeCount;
	u8 field_99;
	u8 field_9A;
	u8 field_9B;
	u32 field_9C;
	u16 field_A0;
	u16 field_A2;
	u32 earnedMoney;
	BattleResult battleResult;
	u8 capturedPkmIdx;
	u8 field_AD;
	u16 field_AE;
	void* field_B0;
	u32 field_B4;
	u32 field_B8;
	u32 field_BC;
	u32 field_C0;
	u32 field_C4;
	u32 field_C8;
	u32 field_CC;
	u16 HPSum;
	u8 currentTurn;
	u8 switchCount;
	u8 noEffectHits;
	u8 superEffectiveHits;
	u8 notVeryEffectiveHits;
	u8 opponentNoEffectHits;
	u8 opponentNotVeryEffectiveHits;
	u8 battlesWon;
	u8 opponentFainted;
	u8 allyFainted;
	u8 uniqueMovesUsed;
	u8 field_DD;
	u8 field_DE;
	u8 field_DF;
	u32 field_E0;
	u32 field_E4;
	u32 field_E8;
	u32 field_EC;
	u32 field_F0;
	u32 field_F4;
	u32 field_F8;
	u32 field_FC;
	u32 field_100[4];
	u8 field_110;
	u32 field_114;
	u32 field_118;
	u32 field_11C;
	u32 field_120;
	u32 field_124;
	u8 field_128;
	u8 field_129;
	u32 field_12C;
	u32 field_130;
	u32 field_134;
	u16 field_138;
	u16 field_13A;
	u8 keySystemLevelAdjust;
	u8 field_13D;
	u8 field_13E;
	u8 field_13F;
	u32 field_140;
	u32 field_144;
	u32 field_148;
};

extern "C" u32 GetSidePos(BattleStyle battleStyle, u8 attackingPos, u8 a3);
extern "C" b32 IsCenterInTripleBattle(u32 battlePos);

#endif // __BATTLE_SETUP_H