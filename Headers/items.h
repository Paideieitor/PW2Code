#ifndef __ITEMS_H
#define __ITEMS_H

#include "gfl/core/gfl_heap.h"

#include "defs.h"

#include "battle_events.h"

#define REG_ITEM(regID, itemID) (regID << 10) | itemID

typedef u32 ITEM;
enum ItemID : u32
{
	ITEM_NULL = 0x0,
	ITEM_MASTER_BALL = 0x1,
	ITEM_ULTRA_BALL = 0x2,
	ITEM_GREAT_BALL = 0x3,
	ITEM_POKE_BALL = 0x4,
	ITEM_SAFARI_BALL = 0x5,
	ITEM_NET_BALL = 0x6,
	ITEM_DIVE_BALL = 0x7,
	ITEM_NEST_BALL = 0x8,
	ITEM_REPEAT_BALL = 0x9,
	ITEM_TIMER_BALL = 0xA,
	ITEM_LUXURY_BALL = 0xB,
	ITEM_PREMIER_BALL = 0xC,
	ITEM_DUSK_BALL = 0xD,
	ITEM_HEAL_BALL = 0xE,
	ITEM_QUICK_BALL = 0xF,
	ITEM_CHERISH_BALL = 0x10,
	ITEM_POTION = 0x11,
	ITEM_ANTIDOTE = 0x12,
	ITEM_BURN_HEAL = 0x13,
	ITEM_ICE_HEAL = 0x14,
	ITEM_AWAKENING = 0x15,
	ITEM_PARALYZE_HEAL = 0x16,
	ITEM_FULL_RESTORE = 0x17,
	ITEM_MAX_POTION = 0x18,
	ITEM_HYPER_POTION = 0x19,
	ITEM_SUPER_POTION = 0x1A,
	ITEM_FULL_HEAL = 0x1B,
	ITEM_REVIVE = 0x1C,
	ITEM_MAX_REVIVE = 0x1D,
	ITEM_FRESH_WATER = 0x1E,
	ITEM_SODA_POP = 0x1F,
	ITEM_LEMONADE = 0x20,
	ITEM_MOOMOO_MILK = 0x21,
	ITEM_ENERGY_POWDER = 0x22,
	ITEM_ENERGY_ROOT = 0x23,
	ITEM_HEAL_POWDER = 0x24,
	ITEM_REVIVAL_HERB = 0x25,
	ITEM_ETHER = 0x26,
	ITEM_MAX_ETHER = 0x27,
	ITEM_ELIXIR = 0x28,
	ITEM_MAX_ELIXIR = 0x29,
	ITEM_LAVA_COOKIE = 0x2A,
	ITEM_BERRY_JUICE = 0x2B,
	ITEM_SACRED_ASH = 0x2C,
	ITEM_HP_UP = 0x2D,
	ITEM_PROTEIN = 0x2E,
	ITEM_IRON = 0x2F,
	ITEM_CARBOS = 0x30,
	ITEM_CALCIUM = 0x31,
	ITEM_RARE_CANDY = 0x32,
	ITEM_PP_UP = 0x33,
	ITEM_ZINC = 0x34,
	ITEM_PP_MAX = 0x35,
	ITEM_OLD_GATEAU = 0x36,
	ITEM_GUARD_SPEC = 0x37,
	ITEM_DIRE_HIT = 0x38,
	ITEM_X_ATTACK = 0x39,
	ITEM_X_DEFENSE = 0x3A,
	ITEM_X_SPEED = 0x3B,
	ITEM_X_ACCURACY = 0x3C,
	ITEM_X_SP_ATK = 0x3D,
	ITEM_X_SP_DEF = 0x3E,
	ITEM_POKE_DOLL = 0x3F,
	ITEM_FLUFFY_TAIL = 0x40,
	ITEM_BLUE_FLUTE = 0x41,
	ITEM_YELLOW_FLUTE = 0x42,
	ITEM_RED_FLUTE = 0x43,
	ITEM_BLACK_FLUTE = 0x44,
	ITEM_WHITE_FLUTE = 0x45,
	ITEM_SHOAL_SALT = 0x46,
	ITEM_SHOAL_SHELL = 0x47,
	ITEM_RED_SHARD = 0x48,
	ITEM_BLUE_SHARD = 0x49,
	ITEM_YELLOW_SHARD = 0x4A,
	ITEM_GREEN_SHARD = 0x4B,
	ITEM_SUPER_REPEL = 0x4C,
	ITEM_MAX_REPEL = 0x4D,
	ITEM_ESCAPE_ROPE = 0x4E,
	ITEM_REPEL = 0x4F,
	ITEM_SUN_STONE = 0x50,
	ITEM_MOON_STONE = 0x51,
	ITEM_FIRE_STONE = 0x52,
	ITEM_THUNDER_STONE = 0x53,
	ITEM_WATER_STONE = 0x54,
	ITEM_LEAF_STONE = 0x55,
	ITEM_TINY_MUSHROOM = 0x56,
	ITEM_BIG_MUSHROOM = 0x57,
	ITEM_PEARL = 0x58,
	ITEM_BIG_PEARL = 0x59,
	ITEM_STARDUST = 0x5A,
	ITEM_STAR_PIECE = 0x5B,
	ITEM_NUGGET = 0x5C,
	ITEM_HEART_SCALE = 0x5D,
	ITEM_HONEY = 0x5E,
	ITEM_GROWTH_MULCH = 0x5F,
	ITEM_DAMP_MULCH = 0x60,
	ITEM_STABLE_MULCH = 0x61,
	ITEM_GOOEY_MULCH = 0x62,
	ITEM_ROOT_FOSSIL = 0x63,
	ITEM_CLAW_FOSSIL = 0x64,
	ITEM_HELIX_FOSSIL = 0x65,
	ITEM_DOME_FOSSIL = 0x66,
	ITEM_OLD_AMBER = 0x67,
	ITEM_ARMOR_FOSSIL = 0x68,
	ITEM_SKULL_FOSSIL = 0x69,
	ITEM_RARE_BONE = 0x6A,
	ITEM_SHINY_STONE = 0x6B,
	ITEM_DUSK_STONE = 0x6C,
	ITEM_DAWN_STONE = 0x6D,
	ITEM_OVAL_STONE = 0x6E,
	ITEM_ODD_KEYSTONE = 0x6F,
	ITEM_GRISEOUS_ORB = 0x70,
	ITEM_113 = 0x71,
	ITEM_114 = 0x72,
	ITEM_115 = 0x73,
	ITEM_DOUSE_DRIVE = 0x74,
	ITEM_SHOCK_DRIVE = 0x75,
	ITEM_BURN_DRIVE = 0x76,
	ITEM_CHILL_DRIVE = 0x77,
	ITEM_120 = 0x78,
	ITEM_121 = 0x79,
	ITEM_122 = 0x7A,
	ITEM_123 = 0x7B,
	ITEM_124 = 0x7C,
	ITEM_125 = 0x7D,
	ITEM_126 = 0x7E,
	ITEM_127 = 0x7F,
	ITEM_128 = 0x80,
	ITEM_129 = 0x81,
	ITEM_130 = 0x82,
	ITEM_131 = 0x83,
	ITEM_132 = 0x84,
	ITEM_133 = 0x85,
	ITEM_SWEET_HEART = 0x86,
	ITEM_ADAMANT_ORB = 0x87,
	ITEM_LUSTROUS_ORB = 0x88,
	ITEM_GREET_MAIL = 0x89,
	ITEM_FAVORED_MAIL = 0x8A,
	ITEM_RSVP_MAIL = 0x8B,
	ITEM_THANKS_MAIL = 0x8C,
	ITEM_INQUIRY_MAIL = 0x8D,
	ITEM_LIKE_MAIL = 0x8E,
	ITEM_REPLY_MAIL = 0x8F,
	ITEM_BRIDGE_MAIL_S = 0x90,
	ITEM_BRIDGE_MAIL_D = 0x91,
	ITEM_BRIDGE_MAIL_T = 0x92,
	ITEM_BRIDGE_MAIL_V = 0x93,
	ITEM_BRIDGE_MAIL_M = 0x94,
	ITEM_CHERI_BERRY = 0x95,
	ITEM_CHESTO_BERRY = 0x96,
	ITEM_PECHA_BERRY = 0x97,
	ITEM_RAWST_BERRY = 0x98,
	ITEM_ASPEAR_BERRY = 0x99,
	ITEM_LEPPA_BERRY = 0x9A,
	ITEM_ORAN_BERRY = 0x9B,
	ITEM_PERSIM_BERRY = 0x9C,
	ITEM_LUM_BERRY = 0x9D,
	ITEM_SITRUS_BERRY = 0x9E,
	ITEM_FIGY_BERRY = 0x9F,
	ITEM_WIKI_BERRY = 0xA0,
	ITEM_MAGO_BERRY = 0xA1,
	ITEM_AGUAV_BERRY = 0xA2,
	ITEM_IAPAPA_BERRY = 0xA3,
	ITEM_RAZZ_BERRY = 0xA4,
	ITEM_BLUK_BERRY = 0xA5,
	ITEM_NANAB_BERRY = 0xA6,
	ITEM_WEPEAR_BERRY = 0xA7,
	ITEM_PINAP_BERRY = 0xA8,
	ITEM_POMEG_BERRY = 0xA9,
	ITEM_KELPSY_BERRY = 0xAA,
	ITEM_QUALOT_BERRY = 0xAB,
	ITEM_HONDEW_BERRY = 0xAC,
	ITEM_GREPA_BERRY = 0xAD,
	ITEM_TAMATO_BERRY = 0xAE,
	ITEM_CORNN_BERRY = 0xAF,
	ITEM_MAGOST_BERRY = 0xB0,
	ITEM_RABUTA_BERRY = 0xB1,
	ITEM_NOMEL_BERRY = 0xB2,
	ITEM_SPELON_BERRY = 0xB3,
	ITEM_PAMTRE_BERRY = 0xB4,
	ITEM_WATMEL_BERRY = 0xB5,
	ITEM_DURIN_BERRY = 0xB6,
	ITEM_BELUE_BERRY = 0xB7,
	ITEM_OCCA_BERRY = 0xB8,
	ITEM_PASSHO_BERRY = 0xB9,
	ITEM_WACAN_BERRY = 0xBA,
	ITEM_RINDO_BERRY = 0xBB,
	ITEM_YACHE_BERRY = 0xBC,
	ITEM_CHOPLE_BERRY = 0xBD,
	ITEM_KEBIA_BERRY = 0xBE,
	ITEM_SHUCA_BERRY = 0xBF,
	ITEM_COBA_BERRY = 0xC0,
	ITEM_PAYAPA_BERRY = 0xC1,
	ITEM_TANGA_BERRY = 0xC2,
	ITEM_CHARTI_BERRY = 0xC3,
	ITEM_KASIB_BERRY = 0xC4,
	ITEM_HABAN_BERRY = 0xC5,
	ITEM_COLBUR_BERRY = 0xC6,
	ITEM_BABIRI_BERRY = 0xC7,
	ITEM_CHILAN_BERRY = 0xC8,
	ITEM_LIECHI_BERRY = 0xC9,
	ITEM_GANLON_BERRY = 0xCA,
	ITEM_SALAC_BERRY = 0xCB,
	ITEM_PETAYA_BERRY = 0xCC,
	ITEM_APICOT_BERRY = 0xCD,
	ITEM_LANSAT_BERRY = 0xCE,
	ITEM_STARF_BERRY = 0xCF,
	ITEM_ENIGMA_BERRY = 0xD0,
	ITEM_MICLE_BERRY = 0xD1,
	ITEM_CUSTAP_BERRY = 0xD2,
	ITEM_JABOCA_BERRY = 0xD3,
	ITEM_ROWAP_BERRY = 0xD4,
	ITEM_BRIGHT_POWDER = 0xD5,
	ITEM_WHITE_HERB = 0xD6,
	ITEM_MACHO_BRACE = 0xD7,
	ITEM_EXP_SHARE = 0xD8,
	ITEM_QUICK_CLAW = 0xD9,
	ITEM_SOOTHE_BELL = 0xDA,
	ITEM_MENTAL_HERB = 0xDB,
	ITEM_CHOICE_BAND = 0xDC,
	ITEM_KING_S_ROCK = 0xDD,
	ITEM_SILVER_POWDER = 0xDE,
	ITEM_AMULET_COIN = 0xDF,
	ITEM_CLEANSE_TAG = 0xE0,
	ITEM_SOUL_DEW = 0xE1,
	ITEM_DEEP_SEA_TOOTH = 0xE2,
	ITEM_DEEP_SEA_SCALE = 0xE3,
	ITEM_SMOKE_BALL = 0xE4,
	ITEM_EVERSTONE = 0xE5,
	ITEM_FOCUS_BAND = 0xE6,
	ITEM_LUCKY_EGG = 0xE7,
	ITEM_SCOPE_LENS = 0xE8,
	ITEM_METAL_COAT = 0xE9,
	ITEM_LEFTOVERS = 0xEA,
	ITEM_DRAGON_SCALE = 0xEB,
	ITEM_LIGHT_BALL = 0xEC,
	ITEM_SOFT_SAND = 0xED,
	ITEM_HARD_STONE = 0xEE,
	ITEM_MIRACLE_SEED = 0xEF,
	ITEM_BLACK_GLASSES = 0xF0,
	ITEM_BLACK_BELT = 0xF1,
	ITEM_MAGNET = 0xF2,
	ITEM_MYSTIC_WATER = 0xF3,
	ITEM_SHARP_BEAK = 0xF4,
	ITEM_POISON_BARB = 0xF5,
	ITEM_NEVER_MELT_ICE = 0xF6,
	ITEM_SPELL_TAG = 0xF7,
	ITEM_TWISTED_SPOON = 0xF8,
	ITEM_CHARCOAL = 0xF9,
	ITEM_DRAGON_FANG = 0xFA,
	ITEM_SILK_SCARF = 0xFB,
	ITEM_UP_GRADE = 0xFC,
	ITEM_SHELL_BELL = 0xFD,
	ITEM_SEA_INCENSE = 0xFE,
	ITEM_LAX_INCENSE = 0xFF,
	ITEM_LUCKY_PUNCH = 0x100,
	ITEM_METAL_POWDER = 0x101,
	ITEM_THICK_CLUB = 0x102,
	ITEM_STICK = 0x103,
	ITEM_RED_SCARF = 0x104,
	ITEM_BLUE_SCARF = 0x105,
	ITEM_PINK_SCARF = 0x106,
	ITEM_GREEN_SCARF = 0x107,
	ITEM_YELLOW_SCARF = 0x108,
	ITEM_WIDE_LENS = 0x109,
	ITEM_MUSCLE_BAND = 0x10A,
	ITEM_WISE_GLASSES = 0x10B,
	ITEM_EXPERT_BELT = 0x10C,
	ITEM_LIGHT_CLAY = 0x10D,
	ITEM_LIFE_ORB = 0x10E,
	ITEM_POWER_HERB = 0x10F,
	ITEM_TOXIC_ORB = 0x110,
	ITEM_FLAME_ORB = 0x111,
	ITEM_QUICK_POWDER = 0x112,
	ITEM_FOCUS_SASH = 0x113,
	ITEM_ZOOM_LENS = 0x114,
	ITEM_METRONOME = 0x115,
	ITEM_IRON_BALL = 0x116,
	ITEM_LAGGING_TAIL = 0x117,
	ITEM_DESTINY_KNOT = 0x118,
	ITEM_BLACK_SLUDGE = 0x119,
	ITEM_ICY_ROCK = 0x11A,
	ITEM_SMOOTH_ROCK = 0x11B,
	ITEM_HEAT_ROCK = 0x11C,
	ITEM_DAMP_ROCK = 0x11D,
	ITEM_GRIP_CLAW = 0x11E,
	ITEM_CHOICE_SCARF = 0x11F,
	ITEM_STICKY_BARB = 0x120,
	ITEM_POWER_BRACER = 0x121,
	ITEM_POWER_BELT = 0x122,
	ITEM_POWER_LENS = 0x123,
	ITEM_POWER_BAND = 0x124,
	ITEM_POWER_ANKLET = 0x125,
	ITEM_POWER_WEIGHT = 0x126,
	ITEM_SHED_SHELL = 0x127,
	ITEM_BIG_ROOT = 0x128,
	ITEM_CHOICE_SPECS = 0x129,
	ITEM_FLAME_PLATE = 0x12A,
	ITEM_SPLASH_PLATE = 0x12B,
	ITEM_ZAP_PLATE = 0x12C,
	ITEM_MEADOW_PLATE = 0x12D,
	ITEM_ICICLE_PLATE = 0x12E,
	ITEM_FIST_PLATE = 0x12F,
	ITEM_TOXIC_PLATE = 0x130,
	ITEM_EARTH_PLATE = 0x131,
	ITEM_SKY_PLATE = 0x132,
	ITEM_MIND_PLATE = 0x133,
	ITEM_INSECT_PLATE = 0x134,
	ITEM_STONE_PLATE = 0x135,
	ITEM_SPOOKY_PLATE = 0x136,
	ITEM_DRACO_PLATE = 0x137,
	ITEM_DREAD_PLATE = 0x138,
	ITEM_IRON_PLATE = 0x139,
	ITEM_ODD_INCENSE = 0x13A,
	ITEM_ROCK_INCENSE = 0x13B,
	ITEM_FULL_INCENSE = 0x13C,
	ITEM_WAVE_INCENSE = 0x13D,
	ITEM_ROSE_INCENSE = 0x13E,
	ITEM_LUCK_INCENSE = 0x13F,
	ITEM_PURE_INCENSE = 0x140,
	ITEM_PROTECTOR = 0x141,
	ITEM_ELECTIRIZER = 0x142,
	ITEM_MAGMARIZER = 0x143,
	ITEM_DUBIOUS_DISC = 0x144,
	ITEM_REAPER_CLOTH = 0x145,
	ITEM_RAZOR_CLAW = 0x146,
	ITEM_RAZOR_FANG = 0x147,
	ITEM_TM01 = 0x148,
	ITEM_TM02 = 0x149,
	ITEM_TM03 = 0x14A,
	ITEM_TM04 = 0x14B,
	ITEM_TM05 = 0x14C,
	ITEM_TM06 = 0x14D,
	ITEM_TM07 = 0x14E,
	ITEM_TM08 = 0x14F,
	ITEM_TM09 = 0x150,
	ITEM_TM10 = 0x151,
	ITEM_TM11 = 0x152,
	ITEM_TM12 = 0x153,
	ITEM_TM13 = 0x154,
	ITEM_TM14 = 0x155,
	ITEM_TM15 = 0x156,
	ITEM_TM16 = 0x157,
	ITEM_TM17 = 0x158,
	ITEM_TM18 = 0x159,
	ITEM_TM19 = 0x15A,
	ITEM_TM20 = 0x15B,
	ITEM_TM21 = 0x15C,
	ITEM_TM22 = 0x15D,
	ITEM_TM23 = 0x15E,
	ITEM_TM24 = 0x15F,
	ITEM_TM25 = 0x160,
	ITEM_TM26 = 0x161,
	ITEM_TM27 = 0x162,
	ITEM_TM28 = 0x163,
	ITEM_TM29 = 0x164,
	ITEM_TM30 = 0x165,
	ITEM_TM31 = 0x166,
	ITEM_TM32 = 0x167,
	ITEM_TM33 = 0x168,
	ITEM_TM34 = 0x169,
	ITEM_TM35 = 0x16A,
	ITEM_TM36 = 0x16B,
	ITEM_TM37 = 0x16C,
	ITEM_TM38 = 0x16D,
	ITEM_TM39 = 0x16E,
	ITEM_TM40 = 0x16F,
	ITEM_TM41 = 0x170,
	ITEM_TM42 = 0x171,
	ITEM_TM43 = 0x172,
	ITEM_TM44 = 0x173,
	ITEM_TM45 = 0x174,
	ITEM_TM46 = 0x175,
	ITEM_TM47 = 0x176,
	ITEM_TM48 = 0x177,
	ITEM_TM49 = 0x178,
	ITEM_TM50 = 0x179,
	ITEM_TM51 = 0x17A,
	ITEM_TM52 = 0x17B,
	ITEM_TM53 = 0x17C,
	ITEM_TM54 = 0x17D,
	ITEM_TM55 = 0x17E,
	ITEM_TM56 = 0x17F,
	ITEM_TM57 = 0x180,
	ITEM_TM58 = 0x181,
	ITEM_TM59 = 0x182,
	ITEM_TM60 = 0x183,
	ITEM_TM61 = 0x184,
	ITEM_TM62 = 0x185,
	ITEM_TM63 = 0x186,
	ITEM_TM64 = 0x187,
	ITEM_TM65 = 0x188,
	ITEM_TM66 = 0x189,
	ITEM_TM67 = 0x18A,
	ITEM_TM68 = 0x18B,
	ITEM_TM69 = 0x18C,
	ITEM_TM70 = 0x18D,
	ITEM_TM71 = 0x18E,
	ITEM_TM72 = 0x18F,
	ITEM_TM73 = 0x190,
	ITEM_TM74 = 0x191,
	ITEM_TM75 = 0x192,
	ITEM_TM76 = 0x193,
	ITEM_TM77 = 0x194,
	ITEM_TM78 = 0x195,
	ITEM_TM79 = 0x196,
	ITEM_TM80 = 0x197,
	ITEM_TM81 = 0x198,
	ITEM_TM82 = 0x199,
	ITEM_TM83 = 0x19A,
	ITEM_TM84 = 0x19B,
	ITEM_TM85 = 0x19C,
	ITEM_TM86 = 0x19D,
	ITEM_TM87 = 0x19E,
	ITEM_TM88 = 0x19F,
	ITEM_TM89 = 0x1A0,
	ITEM_TM90 = 0x1A1,
	ITEM_TM91 = 0x1A2,
	ITEM_TM92 = 0x1A3,
	ITEM_HM01 = 0x1A4,
	ITEM_HM02 = 0x1A5,
	ITEM_HM03 = 0x1A6,
	ITEM_HM04 = 0x1A7,
	ITEM_HM05 = 0x1A8,
	ITEM_HM06 = 0x1A9,
	ITEM_426 = 0x1AA,
	ITEM_427 = 0x1AB,
	ITEM_EXPLORER_KIT = 0x1AC,
	ITEM_LOOT_SACK = 0x1AD,
	ITEM_RULE_BOOK = 0x1AE,
	ITEM_POKE_RADAR = 0x1AF,
	ITEM_POINT_CARD = 0x1B0,
	ITEM_JOURNAL = 0x1B1,
	ITEM_SEAL_CASE = 0x1B2,
	ITEM_FASHION_CASE = 0x1B3,
	ITEM_SEAL_BAG = 0x1B4,
	ITEM_PAL_PAD = 0x1B5,
	ITEM_WORKS_KEY = 0x1B6,
	ITEM_OLD_CHARM = 0x1B7,
	ITEM_GALACTIC_KEY = 0x1B8,
	ITEM_RED_CHAIN = 0x1B9,
	ITEM_TOWN_MAP = 0x1BA,
	ITEM_VS_SEEKER = 0x1BB,
	ITEM_COIN_CASE = 0x1BC,
	ITEM_OLD_ROD = 0x1BD,
	ITEM_GOOD_ROD = 0x1BE,
	ITEM_SUPER_ROD = 0x1BF,
	ITEM_SPRAYDUCK = 0x1C0,
	ITEM_POFFIN_CASE = 0x1C1,
	ITEM_BIKE = 0x1C2,
	ITEM_SUITE_KEY = 0x1C3,
	ITEM_OAK_S_LETTER = 0x1C4,
	ITEM_LUNAR_WING = 0x1C5,
	ITEM_MEMBER_CARD = 0x1C6,
	ITEM_AZURE_FLUTE = 0x1C7,
	ITEM_S_S_TICKET = 0x1C8,
	ITEM_CONTEST_PASS = 0x1C9,
	ITEM_MAGMA_STONE = 0x1CA,
	ITEM_PARCEL = 0x1CB,
	ITEM_COUPON_1 = 0x1CC,
	ITEM_COUPON_2 = 0x1CD,
	ITEM_COUPON_3 = 0x1CE,
	ITEM_STORAGE_KEY = 0x1CF,
	ITEM_SECRET_POTION = 0x1D0,
	ITEM_VS_RECORDER = 0x1D1,
	ITEM_GRACIDEA = 0x1D2,
	ITEM_SECRET_KEY = 0x1D3,
	ITEM_APRICORN_BOX = 0x1D4,
	ITEM_UNOWN_REPORT = 0x1D5,
	ITEM_BERRY_POTS = 0x1D6,
	ITEM_DOWSING_MACHINE = 0x1D7,
	ITEM_BLUE_CARD = 0x1D8,
	ITEM_SLOWPOKE_TAIL = 0x1D9,
	ITEM_CLEAR_BELL = 0x1DA,
	ITEM_CARD_KEY = 0x1DB,
	ITEM_BASEMENT_KEY = 0x1DC,
	ITEM_SQUIRT_BOTTLE = 0x1DD,
	ITEM_RED_SCALE = 0x1DE,
	ITEM_LOST_ITEM = 0x1DF,
	ITEM_PASS = 0x1E0,
	ITEM_MACHINE_PART = 0x1E1,
	ITEM_SILVER_WING = 0x1E2,
	ITEM_RAINBOW_WING = 0x1E3,
	ITEM_MYSTERY_EGG = 0x1E4,
	ITEM_RED_APRICORN = 0x1E5,
	ITEM_BLUE_APRICORN = 0x1E6,
	ITEM_YELLOW_APRICORN = 0x1E7,
	ITEM_GREEN_APRICORN = 0x1E8,
	ITEM_PINK_APRICORN = 0x1E9,
	ITEM_WHITE_APRICORN = 0x1EA,
	ITEM_BLACK_APRICORN = 0x1EB,
	ITEM_FAST_BALL = 0x1EC,
	ITEM_LEVEL_BALL = 0x1ED,
	ITEM_LURE_BALL = 0x1EE,
	ITEM_HEAVY_BALL = 0x1EF,
	ITEM_LOVE_BALL = 0x1F0,
	ITEM_FRIEND_BALL = 0x1F1,
	ITEM_MOON_BALL = 0x1F2,
	ITEM_SPORT_BALL = 0x1F3,
	ITEM_PARK_BALL = 0x1F4,
	ITEM_PHOTO_ALBUM = 0x1F5,
	ITEM_GB_SOUNDS = 0x1F6,
	ITEM_TIDAL_BELL = 0x1F7,
	ITEM_RAGE_CANDY_BAR = 0x1F8,
	ITEM_DATA_CARD_01 = 0x1F9,
	ITEM_DATA_CARD_02 = 0x1FA,
	ITEM_DATA_CARD_03 = 0x1FB,
	ITEM_DATA_CARD_04 = 0x1FC,
	ITEM_DATA_CARD_05 = 0x1FD,
	ITEM_DATA_CARD_06 = 0x1FE,
	ITEM_DATA_CARD_07 = 0x1FF,
	ITEM_DATA_CARD_08 = 0x200,
	ITEM_DATA_CARD_09 = 0x201,
	ITEM_DATA_CARD_10 = 0x202,
	ITEM_DATA_CARD_11 = 0x203,
	ITEM_DATA_CARD_12 = 0x204,
	ITEM_DATA_CARD_13 = 0x205,
	ITEM_DATA_CARD_14 = 0x206,
	ITEM_DATA_CARD_15 = 0x207,
	ITEM_DATA_CARD_16 = 0x208,
	ITEM_DATA_CARD_17 = 0x209,
	ITEM_DATA_CARD_18 = 0x20A,
	ITEM_DATA_CARD_19 = 0x20B,
	ITEM_DATA_CARD_20 = 0x20C,
	ITEM_DATA_CARD_21 = 0x20D,
	ITEM_DATA_CARD_22 = 0x20E,
	ITEM_DATA_CARD_23 = 0x20F,
	ITEM_DATA_CARD_24 = 0x210,
	ITEM_DATA_CARD_25 = 0x211,
	ITEM_DATA_CARD_26 = 0x212,
	ITEM_DATA_CARD_27 = 0x213,
	ITEM_JADE_ORB = 0x214,
	ITEM_LOCK_CAPSULE = 0x215,
	ITEM_RED_ORB = 0x216,
	ITEM_BLUE_ORB = 0x217,
	ITEM_ENIGMA_STONE = 0x218,
	ITEM_PRISM_SCALE = 0x219,
	ITEM_EVIOLITE = 0x21A,
	ITEM_FLOAT_STONE = 0x21B,
	ITEM_ROCKY_HELMET = 0x21C,
	ITEM_AIR_BALLOON = 0x21D,
	ITEM_RED_CARD = 0x21E,
	ITEM_RING_TARGET = 0x21F,
	ITEM_BINDING_BAND = 0x220,
	ITEM_ABSORB_BULB = 0x221,
	ITEM_CELL_BATTERY = 0x222,
	ITEM_EJECT_BUTTON = 0x223,
	ITEM_FIRE_GEM = 0x224,
	ITEM_WATER_GEM = 0x225,
	ITEM_ELECTRIC_GEM = 0x226,
	ITEM_GRASS_GEM = 0x227,
	ITEM_ICE_GEM = 0x228,
	ITEM_FIGHTING_GEM = 0x229,
	ITEM_POISON_GEM = 0x22A,
	ITEM_GROUND_GEM = 0x22B,
	ITEM_FLYING_GEM = 0x22C,
	ITEM_PSYCHIC_GEM = 0x22D,
	ITEM_BUG_GEM = 0x22E,
	ITEM_ROCK_GEM = 0x22F,
	ITEM_GHOST_GEM = 0x230,
	ITEM_DRAGON_GEM = 0x231,
	ITEM_DARK_GEM = 0x232,
	ITEM_STEEL_GEM = 0x233,
	ITEM_NORMAL_GEM = 0x234,
	ITEM_HEALTH_WING = 0x235,
	ITEM_MUSCLE_WING = 0x236,
	ITEM_RESIST_WING = 0x237,
	ITEM_GENIUS_WING = 0x238,
	ITEM_CLEVER_WING = 0x239,
	ITEM_SWIFT_WING = 0x23A,
	ITEM_PRETTY_WING = 0x23B,
	ITEM_COVER_FOSSIL = 0x23C,
	ITEM_PLUME_FOSSIL = 0x23D,
	ITEM_LIBERTY_PASS = 0x23E,
	ITEM_PASS_ORB = 0x23F,
	ITEM_DREAM_BALL = 0x240,
	ITEM_POKE_TOY = 0x241,
	ITEM_PROP_CASE = 0x242,
	ITEM_DRAGON_SKULL = 0x243,
	ITEM_BALM_MUSHROOM = 0x244,
	ITEM_BIG_NUGGET = 0x245,
	ITEM_PEARL_STRING = 0x246,
	ITEM_COMET_SHARD = 0x247,
	ITEM_RELIC_COPPER = 0x248,
	ITEM_RELIC_SILVER = 0x249,
	ITEM_RELIC_GOLD = 0x24A,
	ITEM_RELIC_VASE = 0x24B,
	ITEM_RELIC_BAND = 0x24C,
	ITEM_RELIC_STATUE = 0x24D,
	ITEM_RELIC_CROWN = 0x24E,
	ITEM_CASTELIACONE = 0x24F,
	ITEM_DIRE_HIT_2 = 0x250,
	ITEM_X_SPEED_2 = 0x251,
	ITEM_X_SP_ATK_2 = 0x252,
	ITEM_X_SP_DEF_2 = 0x253,
	ITEM_X_DEFENSE_2 = 0x254,
	ITEM_X_ATTACK_2 = 0x255,
	ITEM_X_ACCURACY_2 = 0x256,
	ITEM_X_SPEED_3 = 0x257,
	ITEM_X_SP_ATK_3 = 0x258,
	ITEM_X_SP_DEF_3 = 0x259,
	ITEM_X_DEFENSE_3 = 0x25A,
	ITEM_X_ATTACK_3 = 0x25B,
	ITEM_X_ACCURACY_3 = 0x25C,
	ITEM_X_SPEED_6 = 0x25D,
	ITEM_X_SP_ATK_6 = 0x25E,
	ITEM_X_SP_DEF_6 = 0x25F,
	ITEM_X_DEFENSE_6 = 0x260,
	ITEM_X_ATTACK_6 = 0x261,
	ITEM_X_ACCURACY_6 = 0x262,
	ITEM_ABILITY_URGE = 0x263,
	ITEM_ITEM_DROP = 0x264,
	ITEM_ITEM_URGE = 0x265,
	ITEM_RESET_URGE = 0x266,
	ITEM_DIRE_HIT_3 = 0x267,
	ITEM_LIGHT_STONE = 0x268,
	ITEM_DARK_STONE = 0x269,
	ITEM_TM93 = 0x26A,
	ITEM_TM94 = 0x26B,
	ITEM_TM95 = 0x26C,
	ITEM_XTRANSCEIVER_M = 0x26D,
	ITEM_622 = 0x26E,
	ITEM_GRAM_1 = 0x26F,
	ITEM_GRAM_2 = 0x270,
	ITEM_GRAM_3 = 0x271,
	ITEM_XTRANSCEIVER_F = 0x272,
	ITEM_MEDAL_BOX = 0x273,
	ITEM_DNA_SPLICERS_1 = 0x274,
	ITEM_DNA_SPLICERS_2 = 0x275,
	ITEM_PERMIT = 0x276,
	ITEM_OVAL_CHARM = 0x277,
	ITEM_SHINY_CHARM = 0x278,
	ITEM_PLASMA_CARD = 0x279,
	ITEM_GRUBBY_HANKY = 0x27A,
	ITEM_COLRESS_MACHINE = 0x27B,
	ITEM_DROPPED_ITEM_1 = 0x27C,
	ITEM_DROPPED_ITEM_2 = 0x27D,
	ITEM_REVEAL_GLASS = 0x27E,
};

enum ItemType : u32
{
	ITEMTYPE_STD = 0,
	ITEMTYPE_MEDICINE = 1,
	ITEMTYPE_TM = 2,
	ITEMTYPE_BERRY = 3,
	ITEMTYPE_KEY = 4,
	ITEMTYPE_FREE_SPACE = 5,
	ITEMTYPE_MAX = 6,
};

typedef u32 REG_ID;
enum RegistrableID : u32
{
	REGIT_BIKE = 0,
	REGIT_TOWN_MAP = 1,
	REGIT_VS_RECORDER = 2,
	REGIT_PAL_PAD = 3,
	REGIT_SUPER_ROD = 4,
	REGIT_DOWSING_MCHN = 5,
	REGIT_GRACIDEA = 6,
	REGIT_DNA_SPLICERS_1 = 7,
	REGIT_DNA_SPLICERS_2 = 8,
	REGIT_POKEINFO_STATUS = 9,
	REGIT_POKEINFO_SKILLS = 10,
	REGIT_POKEINFO_RIBBONS = 11,
	REGIT_BAG_ITEMS = 12,
	REGIT_BAG_MEDICINE = 13,
	REGIT_BAG_TMS_HMS = 14,
	REGIT_BAG_BERRIES = 15,
	REGIT_BAG_KEYITEMS = 16,
	REGIT_BAG_FREESPACE = 17,
	REGIT_POKEDEX_LIST = 18,
	REGIT_POKEDEX_SEARCH = 19,
	REGIT_POKEDEX_INFO = 20,
	REGIT_POKEDEX_AREA = 21,
	REGIT_POKEDEX_CRY = 22,
	REGIT_POKEDEX_FORMS = 23,
	REGIT_TRAINERCARD_FRONT = 24,
	REGIT_TRAINERCARD_BACK = 25,
	REGIT_TRAINERCARD_BADGES = 26,
	REGIT_OPTIONS = 27,
	REGIT_XTRANSCEIVER = 28,
	REGIT_MEDAL_BOX = 29,
	REGIT_POKEMON_LIST = 30,
	REGIT_POKEDEXHABITAT_LIST = 31,
	REGIT_POKEDEXHABITAT_INFO = 32,
	REGIT_REVEAL_GLASS = 33,
	REGIT_UNUSED_34 = 34,
	REGIT_UNUSED_35 = 35,
	REGIT_UNUSED_36 = 36,
	REGIT_UNUSED_37 = 37,
	REGIT_UNUSED_38 = 38,
	REGIT_UNUSED_39 = 39,
	REGIT_UNUSED_40 = 40,
	REGIT_UNUSED_41 = 41,
	REGIT_UNUSED_42 = 42,
	REGIT_UNUSED_43 = 43,
	REGIT_UNUSED_44 = 44,
	REGIT_UNUSED_45 = 45,
	REGIT_UNUSED_46 = 46,
	REGIT_UNUSED_47 = 47,
	REGIT_UNUSED_48 = 48,
	REGIT_UNUSED_49 = 49,
	REGIT_UNUSED_50 = 50,
	REGIT_UNUSED_51 = 51,
	REGIT_UNUSED_52 = 52,
	REGIT_UNUSED_53 = 53,
	REGIT_UNUSED_54 = 54,
	REGIT_UNUSED_55 = 55,
	REGIT_UNUSED_56 = 56,
	REGIT_UNUSED_57 = 57,
	REGIT_UNUSED_58 = 58,
	REGIT_UNUSED_59 = 59,
	REGIT_UNUSED_60 = 60,
	REGIT_UNUSED_61 = 61,
	REGIT_UNUSED_62 = 62,
	REGIT_UNUSED_63 = 63,
	REGIT_NOT_REGISTRABLE = 0xFF,
};

struct ItemBattleStats
{
	u8 cureInflict;
	u8 boost[4];
	u8 functionFlags0;
	u8 functionFlags1;
	u8 evHP;
	u8 evATK;
	u8 evDEF;
	u8 evSPE;
	u8 evSPA;
	u8 evSPD;
	u8 healAmount;
	u8 PPGain;
	u8 friendship1;
	u8 friendship2;
	u8 friendship3;
	u16 flags;
};

struct SWAN_ALIGNED(4) ItemData
{
	u16 price;
	u8 heldEffect;
	u8 heldArgument;
	u8 naturalGiftEffect;
	u8 flingEffect;
	u8 flingPower;
	u8 naturalGiftPower;
	u16 packed;
	u8 effectField;
	u8 effectBattle;
	u8 hasBattleStats;
	u8 itemClass;
	u8 consumable;
	u8 sortIndex;
	ItemBattleStats battleStats;
};

enum ItemField : u32
{
	ITSTAT_PRICE = 0x0,
	ITSTAT_USE_EFFECT = 0x1,
	ITSTAT_USE_PARAM = 0x2,
	ITSTAT_FLAG1 = 0x3,
	ITSTAT_FLAG2 = 0x4,
	ITSTAT_POCKET_FIELD = 0x5,
	ITSTAT_EFFECT_FIELD = 0x6,
	ITSTAT_EFFECT_BATTLE = 0x7,
	ITSTAT_NATURAL_GIFT_EFFECT = 0x8,
	ITSTAT_FLING_EFFECT = 0x9,
	ITSTAT_FLING_POWER = 0xA,
	ITSTAT_NATURAL_GIFT_POWER = 0xB,
	ITSTAT_NATURAL_GIFT_TYPE = 0xC,
	ITSTAT_POCKET_BATTLE = 0xD,
	ITSTAT_HAS_BATTLE_STATS = 0xE,
	ITSTAT_CLASS = 0xF,
	ITSTAT_CONSUMABLE = 0x10,
	ITSTAT_SORT_IDX = 0x11,
	ITSTAT_AILMENT_SLP = 0x12,
	ITSTAT_AILMENT_PSN = 0x13,
	ITSTAT_AILMENT_BRN = 0x14,
	ITSTAT_AILMENT_FRZ = 0x15,
	ITSTAT_AILMENT_PAR = 0x16,
	ITSTAT_AILMENT_CFZ = 0x17,
	ITSTAT_AILMENT_INF = 0x18,
	ITSTAT_AILMENT_GSP = 0x19,
	ITSTAT_BOOST_REVIVE = 0x1A,
	ITSTAT_BOOST_SACRED_ASH = 0x1B,
	ITSTAT_BOOST_RARECANDY = 0x1C,
	ITSTAT_BOOST_EVOSTONE = 0x1D,
	ITSTAT_BOOST_ATK = 0x1E,
	ITSTAT_BOOST_DEF = 0x1F,
	ITSTAT_BOOST_SPA = 0x20,
	ITSTAT_BOOST_SPD = 0x21,
	ITSTAT_BOOST_SPE = 0x22,
	ITSTAT_BOOST_ACC = 0x23,
	ITSTAT_BOOST_CRIT = 0x24,
	ITSTAT_BOOST_PP1 = 0x25,
	ITSTAT_BOOST_PPMAX = 0x26,
	ITSTAT_FLAG_PPREPLENISH = 0x27,
	ITSTAT_FLAG_PPREPLENISH_FULL = 0x28,
	ITSTAT_FLAG_HPREPLENISH = 0x29,
	ITSTAT_FLAG_EVADD_HP = 0x2A,
	ITSTAT_FLAG_EVADD_ATK = 0x2B,
	ITSTAT_FLAG_EVADD_DEF = 0x2C,
	ITSTAT_FLAG_EVADD_SPE = 0x2D,
	ITSTAT_FLAG_EVADD_SPA = 0x2E,
	ITSTAT_FLAG_EVADD_SPD = 0x2F,
	ITSTAT_FLAG_EVADD_ABOVE100 = 0x30,
	ITSTAT_FLAG_FRIENDSHIP_ADD1 = 0x31,
	ITSTAT_FLAG_FRIENDSHIP_ADD2 = 0x32,
	ITSTAT_FLAG_FRIENDSHIP_ADD3 = 0x33,
	ITSTAT_EV_HP = 0x34,
	ITSTAT_EV_ATK = 0x35,
	ITSTAT_EV_DEF = 0x36,
	ITSTAT_EV_SPE = 0x37,
	ITSTAT_EV_SPA = 0x38,
	ITSTAT_EV_SPD = 0x39,
	ITSTAT_HEAL_AMOUNT = 0x3A,
	ITSTAT_PP_GAIN = 0x3B,
	ITSTAT_FRIENDSHIP1 = 0x3C,
	ITSTAT_FRIENDSHIP2 = 0x3D,
	ITSTAT_FRIENDSHIP3 = 0x3E,
};

typedef BattleEventHandlerTableEntry* (*ItemEventAddFunc)(u32*);
struct ItemEventAddTable
{
	ITEM itemID;
	ItemEventAddFunc func;
};

struct BattleEventItem;
struct BattleMon;

extern "C" void* PML_ItemReadDataFile(u32 itemID, int dataType, HeapID heapID);
extern "C" u32 PML_ItemGetParam(ItemData* itemData, ItemField itemField);
extern "C" b32 PML_ItemIsBerry(u16 itemID);
extern "C" b32 PML_ItemIsMail(u16 itemID);
extern "C" u32 ItemGetParam(u16 itemID, ItemField itemField);
extern "C" u32 GetItemParam(u16 itemID, ItemField itemField, HeapID heapId);

extern "C" int CommonGetItemParam(BattleEventItem* item, ItemField itemField);

extern "C" BattleEventItem * ItemEvent_AddItem(BattleMon * battleMon);
extern "C" void ItemEvent_RemoveItem(BattleMon * battleMon);
extern "C" BattleEventItem * ItemEvent_AddItemCore(BattleMon * battleMon, ITEM itemID);
extern "C" void ItemEvent_PushRun(BattleEventItem * item, ServerFlow * serverFlow, u32 pokemonSlot);

extern "C" BattleEventHandlerTableEntry * EventAddCheriBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChestoBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRawstBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAspearBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPersimBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPechaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLumBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLeppaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddOranBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBerryJuice(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSitrusBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFigyBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWikiBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMagoBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAguavBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddIapapaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLiechiBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddGanlonBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSalacBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPetayaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddApicotBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLansatBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddStarfBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddEnigmaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddOccaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPasshoBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWacanBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRindoBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddYacheBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChopleBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddKebiaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddShucaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddCobaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPayapaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddTangaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChartiBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddKasibBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddHabanBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddColburBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBabiriBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChilanBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddCustapBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMicleBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddJabocaBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRowapBerry(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWhiteHerb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMentalHerb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBrightPowder(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMachoBrace(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddQuickClaw(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLaggingTail(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLaggingTail(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddKingsRock(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddScopeLens(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWideLens(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddScopeLens(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddZoomLens(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLaxIncense(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMuscleBand(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWiseGlasses(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDeepSeaTooth(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDeepSeaScale(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMetalPowder(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddQuickPowder(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLightBall(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLuckyPunch(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddStick(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSoulDew(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddThickClub(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChoiceBand(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBlackSludge(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChoiceSpecs(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddChoiceScarf(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSilverPowder(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSoftSand(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddHardStone(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMiracleSeed(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBlackGlasses(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBlackBelt(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMagnet(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMetalCoat(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMysticWater(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSharpBeak(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRazorFang(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPoisonBarb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddNeverMeltIce(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSpellTag(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddTwistedSpoon(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddCharcoal(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDragonFang(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSilkScarf(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddOddIncense(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRockIncense(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWaveIncense(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSeaIncense(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRoseIncense(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFocusSash(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFocusBand(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddExpertBelt(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLifeOrb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMetronomeItem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddGripClaw(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddShellBell(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLightClay(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerHerb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLeftovers(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddToxicOrb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFlameOrb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddLustrousOrb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAdamantOrb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddIronBall(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDestinyKnot(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddIcyRock(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSmoothRock(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddHeatRock(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDampRock(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddStickyBarb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerBracer(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerBelt(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerLens(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerBand(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerAnklet(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPowerWeight(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFlamePlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSplashPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddZapPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMeadowPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddIciclePlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFistPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddToxicPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddEarthPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSkyPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddMindPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddInsectPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddStonePlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSpookyPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDracoPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDreadPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddIronPlate(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBigRoot(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSmokeBall(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAmuletCoin(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAmuletCoin(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddGriseousOrb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFloatStone(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddEviolite(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRockyHelmet(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAirBalloon(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRedCard(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRingTarget(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBindingBand(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddAbsorbBulb(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddCellBattery(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddEjectButton(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFireGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddWaterGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddElectricGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddGrassGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddIceGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFightingGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPoisonGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddGroundGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddFlyingGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddPsychicGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddBugGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddRockGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddGhostGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDragonGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddDarkGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddSteelGem(u32 * handlerAmount);
extern "C" BattleEventHandlerTableEntry * EventAddNormalGem(u32 * handlerAmount);

#endif // __ITEMS_H

