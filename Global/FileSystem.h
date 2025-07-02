#ifndef __FILE_SYSTEM_H
#define __FILE_SYSTEM_H

#include "settings.h"

#include "defs.h"

#if ADD_FAIRY_TYPE

extern "C" b32 ReadDataFromFile(const char* fileName, const u32 bufferSize, u8 *buffer);

extern "C" u8 ReadByteFromFile(const char* fileName, const u32 idx);

#endif // ADD_FAIRY_TYPE

#endif // __FILE_SYSTEM_H