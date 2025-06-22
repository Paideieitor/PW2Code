#ifndef __ACTION_ORDER_MANAGER_H
#define __ACTION_ORDER_MANAGER_H

#include "server_flow.h"

extern "C" b32 CheckEndTurnSwitchFlag();
extern "C" void SetEndTurnSwitchFlag();
extern "C" void ResetEndTurnSwitchFlag();

extern "C" u32 CommonGetAllyPos(ServerFlow * serverFlow, u32 battlePos);

extern "C" void ShiftExtraActionOrders();
extern "C" ActionOrderWork* GetExtraActionOrder(u32 actionIdx);

extern "C" b32 CheckExtraActionFlag();
extern "C" void SetExtraActionFlag();
extern "C" void ResetExtraActionFlag();

#endif // __ACTION_ORDER_MANAGER_H
