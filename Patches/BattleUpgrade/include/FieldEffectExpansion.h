#ifndef __FIELD_EFFECT_EXPANSION_H
#define __FIELD_EFFECT_EXPANSION_H

#include "server_flow.h"

typedef u32 TERRAIN;
enum Terrain : u32
{
    TERRAIN_NULL = 0,
    TERRAIN_ELECTRIC = 1,
    TERRAIN_GRASSY = 2,
    TERRAIN_MISTY = 3,
    TERRAIN_PSYCHIC = 4,
    TERRAIN_NULLIFY = 5,
};

extern "C" u32 ServerEvent_IncreaseMoveTerrainTurns(ServerFlow * serverFlow, TERRAIN terrain, u32 attackingSlot);
extern "C" b32 ServerEvent_ChangeTerrain(ServerFlow * serverFlow, TERRAIN terrain);
extern "C" void ServerEvent_ChangeTerrainAfter(ServerFlow * serverFlow, TERRAIN terrain);
extern "C" TERRAIN ServerEvent_GetTerrain(ServerFlow * serverFlow);
extern "C" void ServerEvent_GroundedByGravity(ServerFlow * serverFlow, BattleMon * battleMon);

extern "C" b32 ServerControl_ChangeTerrainCheck(ServerFlow * serverFlow, TERRAIN terrain, u32 turns);
extern "C" u32 ServerControl_ChangeTerrain(ServerFlow * serverFlow, TERRAIN terrain, u32 turns);

extern "C" void CommonTerrainChangeAbility(ServerFlow * serverFlow, u32 pokemonSlot, TERRAIN terrain, u16 msgID);

extern "C" BattleEventHandlerTableEntry * EventAddFieldTerrain(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldDarkAura(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldFairyAura(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldVesselOfRuin(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldSwordOfRuin(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldTabletsOfRuin(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFieldBeadsOfRuin(u32 * handlerAmount);

extern "C" b32 CheckDisableEntryHazards(BattleMon * battleMon);
extern "C" b32 CheckUnseenFist(ServerFlow * serverFlow);

extern "C" BattleEventHandlerTableEntry * EventAddSideSpikesUpdated(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSideToxicSpikesUpdated(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSideStealthRockUpdated(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSideWideGuardUpdated(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSideQuickGuardUpdated(u32 * handlerAmount);

#endif // __FIELD_EFFECT_EXPANSION_H
