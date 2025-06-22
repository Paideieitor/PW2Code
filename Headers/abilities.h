#ifndef __ABILITIES_H
#define __ABILITIES_H

#include "defs.h"

#include "battle_events.h"

typedef u32 ABILITY;
enum AbilID : u32
{
	ABIL_NULL = 0x0,
	ABIL_STENCH = 0x1,
	ABIL_DRIZZLE = 0x2,
	ABIL_SPEED_BOOST = 0x3,
	ABIL_BATTLE_ARMOR = 0x4,
	ABIL_STURDY = 0x5,
	ABIL_DAMP = 0x6,
	ABIL_LIMBER = 0x7,
	ABIL_SAND_VEIL = 0x8,
	ABIL_STATIC = 0x9,
	ABIL_VOLT_ABSORB = 0xA,
	ABIL_WATER_ABSORB = 0xB,
	ABIL_OBLIVIOUS = 0xC,
	ABIL_CLOUD_NINE = 0xD,
	ABIL_COMPOUND_EYES = 0xE,
	ABIL_INSOMNIA = 0xF,
	ABIL_COLOR_CHANGE = 0x10,
	ABIL_IMMUNITY = 0x11,
	ABIL_FLASH_FIRE = 0x12,
	ABIL_SHIELD_DUST = 0x13,
	ABIL_OWN_TEMPO = 0x14,
	ABIL_SUCTION_CUPS = 0x15,
	ABIL_INTIMIDATE = 0x16,
	ABIL_SHADOW_TAG = 0x17,
	ABIL_ROUGH_SKIN = 0x18,
	ABIL_WONDER_GUARD = 0x19,
	ABIL_LEVITATE = 0x1A,
	ABIL_EFFECT_SPORE = 0x1B,
	ABIL_SYNCHRONIZE = 0x1C,
	ABIL_CLEAR_BODY = 0x1D,
	ABIL_NATURAL_CURE = 0x1E,
	ABIL_LIGHTNING_ROD = 0x1F,
	ABIL_SERENE_GRACE = 0x20,
	ABIL_SWIFT_SWIM = 0x21,
	ABIL_CHLOROPHYLL = 0x22,
	ABIL_ILLUMINATE = 0x23,
	ABIL_TRACE = 0x24,
	ABIL_HUGE_POWER = 0x25,
	ABIL_POISON_POINT = 0x26,
	ABIL_INNER_FOCUS = 0x27,
	ABIL_MAGMA_ARMOR = 0x28,
	ABIL_WATER_VEIL = 0x29,
	ABIL_MAGNET_PULL = 0x2A,
	ABIL_SOUNDPROOF = 0x2B,
	ABIL_RAIN_DISH = 0x2C,
	ABIL_SAND_STREAM = 0x2D,
	ABIL_PRESSURE = 0x2E,
	ABIL_THICK_FAT = 0x2F,
	ABIL_EARLY_BIRD = 0x30,
	ABIL_FLAME_BODY = 0x31,
	ABIL_RUN_AWAY = 0x32,
	ABIL_KEEN_EYE = 0x33,
	ABIL_HYPER_CUTTER = 0x34,
	ABIL_PICKUP = 0x35,
	ABIL_TRUANT = 0x36,
	ABIL_HUSTLE = 0x37,
	ABIL_CUTE_CHARM = 0x38,
	ABIL_PLUS = 0x39,
	ABIL_MINUS = 0x3A,
	ABIL_FORECAST = 0x3B,
	ABIL_STICKY_HOLD = 0x3C,
	ABIL_SHED_SKIN = 0x3D,
	ABIL_GUTS = 0x3E,
	ABIL_MARVEL_SCALE = 0x3F,
	ABIL_LIQUID_OOZE = 0x40,
	ABIL_OVERGROW = 0x41,
	ABIL_BLAZE = 0x42,
	ABIL_TORRENT = 0x43,
	ABIL_SWARM = 0x44,
	ABIL_ROCK_HEAD = 0x45,
	ABIL_DROUGHT = 0x46,
	ABIL_ARENA_TRAP = 0x47,
	ABIL_VITAL_SPIRIT = 0x48,
	ABIL_WHITE_SMOKE = 0x49,
	ABIL_PURE_POWER = 0x4A,
	ABIL_SHELL_ARMOR = 0x4B,
	ABIL_AIR_LOCK = 0x4C,
	ABIL_TANGLED_FEET = 0x4D,
	ABIL_MOTOR_DRIVE = 0x4E,
	ABIL_RIVALRY = 0x4F,
	ABIL_STEADFAST = 0x50,
	ABIL_SNOW_CLOAK = 0x51,
	ABIL_GLUTTONY = 0x52,
	ABIL_ANGER_POINT = 0x53,
	ABIL_UNBURDEN = 0x54,
	ABIL_HEATPROOF = 0x55,
	ABIL_SIMPLE = 0x56,
	ABIL_DRY_SKIN = 0x57,
	ABIL_DOWNLOAD = 0x58,
	ABIL_IRON_FIST = 0x59,
	ABIL_POISON_HEAL = 0x5A,
	ABIL_ADAPTABILITY = 0x5B,
	ABIL_SKILL_LINK = 0x5C,
	ABIL_HYDRATION = 0x5D,
	ABIL_SOLAR_POWER = 0x5E,
	ABIL_QUICK_FEET = 0x5F,
	ABIL_NORMALIZE = 0x60,
	ABIL_SNIPER = 0x61,
	ABIL_MAGIC_GUARD = 0x62,
	ABIL_NO_GUARD = 0x63,
	ABIL_STALL = 0x64,
	ABIL_TECHNICIAN = 0x65,
	ABIL_LEAF_GUARD = 0x66,
	ABIL_KLUTZ = 0x67,
	ABIL_MOLD_BREAKER = 0x68,
	ABIL_SUPER_LUCK = 0x69,
	ABIL_AFTERMATH = 0x6A,
	ABIL_ANTICIPATION = 0x6B,
	ABIL_FOREWARN = 0x6C,
	ABIL_UNAWARE = 0x6D,
	ABIL_TINTED_LENS = 0x6E,
	ABIL_FILTER = 0x6F,
	ABIL_SLOW_START = 0x70,
	ABIL_SCRAPPY = 0x71,
	ABIL_STORM_DRAIN = 0x72,
	ABIL_ICE_BODY = 0x73,
	ABIL_SOLID_ROCK = 0x74,
	ABIL_SNOW_WARNING = 0x75,
	ABIL_HONEY_GATHER = 0x76,
	ABIL_FRISK = 0x77,
	ABIL_RECKLESS = 0x78,
	ABIL_MULTITYPE = 0x79,
	ABIL_FLOWER_GIFT = 0x7A,
	ABIL_BAD_DREAMS = 0x7B,
	ABIL_PICKPOCKET = 0x7C,
	ABIL_SHEER_FORCE = 0x7D,
	ABIL_CONTRARY = 0x7E,
	ABIL_UNNERVE = 0x7F,
	ABIL_DEFIANT = 0x80,
	ABIL_DEFEATIST = 0x81,
	ABIL_CURSED_BODY = 0x82,
	ABIL_HEALER = 0x83,
	ABIL_FRIEND_GUARD = 0x84,
	ABIL_WEAK_ARMOR = 0x85,
	ABIL_HEAVY_METAL = 0x86,
	ABIL_LIGHT_METAL = 0x87,
	ABIL_MULTISCALE = 0x88,
	ABIL_TOXIC_BOOST = 0x89,
	ABIL_FLARE_BOOST = 0x8A,
	ABIL_HARVEST = 0x8B,
	ABIL_TELEPATHY = 0x8C,
	ABIL_MOODY = 0x8D,
	ABIL_OVERCOAT = 0x8E,
	ABIL_POISON_TOUCH = 0x8F,
	ABIL_REGENERATOR = 0x90,
	ABIL_BIG_PECKS = 0x91,
	ABIL_SAND_RUSH = 0x92,
	ABIL_WONDER_SKIN = 0x93,
	ABIL_ANALYTIC = 0x94,
	ABIL_ILLUSION = 0x95,
	ABIL_IMPOSTER = 0x96,
	ABIL_INFILTRATOR = 0x97,
	ABIL_MUMMY = 0x98,
	ABIL_MOXIE = 0x99,
	ABIL_JUSTIFIED = 0x9A,
	ABIL_RATTLED = 0x9B,
	ABIL_MAGIC_BOUNCE = 0x9C,
	ABIL_SAP_SIPPER = 0x9D,
	ABIL_PRANKSTER = 0x9E,
	ABIL_SAND_FORCE = 0x9F,
	ABIL_IRON_BARBS = 0xA0,
	ABIL_ZEN_MODE = 0xA1,
	ABIL_VICTORY_STAR = 0xA2,
	ABIL_TURBOBLAZE = 0xA3,
	ABIL_TERAVOLT = 0xA4,
};

typedef BattleEventHandlerTableEntry* (*AbilityEventAddFunc)(u32*);

struct BattleMon;
struct BattleEventItem;

struct AbilityEventAddTable
{
	// AbilID
	ABILITY ability;
	AbilityEventAddFunc func;
};

extern "C" BattleEventHandlerTableEntry* EventAddIntimidate(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddClearBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddInnerFocus(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSteadfast(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddThickFat(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHyperCutter(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHugePower(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTintedLens(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSpeedBoost(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBlaze(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTorrent(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOvergrow(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSwarm(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddGuts(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSkillLink(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddKeenEye(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSimple(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSolidRock(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMarvelScale(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRivalry(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLeafGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDrizzle(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDrought(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandStream(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSnowWarning(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAirLock(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTechnician(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOblivious(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHydration(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPoisonHeal(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddIceBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRainDish(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddShieldDust(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAdaptability(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSereneGrace(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSolarPower(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSwiftSwim(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddChlorophyll(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddShedSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTangledFeet(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddQuickFeet(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHustle(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBattleArmor(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSuperLuck(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAngerPoint(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSniper(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddIronFist(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddCompoundEyes(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRockHead(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddReckless(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStatic(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPoisonPoint(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlameBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddEffectSpore(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPlusMinus(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddCuteCharm(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandVeil(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSnowCloak(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTrace(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddNormalize(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRoughSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddNaturalCure(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSynchronize(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDownload(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSturdy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHeatproof(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddUnaware(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDrySkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddVoltAbsorb(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWaterAbsorb(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMotorDrive(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLimber(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddInsomnia(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOwnTempo(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagmaArmor(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWaterVeil(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddImmunity(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddScrappy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSoundproof(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLevitate(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlowerGift(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlashFire(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddForewarn(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAnticipation(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFrisk(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAftermath(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRunAway(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddColorChange(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMoldBreaker(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTruant(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLightningRod(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStormDrain(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSlowStart(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDamp(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWonderGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStall(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddForecast(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSuctionCups(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLiquidOoze(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddKlutz(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStickyHold(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPressure(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagicGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBadDreams(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPickup(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddUnburden(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddStench(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddShadowTag(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddArenaTrap(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagnetPull(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPickpocket(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSheerForce(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDefiant(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddDefeatist(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMultiscale(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHeavyMetal(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddLightMetal(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddContrary(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddUnnerve(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddCursedBody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHealer(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFriendGuard(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWeakArmor(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddToxicBoost(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddFlareBoost(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddHarvest(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddTelepathy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMoody(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddOvercoat(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPoisonTouch(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRegenerator(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddBigPecks(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandRush(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddWonderSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddAnalytic(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddIllusion(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddImposter(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddInfiltrator(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMummy(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMoxie(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddJustified(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRattled(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddMagicBounce(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSapSipper(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddPrankster(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddSandForce(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddRoughSkin(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddZenMode(u32* handlerAmount);
extern "C" BattleEventHandlerTableEntry* EventAddVictoryStar(u32* handlerAmount);

extern "C" u32 AbilityEvent_GetSubPriority(BattleMon* battleMon);
extern "C" BattleEventItem* AbilityEvent_AddItem(BattleMon* battleMon);
extern "C" void AbilityEvent_RemoveItem(BattleMon* battleMon);
extern "C" BattleEventPriority GetHandlerMainPriority(u32* handlerAmount);

extern "C" b32 j_j_IsRolePlayFailAbility(ABILITY ability);
extern "C" b32 j_CheckSkillSwapFailAbility(ABILITY ability);

#endif // __ABILITIES_H
