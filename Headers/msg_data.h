#ifndef __MSG_DATA_H
#define __MSG_DATA_H

#include "defs.h"

struct ArcTool;

struct MsgFileHeader
{
	u16 sectionCount;
	u16 lineCount;
	u32 dataBottomOfs;
	u32 reserved;
};

struct MsgFileHandle
{
	MsgFileHeader header;
	u32 dataTopOffsets[1];
};

struct MsgFileEntry
{
	u32 offset;
	u16 charCount;
	u16 padding;
};

struct SWAN_ALIGNED(4) MsgData
{
	MsgFileHandle* fileHandle;
	u32 rawData;
	MsgFileEntry currentEntry;
	u32 msgOfsInArc;
	ArcTool* msgArc;
	u16 heapID;
	u8 languageId;
	u8 isAllPreload;
};

struct WordSetBuffer
{
	u8 field_0;
	u8 field_1;
	u8 field_2;
	u8 field_3;
	u8 skip84Pixels;
	u8 pad1;
	u16 pad2;
	StrBuf* strbuf;
};

struct WordSetSystem
{
	u32 wordSetCount;
	u32 heapID;
	WordSetBuffer* buffers;
	StrBuf* mainStrbuf;
	u32 field_10;
};

extern "C" void GFL_MsgDataLoadStrbuf(MsgData * msgdata, u32 msgID, StrBuf * strbuf);

extern "C" int GFL_WordSetFormatStrbuf(WordSetSystem * wordSet, StrBuf * dest, StrBuf * source);
extern "C" void copyVarForText(WordSetSystem * wordSet, int wsBufNo, void* a3);

#endif // __MSG_DATA_H
