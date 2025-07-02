#include "settings.h"
#include "FileSystem.h"

#include "types.h"

#define TYPE_CHART_SIZE 0x144

extern "C" TypeEffectiveness THUMB_BRANCH_GetTypeEffectiveness(POKE_TYPE moveType, POKE_TYPE pokeType) {
    if (moveType >= TYPE_NULL || pokeType >= TYPE_NULL) {
        return EFFECTIVENESS_1;
    }

    u8 typeChart[TYPE_CHART_SIZE];
    if (ReadDataFromFile("type_chart.bin", TYPE_CHART_SIZE, typeChart)) {
        switch (typeChart[TYPE_NULL * moveType + pokeType]) {
        case NOT_EFFECTIVE: {
            return EFFECTIVENESS_IMMUNE;
        }
        case NOT_VERY_EFFECTIVE: {
            return EFFECTIVENESS_1_2;
        }
        case EFFECTIVE: {
            return EFFECTIVENESS_1;
        }
        case SUPER_EFFECTIVE: {
            return EFFECTIVENESS_2;
        }
        default: {
            return EFFECTIVENESS_IMMUNE;
        }
        }
    }
    return EFFECTIVENESS_1;
}

extern "C" s32 THUMB_BRANCH_GetTypeWeaknesses(POKE_TYPE type, u8* weakness) {
    if (type >= TYPE_NULL) {
        return 0;
    }

    u8 typeChart[TYPE_CHART_SIZE];
    if (ReadDataFromFile("type_chart.bin", TYPE_CHART_SIZE, typeChart)) {
        u32 weaknessIdx = 0;
        u32 row = TYPE_NULL * type;

        for (u32 i = 0; i < TYPE_NULL; ++i) {
            if (!typeChart[row + i] || 
                typeChart[row + i] == NOT_VERY_EFFECTIVE) {
                weakness[weaknessIdx++] = i;
            }
        }
        return weaknessIdx;
    }
    return 0;
}

extern "C" u32 THUMB_BRANCH_GetPokemonTypeIconPalette(POKE_TYPE type)
{
    return ReadByteFromFile("type_palette_map.bin", type);
}
