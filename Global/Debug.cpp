#include "gfl/core/gfl_heap.h"
#include "data/heapid_def.h"
#include "nds/gx.h"
#include "nds/mem.h"

void swap16(void* a, void* b) {
    u16* a_16 = static_cast<u16*>(a);
    u16* b_16 = static_cast<u16*>(b);
    u16 temp = *a_16;
    *a_16 = *b_16;
    *b_16 = temp;
}

void swapbuffer_memcpy32(void* a, void* b, size_t size) {
    //Not really optimized since we're not copying large amounts
    u32* a_32 = static_cast<u32*>(a);
    u32* b_32 = static_cast<u32*>(b);
    u32 swap;
    u32* end = b_32 + (size >> 2);
    while (b_32 < end) {
        swap = *a_32;
        *a_32 = *b_32;
        *b_32 = swap;
        a_32++;
        b_32++;
    }
}

extern "C" void PrintLowerScreen() {
#define TILES_PER_LINE 32;
#define TILE_AMOUNT 24

    size_t mapSize = TILE_AMOUNT * TILES_PER_LINE;
    u16* mapBase = GFL_NEW(HEAPID_SYSTEM) u16[mapSize];
    sys_memset32_fast(0, mapBase, mapSize * sizeof(u16));
    u16 paletteBGColor = 0x0000;

    BGCntReg bgCfg;
    bgCfg.Mosaic = 0;
    bgCfg.Priority = 0;
    bgCfg.ScreenSize = BGSIZE_T256x256_R128x128;
    bgCfg.ColorPaletteMode = 0;
    bgCfg.Bit12 = 0;
    bgCfg.CharBase = 0;
    bgCfg.ScreenBase = 2;

    u16 bgHOfs = 0;
    u16 bgVOfs = 0;
    u16 bldCnt = 0;
    u16 dispLayout = G2D_DISPLAYOUT_A_UP_B_DOWN;
    u16 enabledBGs = G2D_SCREENBG_1;
    u16 masterBright = 0;

    G2DBGTransformRegs BGTransform;
    memset(&BGTransform, 0, sizeof(BGTransform));

    if (gfxGetScreenAddrBG1B()) {
        swap16(&STD_PALETTE_BG_B.Colors[0xE], &paletteBGColor);

        swap16(&MASTER_BRIGHT_B, &masterBright);
        swap16(&LCDIO_B.BG1CNT, &bgCfg);
        swap16(&LCDIO_B.BG1HOFS, &bgHOfs);
        swap16(&LCDIO_B.BG1VOFS, &bgVOfs);
        swap16(&LCDIO_B.BLD.BLDCNT, &bldCnt);

        u16 bgs = DISPCNT_B.DispBGs;
        u16 dispLayout = POWCNT1.DispLayout;
        swap16(&enabledBGs, &bgs);
        swap16(&dispLayout, &dispLayout);
        DISPCNT_B.DispBGs = bgs;
        POWCNT1.DispLayout = dispLayout;

        swapbuffer_memcpy32(mapBase, gfxGetScreenAddrBG1B(), mapSize << 1);

    }
}