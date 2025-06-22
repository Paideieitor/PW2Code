#ifndef __BIG_BAG_STRUCT_TEMP_H
#define __BIG_BAG_STRUCT_TEMP_H
// THIS STRUCT IS STILL VERY UNDER RESEARCH, NAME IS TEMPORARY

#include "defs.h"

#include "items.h"
#include "msg_data.h"

#include "save/savedata_bag.h"

#define TCBManagerEx u32

struct GameData;
struct TCB;

struct SWAN_ALIGNED(4) PlayerActionPerms
{
    u8 char0;
    u8 gap1[3];
    PlayerExState playerSpecialState;
    u16 followFlagState;
    u32 field_C;
    u32 field_10;
    u8 actionsBlocked[12];
};

struct SWAN_ALIGNED(8) Bag
{
    GameData* gameData;
    BagSaveData* bagSaveData_4;
    void* void_ptr_8;
    PlayerActionPerms* playerActionPerms;
    u32 field_10;
    u32 field_14;
    BagSaveData* bagSaveData;
    u8 gap1C[8];
    BagSaveData* bagSaveData_24;
    BagItem bagitem_ptrs_28[3];
    u8 field_34[1228];
    u32(*function_4FC)(u32);
    u8* u8_ptr_504;
    u16 field_508;
    u8 field_506[10];
    u32 field_510;
    u32* u32_ptr_514;
    u32 field_518;
    MsgData* msgData;
    WordSetSystem* wordSetSystem;
    StrBuf* strBuf1;
    StrBuf* strBuf2;
    StrBuf* strBuf_530;
    u32 int_534;
    u8 field_538[20];
    u16 heapID;
    u8 field_54E[342];
    u32* dword_ptr_6A4;
    TCB* tcb_6A8;
    u8 field_6AC[12];
    u32 u32_6B8;
    u8 field_6BC[24];
    u32 array_6D4[3];
    u32 array_6E0[17];
    u8 field_724[16];
    u32 int_734;
    u8 field_738[16];
    TCBManagerEx* tCBManagerEx;
    u8* u8_ptr_74C;
    u8 field_750[76];
    u32* buttonClickRelated;
    u16 field_7A0;
    u8 field_7A2[93];
    u8 field_7FF;
    u32 field_800;
    u8 field_804[40];
    ItemType pocketType_82C;
    u32 u32_830;
    u32 u32_834;
    u32* ptr_838;
    int field_83C;
    u8 gap840[8];
    u32 BIAArraySizeUnkn[20];
    u32 u32_898;
    u32 bagItemActions;
    ItemID itemID_8A0;
    u8 field_8A4[8];
    b32 bool_8AC;
    u8 field_8B0[16];
    u32 field_8C0;
    u32 field_8C4;
    u32 field_8C8;
    u8 field_8CC[456];
    u32 itemID_A94;
    u8 field_A98[5304];
    u32 field_1F50;
};

extern "C" int Bag_LoadItemName(Bag* bag, u32 narcIdx, u32 itemID);
extern "C" void Bag_CreateTextBox(Bag* bag, int a2);
extern "C" b32 IsSkipDialogueKeyPressed(Bag* bag, int a2);
extern "C" int SkipDialogue(int a1);

#endif // __BIG_BAG_STRUCT_TEMP_H
