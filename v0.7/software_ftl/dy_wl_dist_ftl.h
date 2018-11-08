#ifndef __DY_WL_DIST_FTL_H__
#define __DY_WL_DIST_FTL_H__

#include "dynamic_wl_dftl.h"

typedef struct blockCluster{
	uint iStartAddr;
	uint iSize;
}sBlkClst;

uint getNewMapBlk_distFtl(uint iCh, uint iWy, uint iPl);
uint getMapBlockVictim_distFtl(uint iCh, uint iWy, uint iPl);
uint getUserBlockVictim_distFtl(uint iCh, uint iWy, uint iPl);

uint getFreeBlk_distFtl(uint iCh, uint iWy, uint iPl);
uint getWriteBlk_distFtl(uint iCh, uint iWy, uint iPl, uint adr);
uint getWritePg_distFtl(uint iCh, uint iWy, uint iPl, uint iBlkAdr, uint adr);

void initClst(uint iCh, uint iWy, uint iPl);
void updateBlkClst(uint iCh, uint iWy, uint iPl);
//void updateClstBlkUsed(uint iCh, uint iWy, uint iPl, uint iAllocBlk);
//void updateClstBlkFreed(uint iCh, uint iWy, uint iPl, uint iFreeBlk);


uint getFreeBlk_wrPtrDummy(uint iCh, uint iWy, uint iPl);
uint getWriteBlk_wrPtr(uint iCh, uint iWy, uint iPl, uint adr);
uint getFreeBlk_wrPtr(uint iCh, uint iWy, uint iPl, uint iStart);
uint getUserGcVictim_wrPtr(uint iCh, uint iWy, uint iPl);


#endif
