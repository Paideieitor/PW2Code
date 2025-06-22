#ifndef __CONTACT_H
#define __CONTACT_H

#include "server_flow.h"

extern "C" b32 IsContact(ServerFlow * serverFlow, MOVE_ID moveID, u32 attackingSlot);
extern "C" b32 MakesContact(ServerFlow * serverFlow, MOVE_ID moveID, u32 attackingSlot, u32 defendingSlot);

#endif // __CONTACT_H
