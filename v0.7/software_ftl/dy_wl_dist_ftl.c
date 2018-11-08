#include "dy_wl_dist_ftl.h"
#include "dftl_meta.h"
#include "dist_ftl_meta.h"
/*
extern sDistFtl distFtlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];

void initClst(uint iCh, uint iWy, uint iPl){

	while (distFtlMeta[iCh][iWy][iPl].freeBlkPool->iCurSize != 0){
		if((sBlkClst *)(listGetTail(distFtlMeta[iCh][iWy][iPl].freeBlkPool)->content) != NULL){
        free((sBlkClst *)(listGetTail(distFtlMeta[iCh][iWy][iPl].freeBlkPool)->content));
        memorySize -= 1;
        }
		listDeleteTail(distFtlMeta[iCh][iWy][iPl].freeBlkPool);
	};

	while (distFtlMeta[iCh][iWy][iPl].usedBlkPool->iCurSize != 0){
        if((sBlkClst *)(listGetTail(distFtlMeta[iCh][iWy][iPl].usedBlkPool)->content) != NULL){
		free((sBlkClst *)(listGetTail(distFtlMeta[iCh][iWy][iPl].usedBlkPool)->content));
        memorySize -= 1;
        }
		listDeleteTail(distFtlMeta[iCh][iWy][iPl].usedBlkPool);
	};	
}

void updateBlkClst(uint iCh, uint iWy, uint iPl){
	uint i;
	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	uchar curFreeBlk = isBlockFree(&(curFtl->blocks[0]));
	uint iStartAddr = 0;
	uint iCurSize = 1;
	sBlkClst *newClst;

	uint iClstCnt = 0;
	uint iClstSize = 0;
	
	initClst(iCh, iWy, iPl);

	for (i = 1; i < nandSpec.iBlockPerPlane; i++){

		if (curFreeBlk == isBlockFree(&(curFtl->blocks[i]))){
			iCurSize++;
			continue;
		}
		else{
            ///xoqhd
            //ACQUIRE_LOCK(iCh);
            //barePrintf("1: ch = %d\n", iCh);
			newClst = (sBlkClst *)malloc(sizeof(sBlkClst));
            //RELEASE_LOCK(iCh);
			newClst->iStartAddr = iStartAddr;
			newClst->iSize = iCurSize;
            
            memorySize += 1;
			if (curFreeBlk == UCHAR_TRUE){				
				listPushTail(distFtlMeta[iCh][iWy][iPl].freeBlkPool, iStartAddr, newClst, iCh);
				iClstCnt++;
				iClstSize += newClst->iSize;
			}
			else{
				listPushTail(distFtlMeta[iCh][iWy][iPl].usedBlkPool, iStartAddr, newClst, iCh);
			}

			iStartAddr = i;
			iCurSize = 1;
			curFreeBlk = isBlockFree(&(curFtl->blocks[i]));
		}
	}

    //ACQUIRE_LOCK(iCh);
    //barePrintf("2: ch = %d\n", iCh);
    newClst = (sBlkClst *)malloc(sizeof(sBlkClst));
    //RELEASE_LOCK(iCh);

	newClst->iStartAddr = iStartAddr;
	newClst->iSize = iCurSize;

	if (curFreeBlk == UCHAR_TRUE){
		listPushTail(distFtlMeta[iCh][iWy][iPl].freeBlkPool, iStartAddr, newClst, iCh);
		iClstCnt++;
		iClstSize += newClst->iSize;
	}
	else{
		listPushTail(distFtlMeta[iCh][iWy][iPl].usedBlkPool, iStartAddr, newClst, iCh);
	}

	g_dClstSize = (double)iClstSize / iClstCnt;
	g_iClstCnt = iClstCnt;
}

uint getNewMapBlk_distFtl(uint iCh, uint iWy, uint iPl){
	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
	uint i;

	for (i = 0; i < nandSpec.iBlockPerPlane; i++){
		if (isBlockFree(&(curFtl->blocks[i]))){
			setMapBlock(&(curFtl->blocks[i]));
			curFtl->iNumUsedBlk++;
			curMeta->iNumMapBlock++;
			listPushTail(curMeta->mapBlockList, i, NULL, iCh);
			break;
		}
	}

#if DEBUG
	assert(i < nandSpec.iBlockPerPlane);
#else
	ftlAssert(i < nandSpec.iBlockPerPlane, "i should be smaller than block/plane");
#endif

	//updateBlkClst(iCh, iWy, iPl);

	return i;
}
uint getMapBlockVictim_distFtl(uint iCh, uint iWy, uint iPl){
	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	uint i, iVictimAddr = INVALID_VALUE;
	uint iMinValid = nandSpec.iPagePerBlock + 1;
	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
	
	for (i = 0; i <nandSpec.iBlockPerPlane; i++){
		if (!isBlockFree(&(curFtl->blocks[i])) &&
			!isBlockDead(&(curFtl->blocks[i])) &&
			getValidPageCnt(&(curFtl->blocks[i])) < iMinValid &&
			isMapBlock(&(curFtl->blocks[i])) &&
			curMeta->iCurMapBlock != i
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
}
uint getUserBlockVictim_distFtl(uint iCh, uint iWy, uint iPl){
	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
	uint i, iVictimAddr = INVALID_VALUE;
	uint iMinValid = nandSpec.iPagePerBlock + 1;
	uint iMaxWritePtr = 0;

	for (i = 0; i <nandSpec.iBlockPerPlane; i++){
		if (!isBlockFree(&(curFtl->blocks[i])) &&
			!isBlockDead(&(curFtl->blocks[i])) &&
			!isMapBlock(&(curFtl->blocks[i]))
			)
		{
			if (getValidPageCnt(&(curFtl->blocks[i])) < iMinValid &&
				isBlockFull(&(curFtl->blocks[i])))
			{
				iVictimAddr = i;
				iMinValid = getValidPageCnt(&(curFtl->blocks[i]));
			}
		}
	}

#if DEBUG
	assert(iVictimAddr != INVALID_VALUE);
#else
	ftlAssert(iVictimAddr != INVALID_VALUE, "at least one victim should be found");
#endif
	return iVictimAddr;
}

uint getFreeBlk_distFtl(uint iCh, uint iWy, uint iPl){

	uint iRetBlk = getFreeBlock(iCh, iWy, iPl);

	//updateBlkClst(iCh, iWy, iPl);

	return iRetBlk;

}

uint getWriteBlk_distFtl(uint iCh, uint iWy, uint iPl, uint adr){
		
	if (adr == LONG_INVALID_VALUE){
		if (isBlockFull(&(ftlMeta[iCh][iWy][iPl].blocks[ftlMeta[iCh][iWy][iPl].iCurBlk]))){
			ftlMeta[iCh][iWy][iPl].iCurBlk = getFreeBlk(iCh, iWy, iPl);
			ftlMeta[iCh][iWy][iPl].iCurPg = getWritablePage(&(ftlMeta[iCh][iWy][iPl].blocks[ftlMeta[iCh][iWy][iPl].iCurBlk]));
		}
		return ftlMeta[iCh][iWy][iPl].iCurBlk;
	}
	else{

		uint pgAdr = getTableIdx(adr);
		sDftlAdr idx = adrIntoDftlAdr(pgAdr);
		sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
		sDistFtl *curMeta = &(distFtlMeta[getCh(adr)][getWy(adr)][getPl(adr)]);
		sDistGtd *curGtd = &(curMeta->gtd[idx.iGtdIdx]);
		uchar inTransCache = UCHAR_FALSE;
		uint k;

		// treverse from iLowPpa;

		for (k = 0; k < curMeta->transCache->iCurSize; k++){
			uint transPgAdr = listGetNthEle(curMeta->transCache, k)->iKey;
			uint transCmtKey = adrIntoDftlAdr(transPgAdr).iCmtKey;
			if (transCmtKey == idx.iCmtKey){
				inTransCache = UCHAR_TRUE;
				break;
			}
		}

		if (curGtd->gtd.status == GTD_ELEMENT_INVALID &&
			!inTransCache &&
			curGtd->pTg == NULL){
			uint iRetBlk = getFreeBlk(iCh, iWy, iPl);
			return iRetBlk;
		}
		else{
			uint iHighPpa;
			uint iLowPpa;
			sNandAddr iHighNand, iLowNand;
			uint iHighBlk, iLowBlk;
			uint i;
			uint iRetBlk;

			if (curGtd->gtd.status == GTD_ELEMENT_INVALID &&
				curGtd->pTg == NULL){
				iHighPpa = LONG_INVALID_VALUE;
				iLowPpa = LONG_INVALID_VALUE;
			}else{				
				iHighPpa = curGtd->iHighPpa;
				iLowPpa = curGtd->iLowPpa;
			}			

			for (i = 0; i < curMeta->transCache->iCurSize; i++){

				uint transPgAdr = listGetNthEle(curMeta->transCache, k)->iKey;
				uint transCmtKey = adrIntoDftlAdr(transPgAdr).iCmtKey;

				if (transCmtKey == idx.iCmtKey){
					uint iPpa = ((sTransCache *)listGetNthContent(curMeta->transCache, i))->iPpa;
					if (iPpa < iLowPpa || iLowPpa == LONG_INVALID_VALUE){
						iLowPpa = iPpa;
					}

					if (iPpa > iHighPpa || iHighPpa == LONG_INVALID_VALUE){
						iHighPpa = iPpa;
					}
				}
			}

			iHighNand = intToNandAddr(iHighPpa);
			iLowNand = intToNandAddr(iLowPpa);
			iHighBlk = iHighNand.iBlk;
			iLowBlk = iLowNand.iBlk;

			ftlAssert(iHighBlk <= nandSpec.iBlockPerPlane - 1, "weird block number found\n");
			ftlAssert(iLowBlk <= nandSpec.iBlockPerPlane - 1, "weird block number found\n");

			iRetBlk = INVALID_VALUE;

			if ( !isBlockFull(&(curFtl->blocks[iHighBlk])) &&
				!isMapBlock(&(curFtl->blocks[iHighBlk])) &&
				!isBlockFree(&(curFtl->blocks[iHighBlk]))
				)
			{
				return iHighBlk;
			}
			else{
				//if (curFtl->iNumUsedBlk)
				if (((DATA_BLK_CNT - getFtlPtr(iCh, iWy, iPl)->iNumUsedUserBlk) <= 2))
				{
					uint iBlkIdx, iter;

					for (iter = 0; iter < nandSpec.iBlockPerPlane; iter++){
						iBlkIdx = (iter + iHighBlk) % nandSpec.iBlockPerPlane;
						if (
							!isBlockFull(&(curFtl->blocks[iBlkIdx])) &&
							!isMapBlock(&(curFtl->blocks[iBlkIdx])) &&
							!isBlockFree(&(curFtl->blocks[iBlkIdx]))
							){
							return iBlkIdx;
						}
					}
				}
				else{
					return getFreeBlk(iCh, iWy, iPl);
				}				
			}						
		}		
	}

	ftlAssert(0, "block search failed\n"); 
	return 0;	
}

uint getWritePg_distFtl(uint iCh, uint iWy, uint iPl, uint iBlkAdr, uint adr){
	return getWritablePage(&(ftlMeta[iCh][iWy][iPl].blocks[iBlkAdr]));	
}

uint getWriteBlk_wrPtr(uint iCh, uint iWy, uint iPl, uint adr){
	uint pgAdr = getTableIdx(adr);
	//uint wrPtrIdx = pgAdr % _RD_WR_PTR_CNT_;
	uint wrPtrIdx = adrIntoDftlAdr(pgAdr).iCmtKey % _RD_WR_PTR_CNT_;
	uint iRegionSize = nandSpec.iBlockPerPlane / _RD_WR_PTR_CNT_;
	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
	uint iWriteBlk;
	sBlockMeta *blkMeta;



	if (((DATA_BLK_CNT - getFtlPtr(iCh, iWy, iPl)->iNumUsedUserBlk) > 1)){
		if (g_isSequential){

			if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
				printf("come to sequential");
			}

			if (curMeta->iSqWrBlk[wrPtrIdx] == INVALID_VALUE) // we need initial value;
			{
				curMeta->iSqWrBlk[wrPtrIdx] = getFreeBlk_wrPtr(iCh, iWy, iPl, wrPtrIdx * iRegionSize);
			}

			blkMeta = &(curFtl->blocks[curMeta->iSqWrBlk[wrPtrIdx]]);

			if (isBlockFree(blkMeta) || isMapBlock(blkMeta)){
				// the block is erased due to garbage collection
				curMeta->iSqWrBlk[wrPtrIdx] = getFreeBlk_wrPtr(iCh, iWy, iPl, curMeta->iRdWrBlk[wrPtrIdx]);
				blkMeta = &(curFtl->blocks[curMeta->iSqWrBlk[wrPtrIdx]]);

				if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
					printf("new block due to previous gc");
				}
			}
						
			ftlAssert(!isBlockFree(blkMeta) && !isMapBlock(blkMeta), "wrong blk is used as user block");

			if (isBlockFull(blkMeta) ||
				isMapBlock(blkMeta)				
				)
			{
				curMeta->iSqWrBlk[wrPtrIdx] = getFreeBlk_wrPtr(iCh, iWy, iPl, curMeta->iSqWrBlk[wrPtrIdx]);
				blkMeta = &(curFtl->blocks[curMeta->iSqWrBlk[wrPtrIdx]]);

				if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
					printf("new block due to full blk");
				}
			}

			setWrType(blkMeta, UCHAR_TRUE);
			iWriteBlk = curMeta->iSqWrBlk[wrPtrIdx];

			if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
				printf("write blk is %d", iWriteBlk);
			}
		}
		else{
			if (curMeta->iRdWrBlk[wrPtrIdx] == INVALID_VALUE) // we need initial value
			{
				curMeta->iRdWrBlk[wrPtrIdx] = getFreeBlk_wrPtr(iCh, iWy, iPl, wrPtrIdx * iRegionSize);
			}

			blkMeta = &(curFtl->blocks[curMeta->iRdWrBlk[wrPtrIdx]]);

			if (isBlockFree(blkMeta) || isMapBlock(blkMeta)){
				// the block is erased due to gc
				curMeta->iRdWrBlk[wrPtrIdx] = getFreeBlk_wrPtr(iCh, iWy, iPl, curMeta->iSqWrBlk[wrPtrIdx]);
				blkMeta = &(curFtl->blocks[curMeta->iSqWrBlk[wrPtrIdx]]);

				if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
					printf("new block due to previous gc");
				}
			}

			ftlAssert(!isBlockFree(blkMeta) && !isMapBlock(blkMeta), "wrong blk is used as user block");

			if (isBlockFull(blkMeta) ||
				isMapBlock(blkMeta)			
				)
			{
				curMeta->iRdWrBlk[wrPtrIdx] = getFreeBlk_wrPtr(iCh, iWy, iPl, curMeta->iRdWrBlk[wrPtrIdx]);
				blkMeta = &(curFtl->blocks[curMeta->iRdWrBlk[wrPtrIdx]]);

				if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
					printf("new block due to full blk");
				}
			}

			setWrType(blkMeta, UCHAR_FALSE);
			iWriteBlk = curMeta->iRdWrBlk[wrPtrIdx];

			if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
				printf("write blk is %d", iWriteBlk);
			}
		}
	}
	else{
		uint iBlkIdx, iter;

		for (iter = 0; iter < nandSpec.iBlockPerPlane; iter++){
			iBlkIdx = (iter) % nandSpec.iBlockPerPlane;
			if (
				!isBlockFull(&(curFtl->blocks[iBlkIdx])) &&
				!isMapBlock(&(curFtl->blocks[iBlkIdx])) &&
				!isBlockFree(&(curFtl->blocks[iBlkIdx]))
				){

				if (iCh == 0 && iWy == 3 && iPl == 0 && debugFlag){
					printf("new blk is %d", iBlkIdx);
				}
				return iBlkIdx;
			}
		}
	}

	return iWriteBlk;
}



uint getFreeBlk_wrPtrDummy(uint iCh, uint iWy, uint iPl){
	return 0;
}

uint getFreeBlk_wrPtr(uint iCh, uint iWy, uint iPl, uint iStart){
	uint i, iBlkAdr;
	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);

	for (i = 0; i < nandSpec.iBlockPerPlane; i++){
		iBlkAdr = (iStart + i) % nandSpec.iBlockPerPlane;
		if (isBlockFree(&(curFtl->blocks[iBlkAdr]))){
			setBlockUsed(&(curFtl->blocks[iBlkAdr]));
			curFtl->iNumUsedBlk++;
			curFtl->iNumUsedUserBlk++;
			break;
		}
	}

	ftlAssert(i < nandSpec.iBlockPerPlane, "found idx should be smaller than block/plane");

	return iBlkAdr;
}

uint getUserGcVictim_wrPtr(uint iCh, uint iWy, uint iPl){

	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
	uint i, iVictimAddr = INVALID_VALUE;
	uint iMinValid = nandSpec.iPagePerBlock + 1;
	uint iMaxWritePtr = 0;
	uint k;

	for (i = 0; i <nandSpec.iBlockPerPlane; i++){

		uchar isWrPtr = UCHAR_FALSE;

		for (k = 0; k < _RD_WR_PTR_CNT_; k++){
			if (curMeta->iRdWrBlk[k] == i ||
				curMeta->iSqWrBlk[k] == i)
			{
				isWrPtr = UCHAR_TRUE;
				break;
			}
		}


		if (!isWrPtr	&&
			!isBlockFree(&(curFtl->blocks[i])) &&
			!isBlockDead(&(curFtl->blocks[i])) &&
			!isMapBlock(&(curFtl->blocks[i]))
			)
		{
			if (getValidPageCnt(&(curFtl->blocks[i])) < iMinValid &&
				isBlockFull(&(curFtl->blocks[i])))
			{
				iVictimAddr = i;
				iMinValid = getValidPageCnt(&(curFtl->blocks[i]));
			}
		}
	}

#if DEBUG
	assert(iVictimAddr != INVALID_VALUE);
#else
	ftlAssert(iVictimAddr != INVALID_VALUE, "at least one victim should be found");
#endif
	return iVictimAddr;
}
*/
