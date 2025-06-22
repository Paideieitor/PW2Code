#ifndef _ASSET_MANAGER_H
#define _ASSET_MANAGER_H

#include "../settings.h"

const char* libraryAssets[] =
{
	nullptr,
#if EXPAND_FIELD_EFFECTS
	"FieldEffects",
#endif
#if EXPAND_ABILITIES
	"Abilities",
#endif
#if EXPAND_ITEMS
	"Items",
#endif
};

const char* dataAssets[] =
{
	nullptr,
#if ADD_INFINITE_RARE_CANDY
	"a\\0\\2\\4\\622", // Infinite Rare Candy item data
#endif
#if ADD_INFINITE_REPEL
	"a\\0\\0\\2\\6", // Add Repel toggle message
	"a\\0\\0\\3\\411", // Add Repel toggle message while out of the bag
	"a\\0\\2\\4\\623", // Infinite Repel item data
	"a\\0\\5\\6\\1248", // Change the Repel field script
#endif
#if ADD_INFINITE_RARE_CANDY || ADD_INFINITE_REPEL
	"a\\0\\0\\2\\365", // Register List names
#endif
#if ADD_INFINITE_RARE_CANDY || ADD_INFINITE_REPEL || EXPAND_ITEMS
	"a\\0\\0\\2\\63", // Item descriptions
	"a\\0\\0\\2\\64", // Item names
	"a\\0\\0\\2\\481", // Item pick-up message
	"a\\0\\0\\2\\482", // Item plurals
#endif
#if EXPAND_ABILITIES
	"a\\0\\0\\2\\18", // Adds battle messages that depend on the Pokémon (3 entries per message)
	"a\\0\\0\\2\\19", // Adds battle messages that do not depend on the Pokémon (1 entry per message)
	"a\\0\\0\\2\\374", // Ability names
	"a\\0\\0\\2\\375", // Ability descriptions
	"a\\0\\0\\2\\396", // Ability NPC dialogue
	"a\\0\\0\\2\\487", // Ability names in all-caps
#endif
};

#endif // _ASSET_MANAGER_H