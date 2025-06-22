#include "settings.h"
#include "QoLItems.h"

#include "GameVariables.h"

#include "items.h"
#include "shortcut_menu.h"
#include "encounter.h"
#include "msg_data.h"
#include "poke_list.h"
#include "poke_party.h"
#include "moves.h"
#include "species.h"
#include "game_data.h"
#include "script.h"
#include "bag.h"

#if EXPAND_ITEMS

extern "C" BagItem* BagSave_AddItemCore(BagSaveData* bag, u16 item_idx, u16 quantity, HeapID heapId);
extern "C" bool THUMB_BRANCH_BagSave_AddItem(BagSaveData* bag, u16 itemId, u16 quantity, HeapID heapId)
{
    int amount = 50;
//#define ITEM_BOOSTER_ENERGY 440
    BagSave_AddItemCore(bag, 440, amount, heapId);

    return BagSave_AddItemCore(bag, itemId, quantity, heapId) != 0;
}

extern "C" ItemType THUMB_BRANCH_PML_ItemGetType(u16 itemID) {
    return (ItemType)GetItemParam(itemID, ITSTAT_POCKET_FIELD, 0x1/*HEAPID_USER*/);
}

#endif

#if ADD_INFINITE_RARE_CANDY || ADD_INFINITE_REPEL

// [ARM9:02092A24] Extended list of items that can be registered and their respective RegisterID
u16 ExtRegistrableItems[] = {
    REG_ITEM(REGIT_BIKE, ITEM_BIKE),
    REG_ITEM(REGIT_SUPER_ROD, ITEM_SUPER_ROD),
    REG_ITEM(REGIT_TOWN_MAP, ITEM_TOWN_MAP),
    REG_ITEM(REGIT_VS_RECORDER, ITEM_VS_RECORDER),
    REG_ITEM(REGIT_PAL_PAD, ITEM_PAL_PAD),
    REG_ITEM(REGIT_DOWSING_MCHN, ITEM_DOWSING_MACHINE),
    REG_ITEM(REGIT_GRACIDEA, ITEM_GRACIDEA),
    REG_ITEM(REGIT_XTRANSCEIVER, ITEM_XTRANSCEIVER_M),
    REG_ITEM(REGIT_XTRANSCEIVER, ITEM_XTRANSCEIVER_F),
    REG_ITEM(REGIT_MEDAL_BOX, ITEM_MEDAL_BOX),
    REG_ITEM(REGIT_DNA_SPLICERS_1, ITEM_DNA_SPLICERS_1),
    REG_ITEM(REGIT_DNA_SPLICERS_2, ITEM_DNA_SPLICERS_2),
    REG_ITEM(REGIT_REVEAL_GLASS, ITEM_REVEAL_GLASS),
#if ADD_INFINITE_RARE_CANDY
    REG_ITEM(REGIT_INFINITE_CANDY, ITEM_INFINITE_CANDY),
#endif
#if ADD_INFINITE_REPEL
    REG_ITEM(REGIT_INFINITE_REPEL, ITEM_INFINITE_REPEL),
#endif
};

// Checks if an item can be registered and returns its RegisterID if it does
extern "C" REG_ID THUMB_BRANCH_GetRegistrableID(ItemID itemID) {
    for (u16 i = 0; i < ARRAY_COUNT(ExtRegistrableItems); ++i) {
        u16 regItem = ExtRegistrableItems[i];

        if (itemID == (regItem & 0x3FF)) {
            return regItem >> 10;
        }
    }

    return REGIT_NOT_REGISTRABLE;
}

// Fills the data to trigger the registered item effect
extern "C" b32 THUMB_BRANCH_ShortcutMenu_SetKeyItemID(ShortcutMenuWork* shrotcutMenu, REG_ID registrableID) {
    // A common event is an event that triggers an effect in the overworld
    // This events are listed in a 6 long array here -> [OVL_12:0216CE24]
    b32 isCommonEvent;

    switch (registrableID) {
    case REGIT_BIKE:
        shrotcutMenu->fieldCommonEffectType = FIELDCOMEFF_BIKE;
        isCommonEvent = 1;
        break;
    case REGIT_TOWN_MAP:
        shrotcutMenu->subProcID = SHORTCUT_TOWN_MAP;
        isCommonEvent = 0;
        break;
    case REGIT_VS_RECORDER:
        shrotcutMenu->subProcID = SHORTCUT_VS_RECORDER;
        isCommonEvent = 0;
        break;
    case REGIT_PAL_PAD:
        shrotcutMenu->subProcID = SHORTCUT_PAL_PAD;
        isCommonEvent = 0;
        break;
    case REGIT_SUPER_ROD:
        shrotcutMenu->fieldCommonEffectType = FIELDCOMEFF_FISHING;
        isCommonEvent = 1;
        break;
    case REGIT_DOWSING_MCHN:
        shrotcutMenu->fieldCommonEffectType = FIELDCOMEFF_DROWSING_MCHN;
        isCommonEvent = 1;
        break;
    case REGIT_GRACIDEA:
        shrotcutMenu->subProcID = SHORTCUT_ITEM;
        shrotcutMenu->subProcParam = ITEM_GRACIDEA;
        isCommonEvent = 0;
        break;
    case REGIT_DNA_SPLICERS_1:
        shrotcutMenu->subProcID = SHORTCUT_ITEM;
        shrotcutMenu->subProcParam = ITEM_DNA_SPLICERS_1;
        isCommonEvent = 0;
        break;
    case REGIT_DNA_SPLICERS_2:
        shrotcutMenu->subProcID = SHORTCUT_ITEM;
        shrotcutMenu->subProcParam = ITEM_DNA_SPLICERS_2;
        isCommonEvent = 0;
        break;
    case REGIT_POKEINFO_STATUS:
        shrotcutMenu->subProcID = SHORTCUT_POKEINFO;
        shrotcutMenu->subProcParam = POKEINFO_STATUS;
        isCommonEvent = 0;
        break;
    case REGIT_POKEINFO_SKILLS:
        shrotcutMenu->subProcID = SHORTCUT_POKEINFO;
        shrotcutMenu->subProcParam = POKEINFO_SKILLS;
        isCommonEvent = 0;
        break;
    case REGIT_POKEINFO_RIBBONS:
        shrotcutMenu->subProcID = SHORTCUT_POKEINFO;
        shrotcutMenu->subProcParam = POKEINFO_RIBBONS;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_ITEMS:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = ITEMTYPE_STD;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_MEDICINE:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = ITEMTYPE_MEDICINE;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_TMS_HMS:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = ITEMTYPE_TM;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_BERRIES:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = ITEMTYPE_BERRY;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_KEYITEMS:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = ITEMTYPE_KEY;
        isCommonEvent = 0;
        break;
    case REGIT_BAG_FREESPACE:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = ITEMTYPE_FREE_SPACE;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_LIST:
        shrotcutMenu->subProcID = SHORTCUT_BAG_POCKET;
        shrotcutMenu->subProcParam = POKEDEX_LIST;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_SEARCH:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_SEARCH;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_INFO:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_INFO;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_AREA:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_AREA;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_CRY:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_CRY;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEX_FORMS:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_FORMS;
        isCommonEvent = 0;
        break;
    case REGIT_TRAINERCARD_FRONT:
        shrotcutMenu->subProcID = SHORTCUT_TRAINER_CARD;
        shrotcutMenu->subProcParam = TRAINERCARD_FRONT;
        isCommonEvent = 0;
        break;
    case REGIT_TRAINERCARD_BACK:
        shrotcutMenu->subProcID = SHORTCUT_TRAINER_CARD;
        shrotcutMenu->subProcParam = TRAINERCARD_BACK;
        isCommonEvent = 0;
        break;
    case REGIT_TRAINERCARD_BADGES:
        shrotcutMenu->subProcID = SHORTCUT_TRAINER_CARD;
        shrotcutMenu->subProcParam = TRAINERCARD_BADGES;
        isCommonEvent = 0;
        break;
    case REGIT_OPTIONS:
        shrotcutMenu->subProcID = SHORTCUT_OPTIONS;
        isCommonEvent = 0;
        break;
    case REGIT_XTRANSCEIVER:
        shrotcutMenu->subProcID = SHORTCUT_XTRANSCEIVER;
        isCommonEvent = 0;
        break;
    case REGIT_MEDAL_BOX:
        shrotcutMenu->subProcID = SHORTCUT_MEDAL_BOX;
        isCommonEvent = 0;
        break;
    case REGIT_POKEMON_LIST:
        shrotcutMenu->subProcID = SHORTCUT_ITEM;
        shrotcutMenu->subProcParam = 0xFFFFFFFF;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEXHABITAT_LIST:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_HABITAT_LIST;
        isCommonEvent = 0;
        break;
    case REGIT_POKEDEXHABITAT_INFO:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        shrotcutMenu->subProcParam = POKEDEX_HABITAT_INFO;
        isCommonEvent = 0;
        break;
    case REGIT_REVEAL_GLASS:
        shrotcutMenu->subProcID = SHORTCUT_ITEM;
        shrotcutMenu->subProcParam = ITEM_REVEAL_GLASS;
        isCommonEvent = 0;
        break;
#if ADD_INFINITE_RARE_CANDY
    case REGIT_INFINITE_CANDY:
        shrotcutMenu->subProcID = SHORTCUT_ITEM;
        shrotcutMenu->subProcParam = ITEM_INFINITE_CANDY;
        isCommonEvent = 0;
        break;
#endif
#if ADD_INFINITE_REPEL
    case REGIT_INFINITE_REPEL:
        shrotcutMenu->fieldCommonEffectType = FIELDCOMEFF_REPEL;
        isCommonEvent = 1;
        break;
#endif
    default:
        shrotcutMenu->subProcID = SHORTCUT_POKEDEX;
        isCommonEvent = 0;
    }

    return isCommonEvent;
}

// Returns data from the a Registrable ID, if the function returns 0 with isNotRegistrable being 0 the item loads the Pokemon List overlay
extern "C" b32 THUMB_BRANCH_ShortcutMenu_GetActionFromKeyItem(REG_ID registrableID, ACTION_ID* actionID, b32* isNotRegistrable) {
    *isNotRegistrable = 0;

    switch (registrableID) {
    case REGIT_BIKE:            *actionID = ACTID_BIKE;            break;
    case REGIT_TOWN_MAP:        *actionID = ACTID_TOWN_MAP;        break;
    case REGIT_VS_RECORDER:     *actionID = ACTID_VS_RECORDER;     break;
    case REGIT_PAL_PAD:         *actionID = ACTID_PAL_PAD;         break;
    case REGIT_SUPER_ROD:       *actionID = ACTID_FISHING;         break;
    case REGIT_DOWSING_MCHN:    *actionID = ACTID_DOWSING_MCHN;    break;
    case REGIT_XTRANSCEIVER:    *actionID = ACTID_XTRANSCEIVER;    break;
    case REGIT_MEDAL_BOX:       *actionID = ACTID_MEDAL_BOX;       break;
#if ADD_INFINITE_REPEL
    case REGIT_INFINITE_REPEL:  *actionID = ACTID_IGNORE;          break;
#endif
    case REGIT_GRACIDEA:
    case REGIT_DNA_SPLICERS_1:
    case REGIT_DNA_SPLICERS_2:
    case REGIT_REVEAL_GLASS:
#if ADD_INFINITE_RARE_CANDY
    case REGIT_INFINITE_CANDY:
#endif
        return 0;
    case REGIT_NOT_REGISTRABLE:
    default:
        *isNotRegistrable = 1;
        return 0;
    }

    return 1;
}

#endif // ADD_INFINITE_RARE_CANDY || ADD_INFINITE_REPEL

#if ADD_INFINITE_RARE_CANDY

extern "C" b32 BagSave_SubItem(BagSaveData* bag, u16 itemId, u16 quantity, HeapID heapId);

// Called when an item is removed after being used
extern "C" b32 THUMB_BRANCH_PokeList_SubItem(PokeList* pokeList, u16 itemID) {
    if (itemID == (u16)ITEM_INFINITE_CANDY) {
        return 0;
    }

    return BagSave_SubItem(pokeList->pokeListParty->bagSaveData, itemID, 1u, pokeList->heapID);  
}

#endif

#if ADD_LEVEL_CAP || ADD_INFINITE_RARE_CANDY

extern "C" b32 CanItemWithBattleStatsBeUsed(ItemData* itemData, PartyPkm* partyPkm, u32 itemID, u32 moveSlot) {
    if (PML_ItemGetParam(itemData, ITSTAT_HAS_BATTLE_STATS) == 1) {

        u32 status = PokeParty_GetStatusCond(partyPkm);
        if (PML_ItemGetParam(itemData, ITSTAT_AILMENT_SLP) && status == 2 ||
            PML_ItemGetParam(itemData, ITSTAT_AILMENT_PSN) && status == 5 ||
            PML_ItemGetParam(itemData, ITSTAT_AILMENT_BRN) && status == 4 ||
            PML_ItemGetParam(itemData, ITSTAT_AILMENT_FRZ) && status == 3 ||
            PML_ItemGetParam(itemData, ITSTAT_AILMENT_PAR) && status == 1) {
            return 1;
        }

        u32 currentHP = PokeParty_GetParam(partyPkm, PF_NowHP, 0);
        if ((PML_ItemGetParam(itemData, ITSTAT_BOOST_REVIVE) || PML_ItemGetParam(itemData, ITSTAT_BOOST_SACRED_ASH))
            && !PML_ItemGetParam(itemData, ITSTAT_BOOST_RARECANDY)) {
            if (!currentHP) {
                return 1;
            }
        }
        else if (PML_ItemGetParam(itemData, ITSTAT_FLAG_HPREPLENISH)
            && currentHP
            && currentHP < PokeParty_GetParam(partyPkm, PF_MaxHP, 0)) {
            return 1;
        }

        if (PML_ItemGetParam(itemData, ITSTAT_BOOST_RARECANDY) && PokeParty_GetParam(partyPkm, PF_Level, 0) < 0x64) {
            u32 levelCap = 100;
#if ADD_INFINITE_RARE_CANDY
            if (itemID == ITEM_INFINITE_CANDY || !RARE_CANDY_IGNORE_LVL_CAP)
                levelCap = GetLvlCap();
#else
            if (!RARE_CANDY_IGNORE_LVL_CAP)
                level_cap = GetLvlCap();
#endif
            u32 level = PokeParty_GetParam(partyPkm, PF_Level, 0);
            if (level < levelCap) {
                return 1;
            }
        }

        if ((PML_ItemGetParam(itemData, ITSTAT_BOOST_PP1) || PML_ItemGetParam(itemData, ITSTAT_BOOST_PPMAX))
            && PokeParty_GetParam(partyPkm, (PkmField)(PF_Move1PPUp + moveSlot), 0) < 3) {

            u32 moveID = PokeParty_GetParam(partyPkm, (PkmField)(PF_Move1 + moveSlot), 0);
            if (PML_MoveGetMaxPP(moveID, 0) >= 5) {
                return 1;
            }
        }

        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_PPREPLENISH) && PokeParty_CheckPPNeedsReplenish(partyPkm, moveSlot) == 1) {
            return 1;
        }
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_PPREPLENISH_FULL)) {

            u32 currentMoveSlot = 0;
            for (u8 currentMoveSlot = 0; currentMoveSlot < 4; ++currentMoveSlot) {
                if (PokeParty_CheckPPNeedsReplenish(partyPkm, currentMoveSlot)) {
                    return 1;
                }
            }
        }

        int HPEvs = PokeParty_GetParam(partyPkm, PF_EvHP, 0);
        int ATKEvs = PokeParty_GetParam(partyPkm, PF_EvATK, 0);
        int DEFEvs = PokeParty_GetParam(partyPkm, PF_EvDEF, 0);
        int SPEEvs = PokeParty_GetParam(partyPkm, PF_EvSPE, 0);
        int SPAEvs = PokeParty_GetParam(partyPkm, PF_EvSPA, 0);
        int SPDEvs = PokeParty_GetParam(partyPkm, PF_EvSPD, 0);
        u32 itemEvCapFlag = PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_ABOVE100);
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_HP)) {

            int itemHPEvs = PML_ItemGetParam(itemData, ITSTAT_EV_HP);
            if (itemHPEvs <= 0 || PokeParty_GetParam(partyPkm, PF_Species, 0) == PKM_SHEDINJA) {
                if (itemHPEvs < 0 && (HPEvs > 0 || PokeList_CanFriendshipBeIncreased(partyPkm, itemData))) {
                    return 1;
                }
            }
            else if ((HPEvs < 100 || itemEvCapFlag) &&
                HPEvs < 255 &&
                SPDEvs + SPAEvs + SPEEvs + DEFEvs + HPEvs + ATKEvs < 510) {
                return 1;
            }
        }
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_ATK)) {

            int itemATKEvs = PML_ItemGetParam(itemData, ITSTAT_EV_ATK);
            if (itemATKEvs <= 0) {
                if (itemATKEvs < 0 && (ATKEvs > 0 || PokeList_CanFriendshipBeIncreased(partyPkm, itemData))) {
                    return 1;
                }
            }
            else if ((ATKEvs < 100 || itemEvCapFlag) &&
                ATKEvs < 255 &&
                SPDEvs + SPAEvs + SPEEvs + DEFEvs + HPEvs + ATKEvs < 510) {
                return 1;
            }
        }
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_DEF)) {

            int itemDEFEvs = PML_ItemGetParam(itemData, ITSTAT_EV_DEF);
            if (itemDEFEvs <= 0) {
                if (itemDEFEvs < 0 && (DEFEvs > 0 || PokeList_CanFriendshipBeIncreased(partyPkm, itemData))) {
                    return 1;
                }
            }
            else if ((DEFEvs < 100 || itemEvCapFlag) &&
                DEFEvs < 255 &&
                SPDEvs + SPAEvs + SPEEvs + DEFEvs + HPEvs + ATKEvs < 510) {
                return 1;
            }
        }
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_SPE)) {

            int itemSPEEvs = PML_ItemGetParam(itemData, ITSTAT_EV_SPE);
            if (itemSPEEvs <= 0) {
                if (itemSPEEvs < 0 && (SPEEvs > 0 || PokeList_CanFriendshipBeIncreased(partyPkm, itemData))) {
                    return 1;
                }
            }
            else if ((SPEEvs < 100 || itemEvCapFlag) &&
                SPEEvs < 255 &&
                SPDEvs + SPAEvs + SPEEvs + DEFEvs + HPEvs + ATKEvs < 510) {
                return 1;
            }
        }
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_SPA)) {

            int itemSPAEvs = PML_ItemGetParam(itemData, ITSTAT_EV_SPA);
            if (itemSPAEvs <= 0) {
                if (itemSPAEvs < 0 && (SPAEvs > 0 || PokeList_CanFriendshipBeIncreased(partyPkm, itemData))) {
                    return 1;
                }
            }
            else if ((SPAEvs < 100 || itemEvCapFlag) &&
                SPAEvs < 255 &&
                SPDEvs + SPAEvs + SPEEvs + DEFEvs + HPEvs + ATKEvs < 510) {
                return 1;
            }
        }
        if (PML_ItemGetParam(itemData, ITSTAT_FLAG_EVADD_SPD)) {

            int itemSPDEvs = PML_ItemGetParam(itemData, ITSTAT_EV_SPD);
            if (itemSPDEvs <= 0) {
                if (itemSPDEvs < 0 && (SPDEvs > 0 || PokeList_CanFriendshipBeIncreased(partyPkm, itemData))) {
                    return 1;
                }
            }
            else if ((SPDEvs < 100 || itemEvCapFlag)
                && SPDEvs < 255
                && SPDEvs + SPAEvs + SPEEvs + DEFEvs + HPEvs + ATKEvs < 510) {
                return 1;
            }
        }
    }

    return 0;
}

extern "C" b32 THUMB_BRANCH_PokeList_CanItemWithBattleStatsBeUsed(PartyPkm* partyPkm, u32 itemID, u32 moveSlot, u16 heapID) {
    ItemData* itemData = (ItemData*)PML_ItemReadDataFile(itemID, 0, heapID);

    b32 output = CanItemWithBattleStatsBeUsed(itemData, partyPkm, itemID, moveSlot);

    GFL_HeapFree(itemData);
    return output;
}

#endif // ADD_LEVEL_CAP || ADD_INFINITE_RARE_CANDY


#if ADD_INFINITE_REPEL

extern "C" void playSeqFullVol(int a1);
extern "C" GameEvent* EventFieldToggleCycling_Create(Field* field, GameSystem* gsys);
extern "C" GameEvent* EventEntralinkWarpIn_CreateDefault(Field* field, GameSystem* gsys);
extern "C" GameEvent* EventMapChangeEscapeRope_Create(Field* field, GameSystem* gsys);
extern "C" GameEvent* EventSweetScent_Create(Field* field, GameSystem* gsys);
extern "C" GameEvent* EventFieldFishing_Create(Field* field, GameSystem* gsys);
extern "C" GameEvent* EventFieldToggleDowsing_Create(Field* field, GameSystem* gsys);
extern "C" ScriptWork* EventScriptCall_Start(GameEvent* parentEvent, u16 scrId, FieldActor* actor, int a4, u16 heapId);

extern "C" GameEvent* EventFieldToggleRepel_Create(Field* field, GameSystem* gsys);

// [OVL_12:0216CE24] Extended list of items that have an overworld effect while in the overworld
FieldCommonEvent ExtFieldCommonEvents[] = {
    { EventFieldToggleCycling_Create, ACTID_BIKE},
    { EventEntralinkWarpIn_CreateDefault, ACTID_WARP_SWEETSCENT },
    { EventMapChangeEscapeRope_Create, ACTID_ESCAPE_ROPE },
    { EventSweetScent_Create, ACTID_WARP_SWEETSCENT },
    { EventFieldFishing_Create, ACTID_FISHING },
    { EventFieldToggleDowsing_Create, ACTID_DOWSING_MCHN },
    { EventFieldToggleRepel_Create, ACTID_IGNORE },
};

// Callback for the use of the repel item from the Register menu
extern "C" b32 EventFieldToggleRepel_Callback(GameEvent* gEvent, u32* pState, EventFieldToggle* data) {
    if (*pState > 0)
        return 1;

    SaveControl* saveControl = GameData_GetSaveControl(GAME_DATA);
    EncountSave* encountSave = SaveControl_GetEncountSave(saveControl);
    if (!encountSave->repelSteps) {
        encountSave->repelSteps = 0xFF;
        EventScriptCall_Start(gEvent, BAG_ITEMUSED_SCRIPT, 0, 0, 21);
    }
    else {
        encountSave->repelSteps = 0x00;
        EventScriptCall_Start(gEvent, BAG_REPELEND_SCRIPT, 0, 0, 21);
    }

    ++*pState;
    return 0;
}

// Creates the event for the use of the repel item from the Register menu
extern "C" GameEvent * EventFieldToggleRepel_Create(Field * field, GameSystem * gsys) {
    GameEvent* repelEvent = GameEvent_Create(gsys, 0, (GameEventCallback)EventFieldToggleRepel_Callback, 8u);
    u32* repelEventData = (u32*)GameEvent_GetData(repelEvent);
    repelEventData[0] = (u32)gsys;
    repelEventData[1] = (u32)field;

    return repelEvent;
}

// Calls the list event function of the item that has an overworld effect while in the overworld
extern "C" GameEvent* THUMB_BRANCH_CallFieldCommonEventFunc(FieldCommonEventID cmnEventId, GameSystem* gsys, Field* field) {
    return ExtFieldCommonEvents[cmnEventId].function(field, gsys);
}

// Checks if the reg item action is blocked in the current moment (Ex. bike while in a house)
extern "C" u32 THUMB_BRANCH_PlayerActionPerms_IsActionBlocked(PlayerActionPerms* perms, ACTION_ID actionID) {
    if (actionID == ACTID_IGNORE) {
        // rn it bypasses the check but you can add your own blocking params HERE
        return 0;
    }

    return perms->actionsBlocked[actionID];
}

// Called every step the player takes to check the repel steps left (with infinite repel this function doesn't need to do anything)
extern "C" GameEvent* THUMB_BRANCH_CheckRepelEvent(Field* field, GameSystem* gsys, GameData* gameData) {
    return 0;
}

extern "C" b32 CheckRepelID(u16 itemID) {
    if (itemID == ITEM_INFINITE_REPEL) {
        return 1;
    }
    return 0;
}

extern "C" b32 THUMB_BRANCH_IsItemRepel(u16 itemID) {
    return CheckRepelID(itemID);
}

// Function that is called when the repel item is used from the bag, triggers the effect and creates the text boxes
extern "C" u32 THUMB_BRANCH_Bag_RepelEffect(Bag* bag) {
    u32 itemID = bag->itemID_8A0;

    if (CheckRepelID(itemID)) {
        SaveControl* SaveControl = GameData_GetSaveControl(bag->gameData);
        EncountSave* EncountSave = SaveControl_GetEncountSave(SaveControl);

        if (!EncountSave->repelSteps) {
            EncountSave->repelSteps = 0xFF;

            GFL_MsgDataLoadStrbuf(bag->msgData, BAG_ITEMUSED_MSGID, bag->strBuf1);
            copyVarForText(bag->wordSetSystem, 0, bag->void_ptr_8);
            Bag_LoadItemName(bag, 1, itemID);
            GFL_WordSetFormatStrbuf(bag->wordSetSystem, bag->strBuf2, bag->strBuf1);
            Bag_CreateTextBox(bag, 1);
            playSeqFullVol(1996);
        }
        else {
            EncountSave->repelSteps = 0x00;

            GFL_MsgDataLoadStrbuf(bag->msgData, BAG_REPELEND_MSGID, bag->strBuf1);
            copyVarForText(bag->wordSetSystem, 0, bag->void_ptr_8);
            Bag_LoadItemName(bag, 1, itemID);
            GFL_WordSetFormatStrbuf(bag->wordSetSystem, bag->strBuf2, bag->strBuf1);
            Bag_CreateTextBox(bag, 1);
            playSeqFullVol(1996);
        }

        return IsSkipDialogueKeyPressed(bag, (int)SkipDialogue);
    }

    return 0;
}

#endif

#if REPEL_SHAKING_GRASS

// Checks the odds muliplier to create a shaking spot
extern "C" u32 THUMB_BRANCH_PassPower_ApplyExploringChance(u32 basePhenomenonChance)
{
    SaveControl* SaveControl = GameData_GetSaveControl(GAME_DATA);
    EncountSave* EncountSave = SaveControl_GetEncountSave(SaveControl);

    // Reduce odds to 0 if the repel is active
    if (EncountSave->repelSteps != 0x00) {
        return 0;
    }

    if (g_PassPowerState.effectSrcIDs[PASSPOWER_EFF_EXPLORING] != 48
        && g_PassPowerState.effectValues[PASSPOWER_EFF_EXPLORING] < 3u) {
        basePhenomenonChance += PASSPOWER_EXPLORING_CHANCES[g_PassPowerState.effectValues[PASSPOWER_EFF_EXPLORING]];
        if (basePhenomenonChance > 100) {
            return 100;
        }
    }
    return basePhenomenonChance;
}

#endif // REPEL_SHAKING_GRASS