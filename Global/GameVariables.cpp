#include "settings.h"
#include "GameVariables.h"

#if ADD_LEVEL_CAP

extern "C" u16 GetLvlCap() {
    EventWorkSave* eventWork = GameData_GetEventWork(GAME_DATA);
    u16* lvlCap = EventWork_GetWkPtr(eventWork, LVL_CAP_VAR);

    if (*lvlCap == 0)
        return 100;
    return *lvlCap;
}

#endif // ADD_LEVEL_CAP
