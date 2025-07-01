.thumb

.type THUMB_BRANCH_LINK_CALCDMG_Set_0xFA, %function

#include "settings.h"

#if WEATHER_ABILITY_TURNS != 255

@ This changes the amount of turns weather will last after being summoned by an ability
FULL_COPY_CommonWeatherChangeAbility_0x2A:
    movs    r0, #WEATHER_ABILITY_TURNS
    .size FULL_COPY_CommonWeatherChangeAbility_0x2A, . - FULL_COPY_CommonWeatherChangeAbility_0x2A

#endif // WEATHER_ABILITY_TURNS != 255

#if EXPAND_FIELD_EFFECTS

FULL_COPY_BattleHandler_RemoveSideEffectCore_0x58:
    cmp     r4, #MAX_SIDE_EFFECT_AMOUNT
    .size FULL_COPY_BattleHandler_RemoveSideEffectCore_0x58, . - FULL_COPY_BattleHandler_RemoveSideEffectCore_0x58

#endif // EXPAND_FIELD_EFFECTS

#if EXPAND_ABILITIES || ADD_MEGA_EVOLUTION

@ Remove the if statements that check for Unnerve and add the checked ability as 
@ the third function parameter so that Unnerve and As One can be checked check inside 
@ the ServerControl_UnnerveAction function
FULL_COPY_ServerControl_AddCondition_0x132:
    nop
    movs    r2, r0
    .size FULL_COPY_ServerControl_AddCondition_0x132, . - FULL_COPY_ServerControl_AddCondition_0x132

FULL_COPY_ServerDisplay_SkillSwap_0x148:
    nop
    movs    r2, r4
    .size FULL_COPY_ServerDisplay_SkillSwap_0x148, . - FULL_COPY_ServerDisplay_SkillSwap_0x148

FULL_COPY_ServerDisplay_SkillSwap_0x16E:
    nop
    movs    r2, r6
    .size FULL_COPY_ServerDisplay_SkillSwap_0x16E, . - FULL_COPY_ServerDisplay_SkillSwap_0x16E

FULL_COPY_ServerControl_ClearMonDependentEffects_0xB6:
    nop
    movs    r2, r0
    .size FULL_COPY_ServerControl_ClearMonDependentEffects_0xB6, . - FULL_COPY_ServerControl_ClearMonDependentEffects_0xB6

FULL_COPY_BattleHandler_ChangeAbility_0x126:
    nop
    movs    r2, r6
    .size FULL_COPY_BattleHandler_ChangeAbility_0x126, . - FULL_COPY_BattleHandler_ChangeAbility_0x126

#endif // EXPAND_ABILITIES || ADD_MEGA_EVOLUTION

#if GEN7_WEAK_ARMOR

FULL_COPY_HandlerWeakArmor_0x9E:
    strb    r7, [r1,#0xC]
    .size FULL_COPY_HandlerWeakArmor_0x9E, . - FULL_COPY_HandlerWeakArmor_0x9E

#endif // GEN7_WEAK_ARMOR

#if GEN8_MOODY

FULL_COPY_HandlerMoody_0x76:
    cmp r5, #5
    .size FULL_COPY_HandlerMoody_0x76, . - FULL_COPY_HandlerMoody_0x76

#endif // GEN8_MOODY

#if GEN7_INFILTRATOR

THUMB_BRANCH_LINK_CALCDMG_Set_0xFA:
    ldr     r3, [sp,#0x18]  @ attackingMon
    push    {r7, lr}
    movs    r7, r0
    movs    r0, r3
    movs    r1, #0x11       @ VALUE_EFFECTIVE_ABILITY
    bl      BattleMon_GetValue
    cmp     r0, #151        @ ABIL_INFILTRATOR
    bne     Check_Substitute
    movs    r0, #0
    b       Exit_CALCDMG_Set
Check_Substitute:
    movs    r0, r7          @ targetMon
    bl      BattleMon_IsSubstituteActive
Exit_CALCDMG_Set:
    pop     {r7, pc}
    .size THUMB_BRANCH_LINK_CALCDMG_Set_0xFA, . - THUMB_BRANCH_LINK_CALCDMG_Set_0xFA

#endif // GEN7_INFILTRATOR

#if EXPAND_ITEMS

FULL_COPY_IsUnselectableMove_0xBE:
    movs    r1, r4
    .size FULL_COPY_IsUnselectableMove_0xBE, . - FULL_COPY_IsUnselectableMove_0xBE

FULL_COPY_IsUnselectableMove_0xD2:
    cmp    r0, #0
    .size FULL_COPY_IsUnselectableMove_0xD2, . - FULL_COPY_IsUnselectableMove_0xD2

FULL_COPY_HandlerDrySkinWeather_0x1A:
    movs    r0, r5
    .size FULL_COPY_HandlerDrySkinWeather_0x1A, . - FULL_COPY_HandlerDrySkinWeather_0x1A

FULL_COPY_HandlerSolarPowerWeather_0x12:
    movs    r0, r5
    .size FULL_COPY_HandlerSolarPowerWeather_0x12, . - FULL_COPY_HandlerSolarPowerWeather_0x12

FULL_COPY_HandlerSolarPowerPower_0x12:
    movs    r1, r5
    .size FULL_COPY_HandlerSolarPowerPower_0x12, . - FULL_COPY_HandlerSolarPowerPower_0x12

FULL_COPY_BattleHandler_ChangeAbility_0x10:
    movs    r1, r5
    .size FULL_COPY_BattleHandler_ChangeAbility_0x10, . - FULL_COPY_BattleHandler_ChangeAbility_0x10

FULL_COPY_ServerDisplay_SkillSwap_0x14:
    movs    r1, r5
    .size FULL_COPY_ServerDisplay_SkillSwap_0x14, . - FULL_COPY_ServerDisplay_SkillSwap_0x14

FULL_COPY_ServerDisplay_SkillSwap_0x1E:
    movs    r1, r5
    .size FULL_COPY_ServerDisplay_SkillSwap_0x1E, . - FULL_COPY_ServerDisplay_SkillSwap_0x1E

#endif // EXPAND_ITEMS

#if ADD_MEGA_EVOLUTION || (EXPAND_ABILITIES && EXPAND_ITEMS)

FULL_COPY_HandlerTrick_0x6C:
    nop
    .size FULL_COPY_HandlerTrick_0x6C, . - FULL_COPY_HandlerTrick_0x6C

FULL_COPY_HandlerTrick_0x82:
    ldr r0, [sp,#0x4]
    .size FULL_COPY_HandlerTrick_0x82, . - FULL_COPY_HandlerTrick_0x82

FULL_COPY_HandlerTrick_0x9A:
    ldr r0, [sp,#0x4]
    .size FULL_COPY_HandlerTrick_0x9A, . - FULL_COPY_HandlerTrick_0x9A

FULL_COPY_HandlerBestow_0x66:
    nop
    .size FULL_COPY_HandlerBestow_0x66, . - FULL_COPY_HandlerBestow_0x66

FULL_COPY_HandlerCommon_CheckUnchangeableItem_0x18:
    movs r0, r5
    .size FULL_COPY_HandlerCommon_CheckUnchangeableItem_0x18, . - FULL_COPY_HandlerCommon_CheckUnchangeableItem_0x18

FULL_COPY_HandlerCommon_CheckIfCanStealPokeItem_0x4A:
    movs r0, r4
    .size FULL_COPY_HandlerCommon_CheckIfCanStealPokeItem_0x4A, . - FULL_COPY_HandlerCommon_CheckIfCanStealPokeItem_0x4A

#endif // ADD_MEGA_EVOLUTION || (EXPAND_ABILITIES && EXPAND_ITEMS)

#if GEN6_CRIT

FULL_COPY_OVL_167_0x21D74E0:
    .byte 16
    .byte 8
    .byte 2
    .byte 1
    .byte 1
    .size FULL_COPY_OVL_167_0x21D74E0, . - FULL_COPY_OVL_167_0x21D74E0

#endif // GEN6_CRIT