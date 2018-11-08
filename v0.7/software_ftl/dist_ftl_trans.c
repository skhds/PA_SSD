#include "dist_ftl_trans.h"
#include "dy_wl_dist_ftl.h"
#include "fg_gc_dist_ftl.h"
#include "bg_gc_dist_ftl.h"
#include "dftl_meta.h"

sDistFtl distFtlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];
//
//void initDistFtl(void){
//
//	int i, j, k, l;
//
//	ftlAssert(_NUM_ELE_IN_LINE_ == _NUM_ELE_IN_PAGE_, "ELE_IN_LINE and ELE_IN_PAGE should be same\n");
//
//	for (i = 0; i < NAND_ARRAY_CHANNEL; i++){
//		for (j = 0; j < NAND_ARRAY_WAY; j++){
//			for (k = 0; k < NAND_PLANE_PER_WAY; k++){
//
//
//				memset(distFtlMeta[i][j][k].iSqWrBlk, 0xff, sizeof(uint)* _RD_WR_PTR_CNT_);
//				memset(distFtlMeta[i][j][k].iRdWrBlk, 0xff, sizeof(uint)* _RD_WR_PTR_CNT_);
//
//				distFtlMeta[i][j][k].freeBlkPool = listConstruct(0);
//				distFtlMeta[i][j][k].usedBlkPool = listConstruct(0);
//				initClst(i, j, k);
//				updateBlkClst(i, j, k);				
//				
//				distFtlMeta[i][j][k].getMapBlk = getNewMapBlk_distFtl;
//				distFtlMeta[i][j][k].getMapBlkVictim = getMapBlockVictim_distFtl;
//				distFtlMeta[i][j][k].gcMapBlk = mapGc_distFtl;
//				distFtlMeta[i][j][k].isMapGcReq = isMapGcReq_distFtl;
//
//				distFtlMeta[i][j][k].cmt = listConstruct(0);
//				distFtlMeta[i][j][k].iCurCmtSize = 0;
//				distFtlMeta[i][j][k].transCache = listConstruct(0);
//				distFtlMeta[i][j][k].iNumMapBlock = 0;
//				distFtlMeta[i][j][k].mapBlockList = listConstruct(0);
//				distFtlMeta[i][j][k].iCurMapBlock = distFtlMeta[i][j][k].getMapBlk(i, j, k);
//								
//				for (l = 0; l < GTD_SIZE; l++){
//					distFtlMeta[i][j][k].gtd[l].iHighPpa = LONG_INVALID_VALUE;
//					distFtlMeta[i][j][k].gtd[l].iLowPpa = LONG_INVALID_VALUE;
//					distFtlMeta[i][j][k].gtd[l].iSize = 0;// _SINGLE_ELE_BYTE_ * _NUM_ELE_IN_LINE_;
//					distFtlMeta[i][j][k].gtd[l].isTg = UCHAR_FALSE;
//					distFtlMeta[i][j][k].gtd[l].pTg = NULL;
//					distFtlMeta[i][j][k].gtd[l].gtd.status = GTD_ELEMENT_INVALID;
//					distFtlMeta[i][j][k].gtd[l].gtd.iAddr = LONG_INVALID_VALUE;	
//
//				}
//			}
//		}
//	}
//
//	barePrintf(" ## Dist_FTL spec \n");
//	barePrintf(" ## GTD size: %d elements\n", GTD_SIZE);
//	barePrintf(" ## Min/Max map block #: %d/%d\n", _MIN_MAP_BLOCK_, _MAX_MAP_BLOCK_);
//	barePrintf(" ## Map block per Plane: %d\n", _MAP_BLOCK_PER_PL_);
//	
//	barePrintf(" ## Single element size: %d-byte\n", _SINGLE_ELE_BYTE_);
//	barePrintf(" ## # element in cmt line: %d\n", _NUM_ELE_IN_LINE_);
//	barePrintf(" ## cmt line size: %d-byte\n", _CMT_LINE_SIZE_);
//	barePrintf(" ## cmt cache line number and size: %d, %d-byte\n", _NUM_CMT_IN_CACHE_, _CMT_SIZE_);
//
//	barePrintf(" ## NAND page size / # element in page: %d-byte/%d\n", (_MAP_NAND_PAGE_SIZE_), _NUM_ELE_IN_PAGE_);
//	barePrintf(" ## Total ND page cnt / GTD element cnt: %d / %d\n", _NAND_PAGE_CNT_, _NUM_GTD_ELEMENT_);
//
//	barePrintf(" ## CMT line per page: %d\n", _NUM_CMT_IN_PAGE_);
//	barePrintf(" ## Available Pg Cnt / Sector cnt: %d / %d\n", AVAIL_PG_CNT, AVAIL_PG_CNT * SECTOR_PER_PAGE);
//}
//
//void distFtlSetL2p(uint adr, sNandAddr phyAdr){
//	sDistFtl *curMeta;	
//	sTransCache *newTrans;
//	uint pgAdr;
//	//sDftlAdr adrIdx = adrIntoDftlAdr(pgAdr);
//	sTransCache *bufferedTrans;
//	sElePtr bufferedTransEle;
//
//    uint test = 1;
//
//    curMeta = &(distFtlMeta[getCh(adr)][getWy(adr)][getPl(adr)]);	
//    
//    ACQUIRE_LOCK(getCh(adr));
//    //barePrintf("8: ch = %d\n", getCh(adr));
//	newTrans = (sTransCache *)malloc(sizeof(sTransCache));
//    RELEASE_LOCK(getCh(adr));
//
//	pgAdr = getTableIdx(adr);
//
//	newTrans->iLpa = pgAdr;
//	newTrans->iPpa = nandAddrtoInt(phyAdr);
//
//	// map table access;
//	incMapAccCnt();
//
//	if ((bufferedTransEle = listFindKey(curMeta->transCache, pgAdr)) != NULL){
//		bufferedTrans = bufferedTransEle->content;
//		bufferedTrans->iPpa = newTrans->iPpa;
//        if(newTrans != NULL){
//            ACQUIRE_LOCK(getCh(adr));
//            free(newTrans);
//            RELEASE_LOCK(getCh(adr));
//        }
//
//		return;
//	}		
//
//	listPushTail(curMeta->transCache, pgAdr, newTrans, getCh(adr));
//	
//	if (curMeta->transCache->iCurSize >= _TRC_SIZE_){
//		// list is full, evict to the lower level cache.		
//
//		///////////////// 
//		// 1) get the element from head
//		sTransCache *victim = (sTransCache *)(listGetHead(curMeta->transCache)->content);
//		sDftlAdr idx = adrIntoDftlAdr(victim->iLpa);
//		uint iVictimLpa = victim->iLpa;
//		uchar isAccessed = UCHAR_FALSE;
//		sTransGp* prevTg = NULL;
//		//sTransGp* newTg;
//		uint i;
//		sCmtLine *tmpPlainTg;
//		uint iMinPpa = LONG_INVALID_VALUE, iMaxPpa = 0x0;
//		uint iVictimIdx = 0;
//
//		sTransCache victimAddrs[_TRC_SIZE_];
//		memset(victimAddrs, 0xff, sizeof(sTransCache)* _TRC_SIZE_);		
//
//		///////////////// 
//		// 2) find associative elements -> in the same tg -> tg index? == cmt index?
//		for (i = 0; i < curMeta->transCache->iCurSize; i++){
//			sElePtr curEle = listGetNthEle(curMeta->transCache, i);
//			if (getGtdIndex(((sTransCache *)(curEle->content))->iLpa) == idx.iGtdIdx){
//				memcpy(victimAddrs + iVictimIdx++, curEle->content, sizeof(sTransCache));
//					
//                if((sTransCache *)(curEle->content)!=NULL){
//                    ACQUIRE_LOCK(getCh(adr));
//                    free((sTransCache *)(curEle->content));
//                    RELEASE_LOCK(getCh(adr));
//                }
//                ACQUIRE_LOCK(getCh(adr));
//				listDeleteEle(curMeta->transCache, curEle);
//                RELEASE_LOCK(getCh(adr));
//				i--;
//			}
//		}
//					
//
//		///////////////// 
//		// 3) read previous version of tg
//
//		switch (curMeta->gtd[idx.iGtdIdx].gtd.status){
//		case GTD_ELEMENT_INVALID:
//			if (curMeta->gtd[idx.iGtdIdx].pTg == NULL){
//				isAccessed = UCHAR_FALSE;
//				break;
//			}
//
//		case GTD_ELEMENT_NAND:
//
//			if (curMeta->gtd[idx.iGtdIdx].pTg == NULL){
//				if (!fetchTg(getCh(adr), getWy(adr), getPl(adr), &(curMeta->gtd[idx.iGtdIdx]), idx)){
//					ftlAssert(UCHAR_FALSE, "fail to fetch translation group!");
//				}
//			}	
//		
//			isAccessed = UCHAR_TRUE;
//			//prevTg = (sTransGp *)(curMeta->gtd[idx.iGtdIdx].gtd.iAddr);
//			prevTg = (sTransGp *)(curMeta->gtd[idx.iGtdIdx].pTg);
//			break;
//
//		case GTD_ELEMENT_CACHED:
//			ftlAssert(UCHAR_FALSE, "GTD status is bad\n");
//			break;
//		default:
//			ftlAssert(UCHAR_FALSE, "GTD status is bad\n");
//		}		
//
//		///////////////// 
//		// 4) compose full tg
//
//        ACQUIRE_LOCK(getCh(adr));
//        //barePrintf("9: ch = %d\n", getCh(adr));
//		tmpPlainTg = (sCmtLine *)malloc(sizeof(sCmtLine));
//        RELEASE_LOCK(getCh(adr));
//
//		if (isAccessed){
//			if (curMeta->gtd[idx.iGtdIdx].isTg){
//				convertToPlain(curMeta->gtd[idx.iGtdIdx], prevTg, tmpPlainTg);
//			}
//			else{
//				memcpy(tmpPlainTg, prevTg, sizeof(sCmtLine));
//				//tmpPlainTg = (sCmtLine *)prevTg;
//			}
//		}
//		else{
//			memset(tmpPlainTg->iAddrs, 0xff, _SINGLE_ELE_BYTE_ * _NUM_ELE_IN_LINE_);
//			tmpPlainTg->iHeadAddr = idx.iCmtKey;
//		}		
//		
//		for (i = 0; i < _TRC_SIZE_; i++)
//		{
//			if (victimAddrs[i].iLpa != LONG_INVALID_VALUE){
//				//if (victimAddrs[i].iPpa < iMinPpa) { iMinPpa = victimAddrs[i].iPpa; }
//				//if (victimAddrs[i].iPpa > iMaxPpa) { iMaxPpa = victimAddrs[i].iPpa; }
//				tmpPlainTg->iAddrs[getOffsetInLine(victimAddrs[i].iLpa)] = victimAddrs[i].iPpa;
//			}
//		}
//				
//
//		//free(victimAddrs);
//
//		tmpPlainTg->dirty = CMT_DIRTY;
//        //RELEASE_LOCK(getCh(adr));
//        //test = 0;
//		cacheTg(getCh(adr), getWy(adr), getPl(adr), tmpPlainTg, idx);		
//	}	
//}
//
//sNandAddr distFtlL2pTrans(uint adr){
//
//	sDistFtl *curMeta = &(distFtlMeta[getCh(adr)][getWy(adr)][getPl(adr)]);
//	uint pgAdr = getTableIdx(adr);
//	sDftlAdr idx = adrIntoDftlAdr(pgAdr);	
//	//sElePtr cmtElement;
//	sTransGp *foundTg;
//	sNandAddr retNandAddr = dummyNandAdr;
//	sDistGtd *gtdEle = &(curMeta->gtd[idx.iGtdIdx]);
//	sTransCache *bufferedTrans;
//	sElePtr bufferedTransEle;
//	uint foundAdr;
//	
//	// increase map access cnt;
//	incMapAccCnt();
//
//	// check translation buffer	
//	
//	if ((bufferedTransEle = listFindKey(curMeta->transCache, pgAdr)) != NULL){
//		bufferedTrans = bufferedTransEle->content;
//		return intToNandAddr(bufferedTrans->iPpa);
//	}	
//
//	// if miss, do the cmt job
//	if(gtdEle->gtd.status == GTD_ELEMENT_INVALID && gtdEle->pTg == NULL){
//		retNandAddr.isValid = UCHAR_FALSE;		// the fresh data
//	}else{		
//		
//		if (gtdEle->pTg == NULL){
//			if(!fetchTg(getCh(adr), getWy(adr), getPl(adr), gtdEle, idx)){
//				ftlAssert(UCHAR_FALSE, "fail to fetch translation group!");
//			}
//		}
//		
//		foundTg = (sTransGp *)(gtdEle->pTg);
//
//		if (gtdEle->isTg){
//			foundAdr = readAdrTg(gtdEle->iLowPpa, gtdEle->iHighPpa,
//				foundTg,
//				getAdrIdx(foundTg, idx.iOffsetInLine),
//				idx.iOffsetInLine
//				);
//		}
//		else{
//			sCmtLine *plainTg = (sCmtLine *)foundTg;
//			foundAdr = plainTg->iAddrs[idx.iOffsetInLine];
//			foundAdr = (foundAdr == INVALID_VALUE) ? LONG_INVALID_VALUE : foundAdr;			
//		}	
//
//		if (foundAdr == LONG_INVALID_VALUE){
//			retNandAddr.isValid = UCHAR_FALSE;
//		}
//		else{
//			retNandAddr = intToNandAddr(foundAdr);
//		}		
//
//		listMoveToTail(curMeta->cmt, listFindKey(curMeta->cmt, (uint)idx.iCmtKey));
//
//	}
//
//	return retNandAddr;
//}
//
//uchar fetchTg(uint iCh, uint iWy, uint iPl, sDistGtd *curGtd, sDftlAdr idx){
//
//	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
//	sNandAddr phyAdr = intToNandAddr(curGtd->gtd.iAddr);
//	sNandCmd nandCmd;
//	sMapPage *fetchedData;
//	sCmtLine *newLine;
//	sFtlMeta *curFtl = (ftlMeta[iCh][iWy][iPl]);
//	//sNandAddr retAdr;
//
//	incMapFetchCnt();
//
//    ACQUIRE_LOCK(iCh);
//    //barePrintf("10: ch = %d\n", iCh);
//	fetchedData = (sMapPage *)malloc(sizeof(sMapPage));
//	newLine = (sCmtLine *)malloc(sizeof(sCmtLine));
//    RELEASE_LOCK(iCh);
//
//	nandCmd = composeNandCmd(NAND_CMD_MAP_READ, phyAdr, dummyNandAdr, LONG_INVALID_VALUE, NULL);
//	setMapBuffer(&nandCmd, (sPgBuf *)(fetchedData));
//
//	ftlAssert(isMapBlock(&(curFtl->blocks[phyAdr.iBlk])), "## this should be a map block\n");
//
//
//	registerNandCmd(nandCmd);
//
//	// add to cmt
//	// cacheTg(iCh, iWy, iPl, &fetchedData);
//	newLine->iHeadAddr = idx.iCmtKey;
//	memcpy(newLine->iAddrs, fetchedData, sizeof(uint)* _NUM_ELE_IN_LINE_);
//
//	newLine->dirty = CMT_CLEAN;
//	cacheTg(iCh, iWy, iPl, newLine, idx);
//    ACQUIRE_LOCK(iCh);
//	free(fetchedData);
//    RELEASE_LOCK(iCh);
//	//cacheTg(iCh, iWy, iPl, fetchedData, idx);
//
//	return UCHAR_TRUE;
//}
//
//void cacheTg(uint iCh, uint iWy, uint iPl, sCmtLine *data, sDftlAdr idx){
//	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
//	sElePtr prevTg = listFindKey(curMeta->cmt, idx.iCmtKey);
//	uchar isTg = UCHAR_FALSE;
//	uint flushedTgSize = 0;
//	uint addedTgSize = _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_;
//	sTransGp *newTg;
//	eCmtDirty isDirty = data->dirty;
//
//	void *curCachedData;
//	sDistGtd newGtd;
//
//
//	if (prevTg != NULL){
//		// delete previously cached tg
//		flushedTgSize = curMeta->gtd[idx.iGtdIdx].iSize;
//		
//		if (curMeta->gtd[idx.iGtdIdx].isTg){
//            if(((sTransGp *)(prevTg->content))->cpAdrList != NULL){
//                ACQUIRE_LOCK(iCh);
//                free(((sTransGp *)(prevTg->content))->cpAdrList);
//                RELEASE_LOCK(iCh);
//            }
//            if(((sTransGp *)(prevTg->content))->cpCbm != NULL){
//                ACQUIRE_LOCK(iCh);
//                free(((sTransGp *)(prevTg->content))->cpCbm);
//                RELEASE_LOCK(iCh);
//            }
//            if((sTransGp *)(prevTg->content) != NULL){
//                ACQUIRE_LOCK(iCh);
//                free((sTransGp *)(prevTg->content));
//                RELEASE_LOCK(iCh);
//            }
//        }
//        else{
//            if((sCmtLine *)(prevTg->content) !=NULL){
//                ACQUIRE_LOCK(iCh);
//                free((sCmtLine *)(prevTg->content));
//                RELEASE_LOCK(iCh);
//            }
//        }
//        ACQUIRE_LOCK(iCh);
//        listDeleteEle(curMeta->cmt, prevTg);
//        RELEASE_LOCK(iCh);
//	}
//
//	curMeta->iCurCmtSize -= flushedTgSize;
//
//	// prepare cached data
//	if (isSmaller(data)){		// convert to tg
//		
//        ACQUIRE_LOCK(iCh);
//        //barePrintf("11: ch = %d\n", iCh);
//		newTg = (sTransGp *)malloc(sizeof(sTransGp));
//        RELEASE_LOCK(iCh);
//
//        memorySize +=1;
//		newTg->iHeadAddr = idx.iCmtKey;
//		newTg->cpCbm = NULL;
//		newTg->cpAdrList = NULL;
//		newTg->dirty = isDirty;
//		newGtd = convertToTg(data, newTg, iCh);
//
//		isTg = UCHAR_TRUE;
//		addedTgSize = newTg->iSize = newGtd.iSize;
//		curCachedData = (void *)newTg;
//        if( data != NULL){
//            ACQUIRE_LOCK(iCh);
//            free(data);
//            RELEASE_LOCK(iCh);
//        }
//	}
//	else{						// do not convert
//
//		uint iOffset;
//
//		isTg = UCHAR_FALSE;
//		addedTgSize = _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_;
//		curCachedData = (void *)data;
//
//		newGtd.iHighPpa = 0;
//		newGtd.iLowPpa = nandSpec.iBlockPerPlane + 1;
//
//		for (iOffset = 0; iOffset < _NUM_ELE_IN_LINE_; iOffset++){
//			if (data->iAddrs[iOffset] != INVALID_VALUE){
//				if (data->iAddrs[iOffset] <= newGtd.iLowPpa || newGtd.iLowPpa == nandSpec.iBlockPerPlane + 1){
//					newGtd.iLowPpa = data->iAddrs[iOffset];
//				}
//				if (data->iAddrs[iOffset] >= newGtd.iHighPpa){
//					newGtd.iHighPpa = data->iAddrs[iOffset];
//				}
//			}			
//		}
//	}
//
//	while ((curMeta->iCurCmtSize + addedTgSize) >
//		(_NUM_CMT_IN_CACHE_ * _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_)){
//		evictTg(iCh, iWy, iPl, idx);
//	}
//
//	listPushTail(curMeta->cmt, idx.iCmtKey, curCachedData, iCh);
//	curMeta->iCurCmtSize += addedTgSize;
//
//	if (isTg){
//
//		curMeta->gtd[idx.iGtdIdx].iHighPpa = newGtd.iHighPpa;
//		curMeta->gtd[idx.iGtdIdx].iLowPpa = newGtd.iLowPpa;
//		curMeta->gtd[idx.iGtdIdx].isTg = isTg;
//		curMeta->gtd[idx.iGtdIdx].iSize = addedTgSize;
//		curMeta->gtd[idx.iGtdIdx].pTg = curCachedData;	
//
//	}
//	else{
//		curMeta->gtd[idx.iGtdIdx].iHighPpa = newGtd.iHighPpa;//LONG_INVALID_VALUE;
//		curMeta->gtd[idx.iGtdIdx].iLowPpa = newGtd.iLowPpa;// LONG_INVALID_VALUE;
//		curMeta->gtd[idx.iGtdIdx].isTg = isTg;
//		curMeta->gtd[idx.iGtdIdx].iSize = _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_;
//		curMeta->gtd[idx.iGtdIdx].pTg = curCachedData;
//	}
//}
//
//void evictTg(uint iCh, uint iWy, uint iPl, sDftlAdr idx){
//	sDistFtl *curMeta = &(distFtlMeta[iCh][iWy][iPl]);
//	sFtlMeta *curFtl = (ftlMeta[iCh][iWy][iPl]);
//	sDistGtd *curGtd;// = &(curMeta->gtd[idx.iGtdIdx]);
//	uchar isTg;// = curGtd->isTg;
//	sCmtLine *victimLine;
//	sTransGp *victimTg;
//	sNandAddr newAdr;
//	sNandCmd nandCmd;
//	sNandAddr prevMapAdr;// = intToNandAddr(curGtd->gtd.iAddr);
//	//sMapPage prevMapData;
//	uint iGtdIdx;
//	eCmtDirty isDirty;
//	
//	victimTg = (sTransGp *)(listGetHead(curMeta->cmt)->content);
//
//	for (iGtdIdx = 0; iGtdIdx < GTD_SIZE; iGtdIdx++)
//	{
//		if (curMeta->gtd[iGtdIdx].pTg == victimTg){
//			break;
//		}
//	}
//
//	ftlAssert(iGtdIdx < GTD_SIZE, "no proper gtd element found");
//
//	curGtd = &(curMeta->gtd[iGtdIdx]);
//	isTg = curGtd->isTg;
//	prevMapAdr = intToNandAddr(curGtd->gtd.iAddr);
//
//	if (isTg){
//		isDirty = victimTg->dirty;
//	}
//	else{
//		isDirty = ((sCmtLine *)victimTg)->dirty;
//	}
//
//
//	if (curGtd->gtd.status != GTD_ELEMENT_INVALID){
//
//		if (isDirty == CMT_CLEAN)
//		{
//			if (
//				isMapBlock(&(curFtl->blocks[prevMapAdr.iBlk])) &&
//				isPageValid(&(curFtl->blocks[prevMapAdr.iBlk].page[prevMapAdr.iPage])) &&
//				!isBlockFree(&(curFtl->blocks[prevMapAdr.iBlk]))
//				)
//			{
//				curMeta->iCurCmtSize -= curMeta->gtd[iGtdIdx].iSize;
//				curMeta->gtd[iGtdIdx].iSize = _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_;
//				curMeta->gtd[iGtdIdx].gtd.status = GTD_ELEMENT_NAND;				
//				curMeta->gtd[iGtdIdx].isTg = UCHAR_FALSE;
//				curMeta->gtd[iGtdIdx].pTg = NULL;
//
//				if (isTg){
//                    if(victimTg->cpCbm !=NULL){
//                        ACQUIRE_LOCK(iCh);
//                        free(victimTg->cpCbm);
//                        RELEASE_LOCK(iCh);
//
//                    }
//                    if(victimTg->cpAdrList != NULL){
//                        ACQUIRE_LOCK(iCh);
//                        free(victimTg->cpAdrList);
//                        RELEASE_LOCK(iCh);
//                    }
//                    if(victimTg != NULL){
//                        ACQUIRE_LOCK(iCh);
//                        free(victimTg);
//                        RELEASE_LOCK(iCh);
//                    }
//				}
//                ACQUIRE_LOCK(iCh);
//				listDeleteHead(curMeta->cmt);
//                RELEASE_LOCK(iCh);
//				return;
//			}
//		}
//
//		nandMapInvalidate(prevMapAdr);
//
//	}
//
//	if (isTg){
//		//victimTg = (sTransGp *)(listGetHead(curMeta->cmt)->content);
//        ACQUIRE_LOCK(iCh);
//        //barePrintf("7: ch = %d\n", iCh);
//		victimLine = (sCmtLine *)malloc(sizeof(sCmtLine));
//        RELEASE_LOCK(iCh);
//
//		convertToPlain(*curGtd, victimTg, victimLine);
//	}
//	else{
//		victimLine = (sCmtLine *)victimTg;
//		//victimLine = (sCmtLine *)(listGetHead(curMeta->cmt)->content);
//	}
//
//	while (curMeta->isMapGcReq(iCh, iWy, iPl)){
//		curMeta->gcMapBlk(iCh, iWy, iPl);
//	}
//
//	if (isBlockFull(&(ftlMeta[iCh][iWy][iPl]->blocks[curMeta->iCurMapBlock]))){
//		curMeta->iCurMapBlock = curMeta->getMapBlk(iCh, iWy, iPl);
//	}  
//
//    newAdr = composeNandAddr(iCh, iWy, iPl, curMeta->iCurMapBlock,
//            getWritablePage(&(ftlMeta[iCh][iWy][iPl]->blocks[curMeta->iCurMapBlock])), 0);
//
//	nandCmd = composeNandCmd(NAND_CMD_MAP_PROGRAM,
//            newAdr, dummyNandAdr, LONG_INVALID_VALUE, NULL);
//
//	setMapBuffer(&nandCmd, (sPgBuf *)(victimLine->iAddrs));
//
//	registerNandCmd(nandCmd);
//
//	curMeta->iCurCmtSize -= curMeta->gtd[iGtdIdx].iSize;
//	curMeta->gtd[iGtdIdx].iSize = _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_;
//	
//	curMeta->gtd[iGtdIdx].gtd.status = GTD_ELEMENT_NAND;
//	curMeta->gtd[iGtdIdx].gtd.iAddr = nandAddrtoInt(newAdr);
//	curMeta->gtd[iGtdIdx].isTg = UCHAR_FALSE;
//	curMeta->gtd[iGtdIdx].pTg = NULL;
//	//curMeta->gtd[iGtdIdx].iLowPpa = curMeta->gtd[iGtdIdx].iHighPpa = LONG_INVALID_VALUE;
//
//	ftlAssert(victimLine != NULL, "victim line cannot be NULL");
//	
//	if (isTg){
//        if(victimTg->cpCbm !=NULL){
//            ACQUIRE_LOCK(iCh);
//            free(victimTg->cpCbm);
//            RELEASE_LOCK(iCh);
//        }
//        if(victimTg->cpAdrList != NULL){
//            ACQUIRE_LOCK(iCh);
//            free(victimTg->cpAdrList);
//            RELEASE_LOCK(iCh);
//        }
//        if(victimTg != NULL){
//            ACQUIRE_LOCK(iCh);
//            free(victimTg);
//            RELEASE_LOCK(iCh);
//        }
//        //free(victimLine);
//	}
//
//    if(victimLine != NULL){
//        ACQUIRE_LOCK(iCh);
//        free(victimLine);	
//        RELEASE_LOCK(iCh);
//    }
//
//    ACQUIRE_LOCK(iCh);
//	listDeleteHead(curMeta->cmt);
//    RELEASE_LOCK(iCh);
//	
//
//}
//
//void mapFlush_distFtl(void){
//	uint i, j, k;
//	sDftlAdr dummyIdx;
//
//	memset(&dummyIdx, 0x00, sizeof(sDftlAdr));
//
//	for (i = 0; i < nandSpec.iChannelCount; i++){
//		for (j = 0; j < nandSpec.iWayCount; j++){
//			for (k = 0; k < nandSpec.iPlanePerDev; k++){
//				sDistFtl *curMeta = &(distFtlMeta[i][j][k]);
//				while (curMeta->iCurCmtSize > 0){
//					evictTg(i, j, k, dummyIdx);
//				}
//			}
//		}
//	}
//}

//uint decodeTg(sDistGtd *curGtd, sTransGp *tg, uint idx){
//
//	//uint iStartIdx = 0;
//	//uint i=0;
//	//uchar cIsCp = isContd(idx, tg);
//	//uint iBw = getEleBitwidth(curGtd->iLowPpa, curGtd->iHighPpa);
//	//uint iStartBitCnt, iEndBitEnt;
//	//uint iFoundAdr = 0;
//	//uchar isValid = UCHAR_FALSE;
//	/*for(i=0 ; i < idx ; i++){
//		if( ! ( (tg->cpCbm[i/8] >> (8-(i%8))) & 0x1 ) )
//			iStartIdx++;
//	}
//
//	iStartBitCnt = iBw * iStartIdx;
//	iEndBitEnt = iBw * (iStartIdx + 1);
//
//	for(i=iStartBitCnt ; i < iEndBitEnt ; i++){
//		iFoundAdr += ((0x1 & (tg->cpAdrList[i/8] >> (8-(i%8)))) << (iBw - 1 - i));
//		if((0x1 & (tg->cpAdrList[i/8] >> (8-(i%8)))) == 0)
//		{
//			isValid = UCHAR_TRUE;
//		}
//	}
//
//	iRetAdr = curGtd->iLowPpa + iFoundAdr;
//
//	if(cIsCp){
//		iRetAdr += getContdLen(iStartIdx, tg);
//	}*/
//
//	return readAdrTg(curGtd->iLowPpa, curGtd->iHighPpa, tg, getAdrIdx(tg, idx));
//}


//uint getTgIdx(sDistFtl *curDistFtl, uint iGtdIdx, sTransGp *tg, uint idx){
//	uint iNdcp = 0, iNcp = 0;
//	uint i=0;
//	for(i=0 ; i < idx ; i++){
//		if( (tg->cpCbm[i/8] >> (8-(i%8))) & 0x1 )
//			iNcp++;
//		else
//			iNdcp++;
//	}
//	return iNdcp++;
//}
