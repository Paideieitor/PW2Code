#ifndef __LEVEL_CAP_H
#define __LEVEL_CAP_H

#include "settings.h"

#include "defs.h"

#include "save/savedata_eventwork.h"
#include "system/game_data.h"

#if ADD_LEVEL_CAP

extern "C" EventWorkSave * GameData_GetEventWork(GameData* gameData);
extern "C" u16 * EventWork_GetWkPtr(EventWorkSave* eventWork, int swkId);

extern "C" u16 GetLvlCap();

#endif // ADD_LEVEL_CAP

#endif // __LEVEL_CAP_H

