#include "bg_gc_dist_ftl.h"

//extern sDistFtl distFtlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];
//
//// initialization
//void initBgc_distFtl(){
//
//}
//// inner
//
//uint getBgcVictim_distFtl(uint iCh, uint iWy, uint iPl)
//{
//	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
//	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
//	uint i, iVictimAddr = INVALID_VALUE;
//	uint iMaxValid = 0;
//	uint iMaxWritePtr = 0;
//
//	sBlkClst *tinyFreeClst;
//	sBlkClst *selectedUsedClst;
//	uint iClstIdx;
//	uint iClstSize = nandSpec.iBlockPerPlane + 1;
//
//	/*for (iClstIdx = 0; iClstIdx < curMeta->freeBlkPool->iCurSize; iClstIdx++){
//		sBlkClst *curClst = listGetNthContent(curMeta->freeBlkPool, iClstIdx);
//
//		if (curClst->iSize < iClstSize){
//			tinyFreeClst = curClst;
//			iClstSize = curClst->iSize;
//		}
//	}
//
//	if (tinyFreeClst == curMeta->freeBlkPool->tail->prev){
//		for (i = 0; i < nandSpec.iBlockPerPlane; i++){
//			if (!isBlockFree(&(curFtl->blocks[i])) &&
//				!isBlockDead(&(curFtl->blocks[i])) &&
//				!isMapBlock(&(curFtl->blocks[i]))
//				)
//			{
//				if (getValidPageCnt(&(curFtl->blocks[i])) >= iMaxValid &&
//					isBlockFull(&(curFtl->blocks[i])))
//				{
//					iVictimAddr = i;
//					iMaxValid = getValidPageCnt(&(curFtl->blocks[i]));
//				}
//			}
//		}
//	}
//	else*/{
//		/*for (iClstIdx = 0; iClstIdx < curMeta->usedBlkPool->iCurSize; iClstIdx++)
//		{
//			sBlkClst* curClst = listGetNthContent(curMeta->usedBlkPool, iClstIdx);
//			if (curClst->iStartAddr > tinyFreeClst){
//				selectedUsedClst = curClst;
//				break;
//			}
//		}
//
//		for (i = 0; i < selectedUsedClst->iSize; i++){
//			uint iBlkAdr = selectedUsedClst->iStartAddr + i;
//
//			if (
//				!isBlockFree(&(curFtl->blocks[iBlkAdr])) &&
//				!isBlockDead(&(curFtl->blocks[iBlkAdr])) &&
//				!isMapBlock(&(curFtl->blocks[iBlkAdr]))
//				)
//			{
//				if (
//					getValidPageCnt(&(curFtl->blocks[iBlkAdr])) >= iMaxValid &&
//					isBlockFull(&(curFtl->blocks[iBlkAdr]))
//					)
//				{
//					iVictimAddr = i;
//					iMaxValid = getValidPageCnt(&(curFtl->blocks[iBlkAdr]));
//				}
//			}
//		}
//
//		if (iVictimAddr != INVALID_VALUE){
//			return iVictimAddr;
//		}*/
//
//		for (i = 0; i < nandSpec.iBlockPerPlane; i++){
//			if (!isBlockFree(&(curFtl->blocks[i])) &&
//				!isBlockDead(&(curFtl->blocks[i])) &&
//				!isMapBlock(&(curFtl->blocks[i]))
//				)
//			{
//				if (getValidPageCnt(&(curFtl->blocks[i])) >= iMaxValid &&
//					isBlockFull(&(curFtl->blocks[i])))
//				{
//					iVictimAddr = i;
//					iMaxValid = getValidPageCnt(&(curFtl->blocks[i]));
//				}
//			}
//		}
//	}		
//
//#if DEBUG
//	assert(iVictimAddr != INVALID_VALUE);
//#else
//	ftlAssert(iVictimAddr != INVALID_VALUE, "at least one victim should be found");
//#endif
//	return iVictimAddr;
//}
//
//
//void bgc_distFtl(uint iCh, uint iWy, uint iPl){
//	uint iTargetAddr = getBgcVictim_distFtl(iCh, iWy, iPl);// getGcVictimBlk(iCh, iWy, iPl);//getGcVictim(iCh, iWy, iPl);
//	uint i = 0;// , k;
//	sNandAddr srcPg, desPg, desBk;
//	uint iTarBlk, iTarPg;
//	sPageMeta *pSrcPg;
//	sNandCmd cmd;
//	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
//
//	/// do gc work;
//	for (i = 0; i < nandSpec.iPagePerBlock; i++)
//	{
//		if (isPageValid(&(ftlMeta[iCh][iWy][iPl].blocks[iTargetAddr].page[i])))
//		{
//			pSrcPg = getPgPtr(iCh, iWy, iPl, iTargetAddr, i);
//			srcPg = composeNandAddr(iCh, iWy, iPl, iTargetAddr, i, 0);
//
//			iTarBlk = getTarBlk(iCh, iWy, iPl, (uint)pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE);
//			iTarPg = getTarPg(iCh, iWy, iPl, iTarBlk, (uint)pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE);
//
//			desPg = composeNandAddr(iCh, iWy, iPl, iTarBlk, iTarPg, 0);
//			cmd = composeNandCmd(NAND_CMD_COPYBACK, desPg, srcPg, 0, NULL);
//			registerNandCmd(cmd);
//
//			setMap(pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE, desPg);
//		}
//	}
//
//	desBk = composeNandAddr(iCh, iWy, iPl, iTargetAddr, 0, 0);
//	cmd = composeNandCmd(NAND_CMD_ERASE, desBk, desBk, 0, NULL);
//	registerNandCmd(cmd);
//
//	//updateBlkClst(iCh, iWy, iPl);
//}
//
//void isBgcReqDev_distFtl(uint iCh, uint iWy, uint iPl){
//	/*return ((DATA_BLK_CNT - ftlMeta[iCh][iWy][iPl].iNumUsedUserBlk) <= BGC_BLK_CNT_THRESHOLD);*/
//}
