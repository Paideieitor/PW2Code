#ifndef __COUNTERS_H
#define __COUNTERS_H

#include "defs.h"

typedef u8 COUNTER;
enum Counter : u8
{
    COUNTER_STOCKPILE = 0x0,
    COUNTER_STOCKPILE_DEF = 0x1,
    COUNTER_STOCKPILE_SPDEF = 0x2,
    COUNTER_PROTECT = 0x3,
    COUNTER_SKYDROP = 0x4,
};

#endif // __COUNTERS_H
