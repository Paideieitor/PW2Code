#ifndef __SCRIPT_H
#define __SCRIPT_H

#include "defs.h"

#include "msg_data.h"

#include "field/field_mmodel.h"

enum FieldBattleType : u32
{
	FLD_BTLTYPE_SINGLE = 0x0,
	FLD_BTLTYPE_DOUBLE_SEPARATE = 0x1,
	FLD_BTLTYPE_DOUBLE_UNIFIED = 0x2,
	FLD_BTLTYPE_DOUBLE_MULTI = 0x3,
};

struct FieldActorAnmProc;
struct StadiumTrainer;
struct ScriptSubwork;

struct ScrMapWk
{
	u32 msgWork;
	Field* field;
};

struct FieldScriptTrainerSetup
{
	FieldActor* trainerActor;
	u32 clashDistance;
	u32 clashDirection;
	u32 scriptID;
	u32 trainerID;
	FieldBattleType battleType;
};

struct EventTrainerEye;

struct FieldScriptTrainerWork
{
	FieldScriptTrainerSetup setup;
	EventTrainerEye* eventTrainerEye;
};

struct EventTrainerEye
{
	u32 state;
	b32 allDone;
	FieldScriptTrainerWork scriptTrainerWork;
	u32 trainerSlot;
	u32 waitCounter;
	FieldTerrainEffectTCB* terrainEffectTCB;
	Field* field;
	FieldPlayer* fieldPlayer;
};

struct ScriptWork
{
	u32 magic;
	u16 SCRID;
	u16 paddingSCRID;
	FieldActor* executorActor;
	u16 heapID;
	u16 paddingHeapID;
	GameSystem* gameSys;
	GameEvent* scriptCallEvent;
	ScrMapWk mapWk;
	u32 field_20;
	u32 isReducedFeatureSetVM;
	u32 cmdFeatureSetLevel;
	WordSetSystem* wordSetBuffers;
	StrBuf* strbuf1;
	StrBuf* strbuf2;
	u32 field_38;
	void* userHeap;
	u8 SEBitMask;
	u8 field_41;
	u16 field_42;
	FieldScriptTrainerWork trainerWks[2];
	u16 scriptVariables[1][98];
	FieldActorAnmProc* actorAnimationWk;
	StadiumTrainer* stadiumTrainers;
	ScriptSubwork* subWork;
};

struct SWAN_ALIGNED(4) FieldCommonEvent
{
	GameEvent* (*function)(Field*, GameSystem*);
	u32 actionID;
};

struct SWAN_ALIGNED(4) EventFieldToggle
{
	GameSystem* gameSystem;
	Field* field;
};

#endif // __SCRIPT_H
