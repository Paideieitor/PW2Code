#ifndef __SPECIES_H
#define __SPECIES_H

typedef u32 SPECIES;
enum Species : u32
{
	PKM_NULL = 0x0,
	PKM_BULBASAUR = 0x1,
	PKM_IVYSAUR = 0x2,
	PKM_VENUSAUR = 0x3,
	PKM_CHARMANDER = 0x4,
	PKM_CHARMELEON = 0x5,
	PKM_CHARIZARD = 0x6,
	PKM_SQUIRTLE = 0x7,
	PKM_WARTORTLE = 0x8,
	PKM_BLASTOISE = 0x9,
	PKM_CATERPIE = 0xA,
	PKM_METAPOD = 0xB,
	PKM_BUTTERFREE = 0xC,
	PKM_WEEDLE = 0xD,
	PKM_KAKUNA = 0xE,
	PKM_BEEDRILL = 0xF,
	PKM_PIDGEY = 0x10,
	PKM_PIDGEOTTO = 0x11,
	PKM_PIDGEOT = 0x12,
	PKM_RATTATA = 0x13,
	PKM_RATICATE = 0x14,
	PKM_SPEAROW = 0x15,
	PKM_FEAROW = 0x16,
	PKM_EKANS = 0x17,
	PKM_ARBOK = 0x18,
	PKM_PIKACHU = 0x19,
	PKM_RAICHU = 0x1A,
	PKM_SANDSHREW = 0x1B,
	PKM_SANDSLASH = 0x1C,
	PKM_NIDORAN_F = 0x1D,
	PKM_NIDORINA = 0x1E,
	PKM_NIDOQUEEN = 0x1F,
	PKM_NIDORAN_M = 0x20,
	PKM_NIDORINO = 0x21,
	PKM_NIDOKING = 0x22,
	PKM_CLEFAIRY = 0x23,
	PKM_CLEFABLE = 0x24,
	PKM_VULPIX = 0x25,
	PKM_NINETALES = 0x26,
	PKM_JIGGLYPUFF = 0x27,
	PKM_WIGGLYTUFF = 0x28,
	PKM_ZUBAT = 0x29,
	PKM_GOLBAT = 0x2A,
	PKM_ODDISH = 0x2B,
	PKM_GLOOM = 0x2C,
	PKM_VILEPLUME = 0x2D,
	PKM_PARAS = 0x2E,
	PKM_PARASECT = 0x2F,
	PKM_VENONAT = 0x30,
	PKM_VENOMOTH = 0x31,
	PKM_DIGLETT = 0x32,
	PKM_DUGTRIO = 0x33,
	PKM_MEOWTH = 0x34,
	PKM_PERSIAN = 0x35,
	PKM_PSYDUCK = 0x36,
	PKM_GOLDUCK = 0x37,
	PKM_MANKEY = 0x38,
	PKM_PRIMEAPE = 0x39,
	PKM_GROWLITHE = 0x3A,
	PKM_ARCANINE = 0x3B,
	PKM_POLIWAG = 0x3C,
	PKM_POLIWHIRL = 0x3D,
	PKM_POLIWRATH = 0x3E,
	PKM_ABRA = 0x3F,
	PKM_KADABRA = 0x40,
	PKM_ALAKAZAM = 0x41,
	PKM_MACHOP = 0x42,
	PKM_MACHOKE = 0x43,
	PKM_MACHAMP = 0x44,
	PKM_BELLSPROUT = 0x45,
	PKM_WEEPINBELL = 0x46,
	PKM_VICTREEBEL = 0x47,
	PKM_TENTACOOL = 0x48,
	PKM_TENTACRUEL = 0x49,
	PKM_GEODUDE = 0x4A,
	PKM_GRAVELER = 0x4B,
	PKM_GOLEM = 0x4C,
	PKM_PONYTA = 0x4D,
	PKM_RAPIDASH = 0x4E,
	PKM_SLOWPOKE = 0x4F,
	PKM_SLOWBRO = 0x50,
	PKM_MAGNEMITE = 0x51,
	PKM_MAGNETON = 0x52,
	PKM_FARFETCH_D = 0x53,
	PKM_DODUO = 0x54,
	PKM_DODRIO = 0x55,
	PKM_SEEL = 0x56,
	PKM_DEWGONG = 0x57,
	PKM_GRIMER = 0x58,
	PKM_MUK = 0x59,
	PKM_SHELLDER = 0x5A,
	PKM_CLOYSTER = 0x5B,
	PKM_GASTLY = 0x5C,
	PKM_HAUNTER = 0x5D,
	PKM_GENGAR = 0x5E,
	PKM_ONIX = 0x5F,
	PKM_DROWZEE = 0x60,
	PKM_HYPNO = 0x61,
	PKM_KRABBY = 0x62,
	PKM_KINGLER = 0x63,
	PKM_VOLTORB = 0x64,
	PKM_ELECTRODE = 0x65,
	PKM_EXEGGCUTE = 0x66,
	PKM_EXEGGUTOR = 0x67,
	PKM_CUBONE = 0x68,
	PKM_MAROWAK = 0x69,
	PKM_HITMONLEE = 0x6A,
	PKM_HITMONCHAN = 0x6B,
	PKM_LICKITUNG = 0x6C,
	PKM_KOFFING = 0x6D,
	PKM_WEEZING = 0x6E,
	PKM_RHYHORN = 0x6F,
	PKM_RHYDON = 0x70,
	PKM_CHANSEY = 0x71,
	PKM_TANGELA = 0x72,
	PKM_KANGASKHAN = 0x73,
	PKM_HORSEA = 0x74,
	PKM_SEADRA = 0x75,
	PKM_GOLDEEN = 0x76,
	PKM_SEAKING = 0x77,
	PKM_STARYU = 0x78,
	PKM_STARMIE = 0x79,
	PKM_MR_MIME = 0x7A,
	PKM_SCYTHER = 0x7B,
	PKM_JYNX = 0x7C,
	PKM_ELECTABUZZ = 0x7D,
	PKM_MAGMAR = 0x7E,
	PKM_PINSIR = 0x7F,
	PKM_TAUROS = 0x80,
	PKM_MAGIKARP = 0x81,
	PKM_GYARADOS = 0x82,
	PKM_LAPRAS = 0x83,
	PKM_DITTO = 0x84,
	PKM_EEVEE = 0x85,
	PKM_VAPOREON = 0x86,
	PKM_JOLTEON = 0x87,
	PKM_FLAREON = 0x88,
	PKM_PORYGON = 0x89,
	PKM_OMANYTE = 0x8A,
	PKM_OMASTAR = 0x8B,
	PKM_KABUTO = 0x8C,
	PKM_KABUTOPS = 0x8D,
	PKM_AERODACTYL = 0x8E,
	PKM_SNORLAX = 0x8F,
	PKM_ARTICUNO = 0x90,
	PKM_ZAPDOS = 0x91,
	PKM_MOLTRES = 0x92,
	PKM_DRATINI = 0x93,
	PKM_DRAGONAIR = 0x94,
	PKM_DRAGONITE = 0x95,
	PKM_MEWTWO = 0x96,
	PKM_MEW = 0x97,
	PKM_CHIKORITA = 0x98,
	PKM_BAYLEEF = 0x99,
	PKM_MEGANIUM = 0x9A,
	PKM_CYNDAQUIL = 0x9B,
	PKM_QUILAVA = 0x9C,
	PKM_TYPHLOSION = 0x9D,
	PKM_TOTODILE = 0x9E,
	PKM_CROCONAW = 0x9F,
	PKM_FERALIGATR = 0xA0,
	PKM_SENTRET = 0xA1,
	PKM_FURRET = 0xA2,
	PKM_HOOTHOOT = 0xA3,
	PKM_NOCTOWL = 0xA4,
	PKM_LEDYBA = 0xA5,
	PKM_LEDIAN = 0xA6,
	PKM_SPINARAK = 0xA7,
	PKM_ARIADOS = 0xA8,
	PKM_CROBAT = 0xA9,
	PKM_CHINCHOU = 0xAA,
	PKM_LANTURN = 0xAB,
	PKM_PICHU = 0xAC,
	PKM_CLEFFA = 0xAD,
	PKM_IGGLYBUFF = 0xAE,
	PKM_TOGEPI = 0xAF,
	PKM_TOGETIC = 0xB0,
	PKM_NATU = 0xB1,
	PKM_XATU = 0xB2,
	PKM_MAREEP = 0xB3,
	PKM_FLAAFFY = 0xB4,
	PKM_AMPHAROS = 0xB5,
	PKM_BELLOSSOM = 0xB6,
	PKM_MARILL = 0xB7,
	PKM_AZUMARILL = 0xB8,
	PKM_SUDOWOODO = 0xB9,
	PKM_POLITOED = 0xBA,
	PKM_HOPPIP = 0xBB,
	PKM_SKIPLOOM = 0xBC,
	PKM_JUMPLUFF = 0xBD,
	PKM_AIPOM = 0xBE,
	PKM_SUNKERN = 0xBF,
	PKM_SUNFLORA = 0xC0,
	PKM_YANMA = 0xC1,
	PKM_WOOPER = 0xC2,
	PKM_QUAGSIRE = 0xC3,
	PKM_ESPEON = 0xC4,
	PKM_UMBREON = 0xC5,
	PKM_MURKROW = 0xC6,
	PKM_SLOWKING = 0xC7,
	PKM_MISDREAVUS = 0xC8,
	PKM_UNOWN = 0xC9,
	PKM_WOBBUFFET = 0xCA,
	PKM_GIRAFARIG = 0xCB,
	PKM_PINECO = 0xCC,
	PKM_FORRETRESS = 0xCD,
	PKM_DUNSPARCE = 0xCE,
	PKM_GLIGAR = 0xCF,
	PKM_STEELIX = 0xD0,
	PKM_SNUBBULL = 0xD1,
	PKM_GRANBULL = 0xD2,
	PKM_QWILFISH = 0xD3,
	PKM_SCIZOR = 0xD4,
	PKM_SHUCKLE = 0xD5,
	PKM_HERACROSS = 0xD6,
	PKM_SNEASEL = 0xD7,
	PKM_TEDDIURSA = 0xD8,
	PKM_URSARING = 0xD9,
	PKM_SLUGMA = 0xDA,
	PKM_MAGCARGO = 0xDB,
	PKM_SWINUB = 0xDC,
	PKM_PILOSWINE = 0xDD,
	PKM_CORSOLA = 0xDE,
	PKM_REMORAID = 0xDF,
	PKM_OCTILLERY = 0xE0,
	PKM_DELIBIRD = 0xE1,
	PKM_MANTINE = 0xE2,
	PKM_SKARMORY = 0xE3,
	PKM_HOUNDOUR = 0xE4,
	PKM_HOUNDOOM = 0xE5,
	PKM_KINGDRA = 0xE6,
	PKM_PHANPY = 0xE7,
	PKM_DONPHAN = 0xE8,
	PKM_PORYGON2 = 0xE9,
	PKM_STANTLER = 0xEA,
	PKM_SMEARGLE = 0xEB,
	PKM_TYROGUE = 0xEC,
	PKM_HITMONTOP = 0xED,
	PKM_SMOOCHUM = 0xEE,
	PKM_ELEKID = 0xEF,
	PKM_MAGBY = 0xF0,
	PKM_MILTANK = 0xF1,
	PKM_BLISSEY = 0xF2,
	PKM_RAIKOU = 0xF3,
	PKM_ENTEI = 0xF4,
	PKM_SUICUNE = 0xF5,
	PKM_LARVITAR = 0xF6,
	PKM_PUPITAR = 0xF7,
	PKM_TYRANITAR = 0xF8,
	PKM_LUGIA = 0xF9,
	PKM_HO_OH = 0xFA,
	PKM_CELEBI = 0xFB,
	PKM_TREECKO = 0xFC,
	PKM_GROVYLE = 0xFD,
	PKM_SCEPTILE = 0xFE,
	PKM_TORCHIC = 0xFF,
	PKM_COMBUSKEN = 0x100,
	PKM_BLAZIKEN = 0x101,
	PKM_MUDKIP = 0x102,
	PKM_MARSHTOMP = 0x103,
	PKM_SWAMPERT = 0x104,
	PKM_POOCHYENA = 0x105,
	PKM_MIGHTYENA = 0x106,
	PKM_ZIGZAGOON = 0x107,
	PKM_LINOONE = 0x108,
	PKM_WURMPLE = 0x109,
	PKM_SILCOON = 0x10A,
	PKM_BEAUTIFLY = 0x10B,
	PKM_CASCOON = 0x10C,
	PKM_DUSTOX = 0x10D,
	PKM_LOTAD = 0x10E,
	PKM_LOMBRE = 0x10F,
	PKM_LUDICOLO = 0x110,
	PKM_SEEDOT = 0x111,
	PKM_NUZLEAF = 0x112,
	PKM_SHIFTRY = 0x113,
	PKM_TAILLOW = 0x114,
	PKM_SWELLOW = 0x115,
	PKM_WINGULL = 0x116,
	PKM_PELIPPER = 0x117,
	PKM_RALTS = 0x118,
	PKM_KIRLIA = 0x119,
	PKM_GARDEVOIR = 0x11A,
	PKM_SURSKIT = 0x11B,
	PKM_MASQUERAIN = 0x11C,
	PKM_SHROOMISH = 0x11D,
	PKM_BRELOOM = 0x11E,
	PKM_SLAKOTH = 0x11F,
	PKM_VIGOROTH = 0x120,
	PKM_SLAKING = 0x121,
	PKM_NINCADA = 0x122,
	PKM_NINJASK = 0x123,
	PKM_SHEDINJA = 0x124,
	PKM_WHISMUR = 0x125,
	PKM_LOUDRED = 0x126,
	PKM_EXPLOUD = 0x127,
	PKM_MAKUHITA = 0x128,
	PKM_HARIYAMA = 0x129,
	PKM_AZURILL = 0x12A,
	PKM_NOSEPASS = 0x12B,
	PKM_SKITTY = 0x12C,
	PKM_DELCATTY = 0x12D,
	PKM_SABLEYE = 0x12E,
	PKM_MAWILE = 0x12F,
	PKM_ARON = 0x130,
	PKM_LAIRON = 0x131,
	PKM_AGGRON = 0x132,
	PKM_MEDITITE = 0x133,
	PKM_MEDICHAM = 0x134,
	PKM_ELECTRIKE = 0x135,
	PKM_MANECTRIC = 0x136,
	PKM_PLUSLE = 0x137,
	PKM_MINUN = 0x138,
	PKM_VOLBEAT = 0x139,
	PKM_ILLUMISE = 0x13A,
	PKM_ROSELIA = 0x13B,
	PKM_GULPIN = 0x13C,
	PKM_SWALOT = 0x13D,
	PKM_CARVANHA = 0x13E,
	PKM_SHARPEDO = 0x13F,
	PKM_WAILMER = 0x140,
	PKM_WAILORD = 0x141,
	PKM_NUMEL = 0x142,
	PKM_CAMERUPT = 0x143,
	PKM_TORKOAL = 0x144,
	PKM_SPOINK = 0x145,
	PKM_GRUMPIG = 0x146,
	PKM_SPINDA = 0x147,
	PKM_TRAPINCH = 0x148,
	PKM_VIBRAVA = 0x149,
	PKM_FLYGON = 0x14A,
	PKM_CACNEA = 0x14B,
	PKM_CACTURNE = 0x14C,
	PKM_SWABLU = 0x14D,
	PKM_ALTARIA = 0x14E,
	PKM_ZANGOOSE = 0x14F,
	PKM_SEVIPER = 0x150,
	PKM_LUNATONE = 0x151,
	PKM_SOLROCK = 0x152,
	PKM_BARBOACH = 0x153,
	PKM_WHISCASH = 0x154,
	PKM_CORPHISH = 0x155,
	PKM_CRAWDAUNT = 0x156,
	PKM_BALTOY = 0x157,
	PKM_CLAYDOL = 0x158,
	PKM_LILEEP = 0x159,
	PKM_CRADILY = 0x15A,
	PKM_ANORITH = 0x15B,
	PKM_ARMALDO = 0x15C,
	PKM_FEEBAS = 0x15D,
	PKM_MILOTIC = 0x15E,
	PKM_CASTFORM = 0x15F,
	PKM_KECLEON = 0x160,
	PKM_SHUPPET = 0x161,
	PKM_BANETTE = 0x162,
	PKM_DUSKULL = 0x163,
	PKM_DUSCLOPS = 0x164,
	PKM_TROPIUS = 0x165,
	PKM_CHIMECHO = 0x166,
	PKM_ABSOL = 0x167,
	PKM_WYNAUT = 0x168,
	PKM_SNORUNT = 0x169,
	PKM_GLALIE = 0x16A,
	PKM_SPHEAL = 0x16B,
	PKM_SEALEO = 0x16C,
	PKM_WALREIN = 0x16D,
	PKM_CLAMPERL = 0x16E,
	PKM_HUNTAIL = 0x16F,
	PKM_GOREBYSS = 0x170,
	PKM_RELICANTH = 0x171,
	PKM_LUVDISC = 0x172,
	PKM_BAGON = 0x173,
	PKM_SHELGON = 0x174,
	PKM_SALAMENCE = 0x175,
	PKM_BELDUM = 0x176,
	PKM_METANG = 0x177,
	PKM_METAGROSS = 0x178,
	PKM_REGIROCK = 0x179,
	PKM_REGICE = 0x17A,
	PKM_REGISTEEL = 0x17B,
	PKM_LATIAS = 0x17C,
	PKM_LATIOS = 0x17D,
	PKM_KYOGRE = 0x17E,
	PKM_GROUDON = 0x17F,
	PKM_RAYQUAZA = 0x180,
	PKM_JIRACHI = 0x181,
	PKM_DEOXYS = 0x182,
	PKM_TURTWIG = 0x183,
	PKM_GROTLE = 0x184,
	PKM_TORTERRA = 0x185,
	PKM_CHIMCHAR = 0x186,
	PKM_MONFERNO = 0x187,
	PKM_INFERNAPE = 0x188,
	PKM_PIPLUP = 0x189,
	PKM_PRINPLUP = 0x18A,
	PKM_EMPOLEON = 0x18B,
	PKM_STARLY = 0x18C,
	PKM_STARAVIA = 0x18D,
	PKM_STARAPTOR = 0x18E,
	PKM_BIDOOF = 0x18F,
	PKM_BIBAREL = 0x190,
	PKM_KRICKETOT = 0x191,
	PKM_KRICKETUNE = 0x192,
	PKM_SHINX = 0x193,
	PKM_LUXIO = 0x194,
	PKM_LUXRAY = 0x195,
	PKM_BUDEW = 0x196,
	PKM_ROSERADE = 0x197,
	PKM_CRANIDOS = 0x198,
	PKM_RAMPARDOS = 0x199,
	PKM_SHIELDON = 0x19A,
	PKM_BASTIODON = 0x19B,
	PKM_BURMY = 0x19C,
	PKM_WORMADAM = 0x19D,
	PKM_MOTHIM = 0x19E,
	PKM_COMBEE = 0x19F,
	PKM_VESPIQUEN = 0x1A0,
	PKM_PACHIRISU = 0x1A1,
	PKM_BUIZEL = 0x1A2,
	PKM_FLOATZEL = 0x1A3,
	PKM_CHERUBI = 0x1A4,
	PKM_CHERRIM = 0x1A5,
	PKM_SHELLOS = 0x1A6,
	PKM_GASTRODON = 0x1A7,
	PKM_AMBIPOM = 0x1A8,
	PKM_DRIFLOON = 0x1A9,
	PKM_DRIFBLIM = 0x1AA,
	PKM_BUNEARY = 0x1AB,
	PKM_LOPUNNY = 0x1AC,
	PKM_MISMAGIUS = 0x1AD,
	PKM_HONCHKROW = 0x1AE,
	PKM_GLAMEOW = 0x1AF,
	PKM_PURUGLY = 0x1B0,
	PKM_CHINGLING = 0x1B1,
	PKM_STUNKY = 0x1B2,
	PKM_SKUNTANK = 0x1B3,
	PKM_BRONZOR = 0x1B4,
	PKM_BRONZONG = 0x1B5,
	PKM_BONSLY = 0x1B6,
	PKM_MIME_JR = 0x1B7,
	PKM_HAPPINY = 0x1B8,
	PKM_CHATOT = 0x1B9,
	PKM_SPIRITOMB = 0x1BA,
	PKM_GIBLE = 0x1BB,
	PKM_GABITE = 0x1BC,
	PKM_GARCHOMP = 0x1BD,
	PKM_MUNCHLAX = 0x1BE,
	PKM_RIOLU = 0x1BF,
	PKM_LUCARIO = 0x1C0,
	PKM_HIPPOPOTAS = 0x1C1,
	PKM_HIPPOWDON = 0x1C2,
	PKM_SKORUPI = 0x1C3,
	PKM_DRAPION = 0x1C4,
	PKM_CROAGUNK = 0x1C5,
	PKM_TOXICROAK = 0x1C6,
	PKM_CARNIVINE = 0x1C7,
	PKM_FINNEON = 0x1C8,
	PKM_LUMINEON = 0x1C9,
	PKM_MANTYKE = 0x1CA,
	PKM_SNOVER = 0x1CB,
	PKM_ABOMASNOW = 0x1CC,
	PKM_WEAVILE = 0x1CD,
	PKM_MAGNEZONE = 0x1CE,
	PKM_LICKILICKY = 0x1CF,
	PKM_RHYPERIOR = 0x1D0,
	PKM_TANGROWTH = 0x1D1,
	PKM_ELECTIVIRE = 0x1D2,
	PKM_MAGMORTAR = 0x1D3,
	PKM_TOGEKISS = 0x1D4,
	PKM_YANMEGA = 0x1D5,
	PKM_LEAFEON = 0x1D6,
	PKM_GLACEON = 0x1D7,
	PKM_GLISCOR = 0x1D8,
	PKM_MAMOSWINE = 0x1D9,
	PKM_PORYGON_Z = 0x1DA,
	PKM_GALLADE = 0x1DB,
	PKM_PROBOPASS = 0x1DC,
	PKM_DUSKNOIR = 0x1DD,
	PKM_FROSLASS = 0x1DE,
	PKM_ROTOM = 0x1DF,
	PKM_UXIE = 0x1E0,
	PKM_MESPRIT = 0x1E1,
	PKM_AZELF = 0x1E2,
	PKM_DIALGA = 0x1E3,
	PKM_PALKIA = 0x1E4,
	PKM_HEATRAN = 0x1E5,
	PKM_REGIGIGAS = 0x1E6,
	PKM_GIRATINA = 0x1E7,
	PKM_CRESSELIA = 0x1E8,
	PKM_PHIONE = 0x1E9,
	PKM_MANAPHY = 0x1EA,
	PKM_DARKRAI = 0x1EB,
	PKM_SHAYMIN = 0x1EC,
	PKM_ARCEUS = 0x1ED,
	PKM_VICTINI = 0x1EE,
	PKM_SNIVY = 0x1EF,
	PKM_SERVINE = 0x1F0,
	PKM_SERPERIOR = 0x1F1,
	PKM_TEPIG = 0x1F2,
	PKM_PIGNITE = 0x1F3,
	PKM_EMBOAR = 0x1F4,
	PKM_OSHAWOTT = 0x1F5,
	PKM_DEWOTT = 0x1F6,
	PKM_SAMUROTT = 0x1F7,
	PKM_PATRAT = 0x1F8,
	PKM_WATCHOG = 0x1F9,
	PKM_LILLIPUP = 0x1FA,
	PKM_HERDIER = 0x1FB,
	PKM_STOUTLAND = 0x1FC,
	PKM_PURRLOIN = 0x1FD,
	PKM_LIEPARD = 0x1FE,
	PKM_PANSAGE = 0x1FF,
	PKM_SIMISAGE = 0x200,
	PKM_PANSEAR = 0x201,
	PKM_SIMISEAR = 0x202,
	PKM_PANPOUR = 0x203,
	PKM_SIMIPOUR = 0x204,
	PKM_MUNNA = 0x205,
	PKM_MUSHARNA = 0x206,
	PKM_PIDOVE = 0x207,
	PKM_TRANQUILL = 0x208,
	PKM_UNFEZANT = 0x209,
	PKM_BLITZLE = 0x20A,
	PKM_ZEBSTRIKA = 0x20B,
	PKM_ROGGENROLA = 0x20C,
	PKM_BOLDORE = 0x20D,
	PKM_GIGALITH = 0x20E,
	PKM_WOOBAT = 0x20F,
	PKM_SWOOBAT = 0x210,
	PKM_DRILBUR = 0x211,
	PKM_EXCADRILL = 0x212,
	PKM_AUDINO = 0x213,
	PKM_TIMBURR = 0x214,
	PKM_GURDURR = 0x215,
	PKM_CONKELDURR = 0x216,
	PKM_TYMPOLE = 0x217,
	PKM_PALPITOAD = 0x218,
	PKM_SEISMITOAD = 0x219,
	PKM_THROH = 0x21A,
	PKM_SAWK = 0x21B,
	PKM_SEWADDLE = 0x21C,
	PKM_SWADLOON = 0x21D,
	PKM_LEAVANNY = 0x21E,
	PKM_VENIPEDE = 0x21F,
	PKM_WHIRLIPEDE = 0x220,
	PKM_SCOLIPEDE = 0x221,
	PKM_COTTONEE = 0x222,
	PKM_WHIMSICOTT = 0x223,
	PKM_PETILIL = 0x224,
	PKM_LILLIGANT = 0x225,
	PKM_BASCULIN = 0x226,
	PKM_SANDILE = 0x227,
	PKM_KROKOROK = 0x228,
	PKM_KROOKODILE = 0x229,
	PKM_DARUMAKA = 0x22A,
	PKM_DARMANITAN = 0x22B,
	PKM_MARACTUS = 0x22C,
	PKM_DWEBBLE = 0x22D,
	PKM_CRUSTLE = 0x22E,
	PKM_SCRAGGY = 0x22F,
	PKM_SCRAFTY = 0x230,
	PKM_SIGILYPH = 0x231,
	PKM_YAMASK = 0x232,
	PKM_COFAGRIGUS = 0x233,
	PKM_TIRTOUGA = 0x234,
	PKM_CARRACOSTA = 0x235,
	PKM_ARCHEN = 0x236,
	PKM_ARCHEOPS = 0x237,
	PKM_TRUBBISH = 0x238,
	PKM_GARBODOR = 0x239,
	PKM_ZORUA = 0x23A,
	PKM_ZOROARK = 0x23B,
	PKM_MINCCINO = 0x23C,
	PKM_CINCCINO = 0x23D,
	PKM_GOTHITA = 0x23E,
	PKM_GOTHORITA = 0x23F,
	PKM_GOTHITELLE = 0x240,
	PKM_SOLOSIS = 0x241,
	PKM_DUOSION = 0x242,
	PKM_REUNICLUS = 0x243,
	PKM_DUCKLETT = 0x244,
	PKM_SWANNA = 0x245,
	PKM_VANILLITE = 0x246,
	PKM_VANILLISH = 0x247,
	PKM_VANILLUXE = 0x248,
	PKM_DEERLING = 0x249,
	PKM_SAWSBUCK = 0x24A,
	PKM_EMOLGA = 0x24B,
	PKM_KARRABLAST = 0x24C,
	PKM_ESCAVALIER = 0x24D,
	PKM_FOONGUS = 0x24E,
	PKM_AMOONGUSS = 0x24F,
	PKM_FRILLISH = 0x250,
	PKM_JELLICENT = 0x251,
	PKM_ALOMOMOLA = 0x252,
	PKM_JOLTIK = 0x253,
	PKM_GALVANTULA = 0x254,
	PKM_FERROSEED = 0x255,
	PKM_FERROTHORN = 0x256,
	PKM_KLINK = 0x257,
	PKM_KLANG = 0x258,
	PKM_KLINKLANG = 0x259,
	PKM_TYNAMO = 0x25A,
	PKM_EELEKTRIK = 0x25B,
	PKM_EELEKTROSS = 0x25C,
	PKM_ELGYEM = 0x25D,
	PKM_BEHEEYEM = 0x25E,
	PKM_LITWICK = 0x25F,
	PKM_LAMPENT = 0x260,
	PKM_CHANDELURE = 0x261,
	PKM_AXEW = 0x262,
	PKM_FRAXURE = 0x263,
	PKM_HAXORUS = 0x264,
	PKM_CUBCHOO = 0x265,
	PKM_BEARTIC = 0x266,
	PKM_CRYOGONAL = 0x267,
	PKM_SHELMET = 0x268,
	PKM_ACCELGOR = 0x269,
	PKM_STUNFISK = 0x26A,
	PKM_MIENFOO = 0x26B,
	PKM_MIENSHAO = 0x26C,
	PKM_DRUDDIGON = 0x26D,
	PKM_GOLETT = 0x26E,
	PKM_GOLURK = 0x26F,
	PKM_PAWNIARD = 0x270,
	PKM_BISHARP = 0x271,
	PKM_BOUFFALANT = 0x272,
	PKM_RUFFLET = 0x273,
	PKM_BRAVIARY = 0x274,
	PKM_VULLABY = 0x275,
	PKM_MANDIBUZZ = 0x276,
	PKM_HEATMOR = 0x277,
	PKM_DURANT = 0x278,
	PKM_DEINO = 0x279,
	PKM_ZWEILOUS = 0x27A,
	PKM_HYDREIGON = 0x27B,
	PKM_LARVESTA = 0x27C,
	PKM_VOLCARONA = 0x27D,
	PKM_COBALION = 0x27E,
	PKM_TERRAKION = 0x27F,
	PKM_VIRIZION = 0x280,
	PKM_TORNADUS = 0x281,
	PKM_THUNDURUS = 0x282,
	PKM_RESHIRAM = 0x283,
	PKM_ZEKROM = 0x284,
	PKM_LANDORUS = 0x285,
	PKM_KYUREM = 0x286,
	PKM_KELDEO = 0x287,
	PKM_MELOETTA = 0x288,
	PKM_GENESECT = 0x289,
	PKM_MAX = 0x28A,
};

typedef u32 ITEM;

extern "C" b32 HandlerCommon_IsUnremovableItem(SPECIES species, ITEM itemID);

#endif // __SPECIES_H
