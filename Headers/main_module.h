#ifndef __MAIN_MODULE_H
#define __MAIN_MODULE_H

#include "defs.h"

#include "battle_core.h"

struct MainModule;

struct PokeCon
{
    MainModule* mainModule;
    BattleParty party[4];
    PokeParty* srcParty[4];
    BattleMon* activeBattleMon[24];
    u32 forServer;
};

struct SVCL_WORK
{
    u32 adapter;
    BattleParty* party;
    u8 byte8;
    u8 byte9;
    u8 byteA;
    u8 ID;
};

struct SWAN_ALIGNED(4) BtlServerWk
{
    u32 field_0;
    u32 field_4;
    u8 field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    u8* mainModule;
    u32 pokeCon;
    SVCL_WORK client[4];
    ServerFlow* serverFlow;
    u32 serverTurnState;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u8 field_68;
    u8 gap69;
    u8 gap6A[62];
    u16 field_A8;
    u16 field_AA;
    StrBuf* field_AC;
    EscapeInfo* escapeInfo;
    u32 clientAction;
    u8 gapB8[40];
    u32 field_E0;
    u8 field_E4[4];
    u32 field_E8;
    u8 nextEnemyForSwitchMode;
    u8 field_ED;
    u8 field_EE;
    u8 field_EF;
    u32 field_F0;
    u8 gapF4[3004];
    u8* field_CB0;
    u8 field_CB4;
    u8 field_CB5;
    u8 field_CB6;
    u8 field_CB7;
    u8 field_CB8;
    u8 field_CB9;
    u8 field_CBA;
    u8 field_CBB;
    u8 field_CBC;
    u8 field_CBD;
    u8 field_CBE;
    u8 field_CBF;
    HeapID heapID;
    u8 field_CC2;
    u8 field_CC3;
};

struct SWAN_ALIGNED(32) MainModule
{
    BtlSetup* btlSetup;
    u32 field_4;
    BtlServerWk* server;
    SWAN_ALIGNED(8) BtlClientWk* clients[4];
    u32 field_20;
    u32 field_24;
    TrainerData trainerData[4];
    u8 gap78[55];
    u8 field_AF;
    u32 field_B0;
    u32 field_B4;
    u32 field_B8;
    u32 field_BC;
    u32 playerInfo;
    u8 field_C4[4];
    PokeCon pokeconClient;
    PokeCon pokeconServer;
    PokeParty* srcParty[4];
    PokeParty* srcPartyForServer[4];
    PokeParty* tempParty;
    u32 field_2BC;
    PartyPkm* TempPartyPkm;
    u32 field_2C4;
    u32 field_2C8;
    u32 field_2CC;
    u32 field_2D0;
    u32 field_2D4;
    u8 gap2D8[260];
    u32 field_3DC;
    u32 chatotChatterBlk;
    u32 field_3E4;
    u32 field_3E8;
    u32 field_3EC;
    u32 randomSeed;
    u32 field_3F4;
    u32 field_3F8;
    u32 field_3FC;
    u32 field_400;
    u32 field_404;
    u32 field_408;
    u32 field_40C;
    u32 field_410;
    u32 field_414;
    u32 field_418;
    u32 field_41C;
    u16 field_420;
    SWAN_PACKED SWAN_ALIGNED(1) u32 field_422;
    u8 field_426;
    u8 field_427;
    u8 field_428[6];
    u8 field_42E;
    u8 field_42F;
    u32 winMoney;
    u32 bonusMoney;
    u32 loseMoney;
    u32 field_43C;
    u16 field_440;
    u16 field_442;
    u32 serverResult;
    EscapeInfo escapeInfo;
    u32 subproc;
    u32 field_454;
    u32 field_458;
    u32 field_45C;
    u32 subseq;
    u32 field_464;
    u16 heapID;
    u8 field_46A;
    u8 field_46B;
    u8 playerClientID;
    u8 field_46D;
    u8 field_46E;
    u8 field_46F;
    u8 field_470;
    u8 field_471;
    u8 field_472;
    u8 field_473;
    u32 pokestarScriptNarcPtr;
    u32 pokestarScriptHeapPtr;
    u32 field_47C;
};

extern "C" BtlSetup* MainModule_GetBtlSetup(MainModule* main);
extern "C" BtlType MainModule_GetBattleType(MainModule* main);
extern "C" void* MainModule_GetPlayerStatus(MainModule* main);
extern "C" u32 MainModule_PokeIDToPokePos(MainModule* mainModule, PokeCon* pokeCon, u32 pokemonSlot);
extern "C" int MainModule_GetDebugFlag();
extern "C" b32 MainModule_IsAllyMonID(u32 slot1, u32 slot2);
extern "C" u32 MainModule_PokeIDToClientID(u32 battleSlot);
extern "C" u32 MainModule_BattlePosToViewPos(MainModule * mainModule, u32 battlePos);

extern "C" BtlType BtlSetup_GetBattleType(MainModule* main);
extern "C" BattleStyle BtlSetup_GetBattleStyle(MainModule* mainModule);

extern "C" u32 TrainerInfo_GetRegion(void* trainerInfo);
extern "C" b32 IsTrainerOT(BoxPkm* pkm, void* trainerInfo);

#endif // __MAIN_MODULE_H