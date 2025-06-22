#ifndef __ACTION_ORDER_H
#define __ACTION_ORDER_H

#include "defs.h"

#include "battle_mon.h"
#include "battle_action.h"

struct SWAN_ALIGNED(4) ActionOrderWork
{
    BattleMon* battleMon;
    BattleActionParam action;
    u32 speed;
    u8 partyID;
    u8 done;
    u8 field_E;
    u8 field_F;
};

#define ACTION_ORDER_GET_PRIO(actionOrder, idx) (actionOrder[idx].speed >> 16) & 0x3FFFFF
#define ACTION_ORDER_GET_SPECIAL_PRIO(actionOrder, idx) ((actionOrder[idx].speed >> 13) & 0x7)

#endif //__ACTION_ORDER_H
