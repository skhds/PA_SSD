#ifndef __STRUCT_DIST_FTL_H__
#define __STRUCT_DIST_FTL_H__

#include "struct_dftl.h"
#include "dist_ftl_spec.h"

typedef struct distGtdElement{
	sGtdElemnt		gtd;
	uchar			isTg;
	uint			iLowPpa;
	uint			iHighPpa;
	uint			iSize;			// tg size in byte;
	void*			pTg;
}sDistGtd;

typedef struct translationGroup{
	uint iHeadAddr;
	uint iSize;
	char *cpCbm;
	char *cpAdrList;
	eCmtDirty dirty;
}sTransGp;

typedef struct translationCache{
	uint			iLpa;
	uint			iPpa;
}sTransCache;

typedef struct distFtl{
	sDistGtd		gtd[GTD_SIZE];
	sListPtr		cmt;
	uint			iCurCmtSize;
	uint			iNumMapBlock;
	uint			iCurMapBlock;
	sListPtr		mapBlockList;
	sListPtr		transCache;
	sListPtr		freeBlkPool;
	sListPtr		usedBlkPool;

	uint			iRdWrBlk[_RD_WR_PTR_CNT_];
	uint			iSqWrBlk[_RD_WR_PTR_CNT_];

	uint			(*getMapBlk)(uint iCh, uint iWy, uint iPl);
	uint			(*getMapBlkVictim)(uint iCh, uint iWy, uint iP);
	void			(*gcMapBlk)(uint iCh, uint iWy, uint iPl);
	uchar			(*isMapGcReq)(uint iCh, uint iWy, uint iPl);
}sDistFtl;

#endif