.thumb

#include "settings.h"

#if PERFECT_TRAINERS

FULL_COPY_TrainerUtil_LoadParty_0x4BA:
    nop
    nop
    nop
    ldr     r0, [sp,#0x14]  // PokemonParty
    movs    r1, r4          // PartyPkm
    ldr     r2, [sp,#0x70]  // TrainerData
    ldr     r3, [sp,#0x68]  // TrainerTeamData
    .size FULL_COPY_TrainerUtil_LoadParty_0x4BA, . - FULL_COPY_TrainerUtil_LoadParty_0x4BA

#endif // PERFECT_TRAINERS