#include "fg_gc_dist_ftl.h"
#include "dy_wl_dist_ftl.h"
#include "dftl_meta.h"

//extern sDistFtl distFtlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];
//
//void mapGc_distFtl(uint iCh, uint iWy, uint iPl){
//
//	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
//	uint iTargetMapAdr = curMeta->getMapBlkVictim(iCh, iWy, iPl);
//	uint i, j;
//	sNandAddr srcPgAdr, desPgAdr, desBkAdr;
//	sPageMeta *pSrcPg;
//	sNandCmd cmd;
//	sFtlMeta *ftl = &(ftlMeta[iCh][iWy][iPl]);
//    sElePtr* test;
//
//	
//#if DEBUG
//	assert((curMeta->iNumMapBlock >= _MAP_BLOCK_PER_PL_));
//#else
//	ftlAssert((curMeta->iNumMapBlock >= _MAP_BLOCK_PER_PL_), "# map block should be larger than threshold");
//#endif
//
//	for (i = 0; i < nandSpec.iPagePerBlock; i++)
//	{
//		if (isPageValid(&(ftl->blocks[iTargetMapAdr].page[i]))){
//			pSrcPg = getPgPtr(iCh, iWy, iPl, iTargetMapAdr, i);
//			srcPgAdr = composeNandAddr(iCh, iWy, iPl, iTargetMapAdr, i, 0);
//
//			if (isBlockFull(&(ftl->blocks[curMeta->iCurMapBlock]))){
//				curMeta->iCurMapBlock = curMeta->getMapBlk(iCh, iWy, iPl);
//			}
//
//			desPgAdr = composeNandAddr(iCh, iWy, iPl,
//				curMeta->iCurMapBlock, getWritablePage(&(ftl->blocks[curMeta->iCurMapBlock])), 0);
//
//			cmd = composeNandCmd(NAND_CMD_MAP_COPYBACK,
//				desPgAdr, srcPgAdr, LONG_INVALID_VALUE, NULL);
//
//			registerNandCmd(cmd);
//
//			for (j = 0; j < GTD_SIZE; j++){
//				if (curMeta->gtd[j].gtd.iAddr == nandAddrtoInt(srcPgAdr)){
//#if DEBUG
//					assert(curMeta->gtd[j].gtd.status == GTD_ELEMENT_NAND);
//#else					
//					ftlAssert(curMeta->gtd[j].gtd.status == GTD_ELEMENT_NAND, "found GTD element should be valid");
//#endif
//					break;
//				}
//			}
//
//#if DEBUG
//			assert(j < GTD_SIZE);
//#else
//			ftlAssert(j < GTD_SIZE, "gtd idx should be smaller than entire size");
//#endif
//			curMeta->gtd[j].gtd.iAddr = nandAddrtoInt(desPgAdr);
//		}
//	}
//
//	desBkAdr = composeNandAddr(iCh, iWy, iPl, iTargetMapAdr, 0, 0);
//	cmd = composeNandCmd(NAND_CMD_MAP_ERASE, desBkAdr, dummyNandAdr, 0, NULL);
//	registerNandCmd(cmd);
//
//	//updateBlkClst(iCh, iWy, iPl);
//
//	curMeta->iNumMapBlock--;
//    //free(listFindKey(curMeta->mapBlockList, getBk(nandAddrtoInt(cmd.dest)))->content);
//    ///////////////////////////////
//    test = listFindKey(curMeta->mapBlockList, getBk(nandAddrtoInt(cmd.dest)))->content;
//    if(test != NULL){
//        free(test);
//        memorySize -= 1;
//    }
//    ///////////////////////////////////
//	listDeleteEle(curMeta->mapBlockList, listFindKey(curMeta->mapBlockList, getBk(nandAddrtoInt(cmd.dest))));
//}
//
//uchar isMapGcReq_distFtl(uint iCh, uint iWy, uint iPl){
//	return (distFtlMeta[iCh][iWy][iPl].iNumMapBlock > _MAP_BLOCK_PER_PL_) ? UCHAR_TRUE : UCHAR_FALSE;
//}
//
//void userBlkGc_distFtl(uint iCh, uint iWy, uint iPl){
//	uint iTargetAddr = getGcVictimBlk(iCh, iWy, iPl);//getGcVictim(iCh, iWy, iPl);
//	uint i = 0;// , k;
//	sNandAddr srcPg, desPg, desBk;
//	uint iTarBlk, iTarPg;
//	sPageMeta *pSrcPg;
//	sNandCmd cmd;
//	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
//	uchar isSeqBak = g_isSequential;
//
//	/// do gc work;
//
//	g_isSequential = curFtl->blocks[iTargetAddr].iWrType;
//
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
//	g_isSequential = isSeqBak;
//
//	//updateBlkClst(iCh, iWy, iPl);
//}
//
//void userBlkGc_wrPtr(uint iCh, uint iWy, uint iPl){
//	uint iTargetAddr = getGcVictimBlk(iCh, iWy, iPl);//getGcVictim(iCh, iWy, iPl);
//	uint i = 0;// , k;
//	sNandAddr srcPg, desPg, desBk;
//	uint iTarBlk, iTarPg;
//	sPageMeta *pSrcPg;
//	sNandCmd cmd;
//	sFtlMeta *curFtl = &(ftlMeta[iCh][iWy][iPl]);
//	uchar isSeqBak = g_isSequential;
//
//    uint *sortedLpa;
//    uint *sortedPpa;
//    uint iNumValid = 0;
//	uint k;	
//	uint l;
//	uint iLogicalAddr;
//	
//    //ACQUIRE_LOCK(iCh);
//    //barePrintf("5: ch = %d\n", iCh);
//	sortedLpa = (uint *)malloc(sizeof(uint) * nandSpec.iPagePerBlock);
//	sortedPpa = (uint *)malloc(sizeof(uint)* nandSpec.iPagePerBlock);
//    //RELEASE_LOCK(iCh);
//
//    memorySize += 2;
//	memset(sortedLpa, 0xff, sizeof(uint) * nandSpec.iPagePerBlock);
//	memset(sortedPpa, 0xff, sizeof(uint) * nandSpec.iPagePerBlock);
//
//	for (i = 0; i < nandSpec.iPagePerBlock; i++){
//		
//		pSrcPg = getPgPtr(iCh, iWy, iPl, iTargetAddr, i);
//
//		if (isPageValid(pSrcPg)){
//			iNumValid++;
//			iLogicalAddr = (((uint)(pSrcPg->iLogicalPgAddr)) * SECTOR_PER_PAGE);
//
//			for (k = 0; k < nandSpec.iPagePerBlock; k++){
//
//				ftlAssert(iLogicalAddr != sortedLpa[k], "same logical page address during sorting gc pages");
//
//				if (sortedLpa[k] == LONG_INVALID_VALUE){
//
//					sortedLpa[k] = iLogicalAddr;
//					sortedPpa[k] = i;
//
//					break;
//				}
//				else{
//					if (sortedLpa[k] > iLogicalAddr){
//
//						if (k < (nandSpec.iPagePerBlock - 1))
//						{
//							for (l = nandSpec.iPagePerBlock - 1; l != k; l--){
//								sortedLpa[l] = sortedLpa[l - 1];
//								sortedPpa[l] = sortedPpa[l - 1];
//							}
//						}
//
//						sortedLpa[k] = iLogicalAddr;
//						sortedPpa[k] = i;
//
//						break;
//					}
//				}
//
//				ftlAssert(k < nandSpec.iPagePerBlock, "sorted page should be found in the array");
//			}
//		}
//	}
//
//	/// do gc work;
//
//	g_isSequential = curFtl->blocks[iTargetAddr].iWrType;
//
//	for (i = 0; i < iNumValid; i++){
//
//		pSrcPg = getPgPtr(iCh, iWy, iPl, iTargetAddr, sortedPpa[i]);
//		srcPg = composeNandAddr(iCh, iWy, iPl, iTargetAddr, sortedPpa[i], 0);
//
//		ftlAssert(sortedPpa[i] < nandSpec.iPagePerBlock, "weird page for gc");	
//		ftlAssert(
//			isPageValid(&(ftlMeta[iCh][iWy][iPl].blocks[iTargetAddr].page[sortedPpa[i]])),
//			"wrong page for gc is selected!! no valid page!!"
//			);
//
//
//		iTarBlk = getTarBlk(iCh, iWy, iPl, (uint)pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE);
//		iTarPg = getTarPg(iCh, iWy, iPl, iTarBlk, (uint)pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE);
//
//		
//		desPg = composeNandAddr(iCh, iWy, iPl, iTarBlk, iTarPg, 0);
//		cmd = composeNandCmd(NAND_CMD_COPYBACK, desPg, srcPg, 0, NULL);
//		registerNandCmd(cmd);
//
//		
//		setMap(pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE, desPg);		
//	}	
//
//	desBk = composeNandAddr(iCh, iWy, iPl, iTargetAddr, 0, 0);
//	cmd = composeNandCmd(NAND_CMD_ERASE, desBk, desBk, 0, NULL);
//	registerNandCmd(cmd);
//
//	g_isSequential = isSeqBak;
//
//	free(sortedLpa);
//    memorySize -= 1;
//	free(sortedPpa);	
//    memorySize -= 1;
//
//	//updateBlkClst(iCh, iWy, iPl);
//}
