#ifndef __SERVER_EVENTS_H
#define __SERVER_EVENTS_H

#include "defs.h"

#include "server_flow.h"
#include "types.h"
#include "moves.h"

extern "C" u32 ServerEvent_CalculateSpeed(ServerFlow* serverFlow, BattleMon* battleMon, b32 IsTrickRoomEnabled);
extern "C" u32 ServerEvent_GetMovePower(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam);
extern "C" u32 ServerEvent_GetAttackPower(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam, u32 critFlag);
extern "C" u32 ServerEvent_GetTargetDefenses(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* targetMon, MoveParam* moveParam, u32 critFlag);
extern "C" u32 ServerEvent_GetWeather(ServerFlow* serverFlow);
extern "C" u32 ServerEvent_SameTypeAttackBonus(ServerFlow* serverFlow, BattleMon* attackingMon, PokeType moveType);
extern "C" b32 ServerEvent_CheckFloating(ServerFlow* serverFlow, BattleMon* battleMon, b32 checkFlyingType);
extern "C" void ServerEvent_GetMoveParam(ServerFlow* serverFlow, MOVE_ID moveID, BattleMon* battleMon, MoveParam* moveParam);
extern "C" void ServerEvent_ItemSetDecide(ServerFlow* serverFlow, BattleMon* battleMon, ITEM_ID itemID);
extern "C" void ServerEvent_ItemRewriteDone(ServerFlow* serverFlow, BattleMon* battleMon);
extern "C" void ServerEvent_CheckMultihitHits(ServerFlow* serverFlow, BattleMon* attackingMon, MOVE_ID moveID, HitCheckParam* params);
extern "C" void ServerEvent_NotifyAirLock(ServerFlow* serverFlow);
extern "C" void ServerEvent_ApplyDamageToRecover(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon);
extern "C" u32 ServerEvent_CheckDamageToRecover(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, MoveParam* moveParam);
extern "C" u32 ServerEvent_MoveConditionCheckFail(ServerFlow* serverFlow, BattleMon* attackingMon, BattleMon* defendingMon, CONDITION condition);
extern "C" void ServerEvent_AddConditionFailed(ServerFlow* serverFlow, BattleMon* defendingMon, BattleMon* attackingMon, CONDITION condition);
extern "C" u32 ServerEvent_MoveExecuteFail(ServerFlow* serverFlow, BattleMon* attackingMon, MOVE_ID moveID, u32 failCause);
extern "C" void ServerEvent_AbilityNullified(ServerFlow* serverFlow, BattleMon* battleMon);
extern "C" b32 ServerEvent_CheckHiding(ServerFlow * serverFlow, BattleMon * attackingMon, BattleMon * defendingMon);
extern "C" u32 ServerEvent_CheckChargeUpFail(ServerFlow * serverFlow, BattleMon * attackingMon, PokeSet * targetSet);
extern "C" u32 ServerEvent_CheckChargeUpSkip(ServerFlow * serverFlow, BattleMon * battleMon, MOVE_ID moveID);
extern "C" void ServerEvent_ChargeUpSkip(ServerFlow * serverFlow, BattleMon * attackingMon);
extern "C" b32 ServerEvent_ChargeUpRelease(ServerFlow * serverFlow, BattleMon * attackingMon, PokeSet * targetSet);
extern "C" void ServerEvent_CheckSideEffectParam(ServerFlow* serverFlow, u32 battleSlot, SIDE_EFFECT sideEffect, u32 side, ConditionData* condData);

#endif // __SERVER_EVENTS_H