#ifndef __FTL_UTILS_H__
#define __FTL_UTILS_H__

#include <string.h>
#include "common_header.h"
#include "struct_meta.h"
#include "generic_list.h"

#define LOWER_BOUND(x, y) (x)
#define HIGHER_BOUND(x, y) (x + y - 1)

uint getBitWidth(uint num);
void computeSpecBitWidth(sNandSpec *spec);

void setPageReq(uint *startSector, uchar *validMap, DiskReq cmd);
uchar pageChunkGen(uint iStartAdr, uint iSectorCnt, uint iSectorPerPage, uint order,
	uint *iAdr, uint *iLen, uchar *validMap);

uchar isDifferentValidMap(uchar *a, uchar *b, uint size);
void mergeValidMap(uchar *target, uchar *a, uchar *b, uint size);

sNandAddr composeNandAddr(uint iCh, uint iWy, uint iPl, uint iBk, uint iPg, uint iSe);
sNandCmd composeNandCmd(eNandOp op, sNandAddr des, sNandAddr src, uint iLogicalAdr, uchar* validMap);

void setMapBuffer(sNandCmd *cmd, sPgBuf *dataBuf);

void ftlAssert(uchar condition, const char *msg);


void ACQUIRE_LOCK(uint x);
void RELEASE_LOCK(uint x);

#endif
