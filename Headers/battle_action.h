#ifndef __BATTLE_ACTION_H
#define __BATTLE_ACTION_H

#include "defs.h"

struct BattleAction_Fight
{
    u32 cmd : 4;
    u32 targetPos : 3;
    u32 moveID : 16;
    u32 pad : 9;
};

struct BattleAction_Item
{
    u32 cmd : 4;
    u32 targetIdx : 3;
    u32 itemID : 16;
    u32 param : 8;
    u32 pad : 1;
};

struct BattleAction_Switch
{
    u32 cmd : 4;
    u32 posIdx : 3;
    u32 memberIdx : 3;
    u32 depleteFlag : 1;
    u32 pad : 21;
};

struct BattleAction_Run
{
    u32 cmd : 4;
    u32 pad : 28;
};

struct BattleAction_Default
{
    u32 cmd : 4;
    u32 param : 28;
};

union BattleActionParam
{
    BattleAction_Fight baFight;
    BattleAction_Item baItem;
    BattleAction_Switch baSwitch;
    BattleAction_Run baRun;
    BattleAction_Default baDefault;
};

extern "C" u32 BattleAction_GetAction(BattleActionParam* param);

#endif // __BATTLE_ACTION_H
