#ifndef __SHORTCUT_MENU_H
#define __SHORTCUT_MENU_H

#include "defs.h"

#include "field/fieldmap.h"
#include "system/game_event.h"

enum ShortcutMenuType : u32
{
    SHORTCUT_ITEM = 0,
    SHORTCUT_POKEDEX = 1,
    SHORTCUT_BAG_POCKET = 2,
    SHORTCUT_TRAINER_CARD = 3,
    SHORTCUT_UNKNOWN_4 = 4,
    SHORTCUT_OPTIONS = 5,
    SHORTCUT_UNKNOWN_6 = 6,
    SHORTCUT_POKEINFO = 7,
    SHORTCUT_TOWN_MAP = 8,
    SHORTCUT_PAL_PAD = 9,
    SHORTCUT_UNKNOWN_10 = 10,
    SHORTCUT_UNKNOWN_11 = 11,
    SHORTCUT_VS_RECORDER = 12,
    SHORTCUT_XTRANSCEIVER = 13,
    SHORTCUT_MEDAL_BOX = 14,
};

typedef u32 FIELD_COMMON_EVENT;
enum FieldCommonEffectType : u32
{
    FIELDCOMEFF_BIKE = 0,
    FIELDCOMEFF_WARP = 1,
    FIELDCOMEFF_ESCAPE_ROPE = 2,
    FIELDCOMEFF_SWEET_SCENT = 3,
    FIELDCOMEFF_FISHING = 4,
    FIELDCOMEFF_DROWSING_MCHN = 5,
};

enum FieldCommonEventID : u32
{
    FLD_CMNEVENT_CYCLING = 0x0,
    FLD_CMNEVENT_ENTRALINK_WARP = 0x1,
    FLD_CMNEVENT_ESCAPE_ROPE = 0x2,
    FLD_CMNEVENT_SWEET_SCENT = 0x3,
    FLD_CMNEVENT_FISHING = 0x4,
    FLD_CMNEVENT_5 = 0x5,
};

enum ItemUseEventType : u32
{
    ITEMUSE_EVENT_CHECKAGAIN = 0x0,
    ITEMUSE_EVENT_CANCEL = 0x1,
    ITEMUSE_EVENT_HIDEN = 0x2,
    ITEMUSE_EVENT_FIELD = 0x3,
};

typedef u32 ACTION_ID;
enum ActionID : u32
{
    ACTID_BIKE = 0x0,
    ACTID_TOWN_MAP = 0x1,
    ACTID_PAL_PAD = 0x2,
    ACTID_ESCAPE_ROPE = 0x3,
    ACTID_UKN_4 = 0x4,
    ACTID_FISHING = 0x5,
    ACTID_VS_RECORDER = 0x6,
    ACTID_UKN_7 = 0x4,
    ACTID_UKN_8 = 0x4,
    ACTID_DOWSING_MCHN = 0x9,
    ACTID_XTRANSCEIVER = 0x10,
    ACTID_MEDAL_BOX = 0x11,
    ACTID_WARP_SWEETSCENT = 0xFF,
};

enum PokeInfoMenu : u32
{
    POKEINFO_STATUS = 0,
    POKEINFO_SKILLS = 1,
    POKEINFO_RIBBONS = 2,
};

enum PokeDexMenu : u32
{
    POKEDEX_NONE = 0,
    POKEDEX_LIST = 1,
    POKEDEX_INFO = 2,
    POKEDEX_AREA = 3,
    POKEDEX_CRY = 4,
    POKEDEX_FORMS = 5,
    POKEDEX_SEARCH = 6,
    POKEDEX_HABITAT_LIST = 7,
    POKEDEX_HABITAT_INFO = 8,
};

enum TrainerCardMenu : u32
{
    TRAINERCARD_NONE = 0,
    TRAINERCARD_FRONT = 1,
    TRAINERCARD_BACK = 2,
    TRAINERCARD_BADGES = 3,
};

struct ShortcutMenuWork
{
    GameSystem* gameSystem;
    Field* field;
    GameEvent* parentEvent;
    u32 field_C;
    ShortcutMenuType subProcID;
    u32 subProcParam;
    u32 menuRoutine1;
    u32 menuRoutine2;
    u32 field_20;
    void* parentEventData;
    ItemUseEventType eventType;
    FIELD_COMMON_EVENT fieldCommonEffectType;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
};

struct ShortcutSave
{
    u8 registableIDs[34];
    u16 registeredCount;
};


#endif // __SHORTCUT_MENU_H
