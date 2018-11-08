#include "dynamic_wl_dftl.h"
/*
uint getNewMapBlock(uint iCh, uint iWy, uint iPl){

	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	sDftl *dftl = &(dftlMeta[iCh][iWy][iPl]);
	uint i;

	for(i=0; i < nandSpec.iBlockPerPlane ; i++){
		if(isBlockFree(&(curFtl->blocks[i]))){
			setMapBlock(&(curFtl->blocks[i]));
			curFtl->iNumUsedBlk++;
			dftl->iNumMapBlock++;
			listPushTail(dftl->mapBlockList, i, NULL, iCh);

			break;
		}
	}

#if DEBUG
	assert( i < nandSpec.iBlockPerPlane );
#else
	ftlAssert(i < nandSpec.iBlockPerPlane, "i should be smaller than block/plane");
#endif

	return i;
}

uint getMapBlockVictim(uint iCh, uint iWy, uint iPl){

	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	uint i, iVictimAddr = INVALID_VALUE;
	uint iMinValid = nandSpec.iPagePerBlock + 1;
	sDftl *dftl = &(dftlMeta[iCh][iWy][iPl]);

	for(i=0 ; i <nandSpec.iBlockPerPlane ; i++){
		if(!isBlockFree(&(curFtl->blocks[i])) &&
			!isBlockDead(&(curFtl->blocks[i])) &&
			getValidPageCnt(&(curFtl->blocks[i])) < iMinValid &&
			isMapBlock(&(curFtl->blocks[i])) &&
			dftl->iCurMapBlock != i			
			)
		{
			iVictimAddr = i;
			iMinValid = getValidPageCnt(&(curFtl->blocks[i]));
		}
	}

#if DEBUG
	assert(iVictimAddr != INVALID_VALUE && "No victim for map garbage collection");
#else
	ftlAssert(iVictimAddr != INVALID_VALUE, "No victim for map garbage collection");
#endif

	return iVictimAddr;
}*/
