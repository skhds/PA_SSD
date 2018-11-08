/*#include "dftl_trans.h"
#include "hw_if.h"
#include "dftl_meta.h"
#include "dynamic_wl_dftl.h"
#include "foreground_gc_dftl.h"

sDftl dftlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];

sDftlAdr adrIntoDftlAdr(uint adr){
	sDftlAdr retAdr;
	retAdr.iGtdIdx = getGtdIndex(adr);
	retAdr.iCmtKey = getCmtKey(adr);
	retAdr.iOffsetInLine = (uint)getOffsetInLine(adr);
	retAdr.iLineOffsetInPage = getLineOffsetInPage(adr);
	retAdr.iOffsetInPage = getOffsetInPage(adr);

	return retAdr;
}

void initDftl(){

	int i,j,k, l;

	for(i=0 ; i <NAND_ARRAY_CHANNEL ; i++){
		for(j=0 ; j <NAND_ARRAY_WAY ; j++){
			for(k=0 ; k < NAND_PLANE_PER_WAY ; k++){
				dftlMeta[i][j][k].cmt = listConstruct(0);
				dftlMeta[i][j][k].iNumMapBlock = 0;
				dftlMeta[i][j][k].mapBlockList = listConstruct(0);
				dftlMeta[i][j][k].iCurMapBlock = getNewMapBlock(i, j, k);				
				
				for(l=0 ; l < GTD_SIZE ; l++){
					dftlMeta[i][j][k].gtd[l].status = GTD_ELEMENT_INVALID;
					dftlMeta[i][j][k].gtd[l].iAddr = LONG_INVALID_VALUE;
					//dftlMeta[i][j][k].gtd[l].dirty = GTD_ELEMENT_CLEAN;
				}				
			}
		}
	}

	printf(" ## DFTL spec \n");
	printf(" ## GTD size: %d elements\n", GTD_SIZE);
	printf(" ## Min/Max map block #: %d/%d\n", _MIN_MAP_BLOCK_, _MAX_MAP_BLOCK_);
	printf(" ## Map block per Plane: %d\n", _MAP_BLOCK_PER_PL_);

	printf(" ## Single element size: %d-byte\n", _SINGLE_ELE_BYTE_);
	printf(" ## # element in cmt line: %d\n", _NUM_ELE_IN_LINE_);
	printf(" ## cmt line size: %d-byte\n", _CMT_LINE_SIZE_);
	printf(" ## cmt cache line number and size: %d, %d-byte\n", _NUM_CMT_IN_CACHE_, _CMT_SIZE_);

	printf(" ## NAND page size / # element in page: %d-byte/%d\n", (_MAP_NAND_PAGE_SIZE_), _NUM_ELE_IN_PAGE_);
	printf(" ## Total ND page cnt / GTD element cnt: %d / %d\n", _NAND_PAGE_CNT_, _NUM_GTD_ELEMENT_);

	printf(" ## CMT line per page: %d\n", _NUM_CMT_IN_PAGE_);
	
    barePrintf(" ## DFTL spec \n");
	barePrintf(" ## GTD size: %d elements\n", GTD_SIZE);
	barePrintf(" ## Min/Max map block #: %d/%d\n", _MIN_MAP_BLOCK_, _MAX_MAP_BLOCK_);
	barePrintf(" ## Map block per Plane: %d\n", _MAP_BLOCK_PER_PL_);

	barePrintf(" ## Single element size: %d-byte\n", _SINGLE_ELE_BYTE_);
	barePrintf(" ## # element in cmt line: %d\n", _NUM_ELE_IN_LINE_);
	barePrintf(" ## cmt line size: %d-byte\n", _CMT_LINE_SIZE_);
	barePrintf(" ## cmt cache line number and size: %d, %d-byte\n", _NUM_CMT_IN_CACHE_, _CMT_SIZE_);

	barePrintf(" ## NAND page size / # element in page: %d-byte/%d\n", (_MAP_NAND_PAGE_SIZE_), _NUM_ELE_IN_PAGE_);
	barePrintf(" ## Total ND page cnt / GTD element cnt: %d / %d\n", _NAND_PAGE_CNT_, _NUM_GTD_ELEMENT_);

	barePrintf(" ## CMT line per page: %d\n", _NUM_CMT_IN_PAGE_);
	


	
}

sNandAddr dftlL2pTrans(uint adr)
{
	sDftl *curDftl = &(dftlMeta[getCh(adr)][getWy(adr)][getPl(adr)]);
	uint pgAdr = getTableIdx(adr);
	sDftlAdr idx = adrIntoDftlAdr(pgAdr);
	sElePtr cmtElement = listFindKey(curDftl->cmt, (uint)idx.iCmtKey);
	sNandAddr retNandAddr = dummyNandAdr;

	incMapAccCnt();

	if(cmtElement != NULL){ // cmt hit
		
		uint iFoundAdr = ((sCmtLine *)(cmtElement->content))->iAddrs[idx.iOffsetInLine];
#if DEBUG
		assert(((sCmtLine *)(cmtElement->content))->iHeadAddr == idx.iCmtKey);
#else
		ftlAssert(((sCmtLine *)(cmtElement->content))->iHeadAddr == idx.iCmtKey, "head addr is not match with cmt key");
#endif
		listMoveToTail(curDftl->cmt, cmtElement);

		if(iFoundAdr != INVALID_VALUE)
			retNandAddr = intToNandAddr(iFoundAdr);

	}else{
		sGtdElemnt *gtdEle = &(curDftl->gtd[idx.iGtdIdx]);

		if(gtdEle->status == GTD_ELEMENT_INVALID){
			// this address has never been written
			// even its adjacent pages have never been written

			retNandAddr.isValid = UCHAR_FALSE;
		}else{
			// read nand and caching to cmt

			retNandAddr = fetchCmt(getCh(adr), getWy(adr), getPl(adr), gtdEle, idx);
		}
	}
	return retNandAddr;
}

void DftlSetL2p(uint adr, sNandAddr phyAdr){
	sDftl *dftl = &(dftlMeta[getCh(adr)][getWy(adr)][getPl(adr)]);
	uint pgAdr = getTableIdx(adr);
	sDftlAdr idx = adrIntoDftlAdr(pgAdr);
	sElePtr cmtElement = listFindKey(dftl->cmt, (uint)idx.iCmtKey);
	sNandAddr retNandAddr = dummyNandAdr;

	//dftl->gtd[idx.iGtdIdx].dirty = GTD_ELEMENT_DIRTY;

#if DEBUG
	assert(getCh(adr) == phyAdr.iChannel &&
		getWy(adr) == phyAdr.iWay &&
		getPl(adr) == phyAdr.iPlane
	);
#else
	ftlAssert((getCh(adr) == phyAdr.iChannel &&
		getWy(adr) == phyAdr.iWay &&
		getPl(adr) == phyAdr.iPlane), 
		"ch/wy/pl values are not matched"
		);
#endif

	/// increase map access
	incMapAccCnt();

	if(cmtElement != NULL){		// cmt hit

#if DEBUG
		assert(((sCmtLine *)(cmtElement->content))->iHeadAddr == idx.iCmtKey);
#else
		ftlAssert(((sCmtLine *)(cmtElement->content))->iHeadAddr == idx.iCmtKey, 
			"head addr is not matched with cmt key");
#endif
		((sCmtLine *)(cmtElement->content))->iAddrs[idx.iOffsetInLine] = nandAddrtoInt(phyAdr);		
		((sCmtLine *)(cmtElement->content))->dirty = CMT_DIRTY;
		listMoveToTail(dftl->cmt, cmtElement);

	}else{

		sGtdElemnt *gtdEle = &(dftl->gtd[idx.iGtdIdx]);

		if(gtdEle->status == GTD_ELEMENT_INVALID){


            sCmtLine *newLine;
            ACQUIRE_LOCK(getCh(adr));
            newLine = malloc(sizeof(sCmtLine));
            RELEASE_LOCK(getCh(adr));

			if(dftl->cmt->iCurSize >= _NUM_CMT_IN_CACHE_){
				evictCmt(getCh(adr), getWy(adr), getPl(adr));
			}

			memset(newLine->iAddrs, 0xff, sizeof(uint) * _NUM_ELE_IN_LINE_);
			newLine->iAddrs[idx.iOffsetInLine] = nandAddrtoInt(phyAdr);
			newLine->iHeadAddr = idx.iCmtKey;
			newLine->dirty = CMT_DIRTY;

			listPushTail(dftl->cmt, newLine->iHeadAddr, (void *)newLine, getCh(adr));

		}else{
			// read nand and caching to cmt
			retNandAddr = fetchCmt(getCh(adr), getWy(adr), getPl(adr), gtdEle, idx);
			cmtElement = listFindKey(dftl->cmt, (uint)idx.iCmtKey);
			
#if DEBUG
			assert(cmtElement != NULL);
			assert(((sCmtLine *)(cmtElement->content))->iHeadAddr == idx.iCmtKey);
#else
			ftlAssert(cmtElement != NULL, "cmtElement cannot be NULL");
			ftlAssert(((sCmtLine *)(cmtElement->content))->iHeadAddr == idx.iCmtKey, "head addr is not matched with cmt key");
#endif

			((sCmtLine *)(cmtElement->content))->iAddrs[idx.iOffsetInLine] = nandAddrtoInt(phyAdr);
			((sCmtLine *)(cmtElement->content))->dirty = CMT_DIRTY;

		}
	}	
}

sNandAddr fetchCmt(uint iCh, uint iWy, uint iPl, sGtdElemnt *gtd, sDftlAdr idx){

	sDftl *dftl = &(dftlMeta[iCh][iWy][iPl]);
	sNandAddr phyAdr = intToNandAddr(gtd->iAddr);
	sNandCmd nandCmd;
	sMapPage fetchedData;	
	sCmtLine* newCmtLine;	
	sNandAddr retAdr;

    ACQUIRE_LOCK(iCh);
	newCmtLine = (sCmtLine *)malloc(sizeof(sCmtLine));	
    RELEASE_LOCK(iCh);
	incMapFetchCnt();

	nandCmd = composeNandCmd(NAND_CMD_MAP_READ, phyAdr, dummyNandAdr, LONG_INVALID_VALUE, NULL);
	setMapBuffer(&nandCmd, (sPgBuf *)(&fetchedData));

	registerNandCmd(nandCmd);

	//// data is fetched to buffer;
	
	
	if(fetchedData.addr[idx.iOffsetInPage] == INVALID_VALUE){
		// if selected data is not valid, return invalid address

		retAdr.isValid = UCHAR_FALSE;
	}else{
		// selected data is valid
		// caching it to cmt		

		retAdr = intToNandAddr(fetchedData.addr[idx.iOffsetInPage]);		
	}
	
	newCmtLine->iHeadAddr = (uint)idx.iCmtKey;
	memcpy(newCmtLine->iAddrs, &(fetchedData.addr[idx.iLineOffsetInPage * _NUM_ELE_IN_LINE_]), _CMT_LINE_SIZE_);
	newCmtLine->dirty = CMT_CLEAN;

	addToCmt(iCh, iWy, iPl, newCmtLine);

	return retAdr;
}

void addToCmt(uint iCh, uint iWy, uint iPl, sCmtLine *newLine){

	sDftl *dftl = &(dftlMeta[iCh][iWy][iPl]);

	if(dftl->cmt->iCurSize >= _NUM_CMT_IN_CACHE_){
		evictCmt(iCh, iWy, iPl);
	}

	listPushTail(dftl->cmt, newLine->iHeadAddr, (void *)newLine, iCh);
}


void evictCmt(uint iCh, uint iWy, uint iPl){

	sDftl *dftl = &(dftlMeta[iCh][iWy][iPl]);
	sFtlMeta *curFtl = (ftlMeta[iCh][iWy][iPl]);
	sCmtLine *evictedLine = (sCmtLine *)(listGetHead(dftl->cmt)->content);
	uint iGtdIdx = (evictedLine->iHeadAddr) / _NUM_CMT_IN_PAGE_;
	uint iLineOffset = (evictedLine->iHeadAddr) % _NUM_CMT_IN_PAGE_;
	sNandAddr prevMapAdr; 
	sNandAddr newAdr;
	sMapPage prevMapData;
	sNandCmd nandCmd;

	memset(prevMapData.addr, 0xff, sizeof(uint) * _NUM_ELE_IN_PAGE_);

 	if(dftl->gtd[iGtdIdx].status != GTD_ELEMENT_INVALID)
	{
		prevMapAdr = intToNandAddr(dftl->gtd[iGtdIdx].iAddr);

		if (evictedLine->dirty == CMT_CLEAN){

			if (
				isMapBlock(&(curFtl->blocks[prevMapAdr.iBlk])) &&
				isPageValid(&(curFtl->blocks[prevMapAdr.iBlk].page[prevMapAdr.iPage])) &&
				!isBlockFree(&(curFtl->blocks[prevMapAdr.iBlk]))
				)
			{
				dftl->gtd[iGtdIdx].status = GTD_ELEMENT_NAND;
                ACQUIRE_LOCK(iCh);
				free(evictedLine);
				listDeleteHead(dftl->cmt);
                RELEASE_LOCK(iCh);
				return;
			}

		}

		nandCmd = composeNandCmd(NAND_CMD_MAP_READ, prevMapAdr, dummyNandAdr, LONG_INVALID_VALUE, NULL);
		setMapBuffer(&nandCmd, (sPgBuf *)(&prevMapData));
		registerNandCmd(nandCmd);
		nandMapInvalidate(prevMapAdr);
	}

	memcpy(&(prevMapData.addr[iLineOffset * _NUM_ELE_IN_LINE_]), evictedLine->iAddrs, _CMT_LINE_SIZE_);
		
	while(isMapBlkGcRequired(iCh, iWy, iPl)){
		reclaimMapBlk(iCh, iWy, iPl);
	}

	if(isBlockFull(&(ftlMeta[iCh][iWy][iPl]->blocks[dftl->iCurMapBlock]))){
		dftl->iCurMapBlock = getNewMapBlock(iCh, iWy, iPl);
	}

	newAdr = composeNandAddr(iCh, iWy, iPl, dftl->iCurMapBlock,
		getWritablePage(&(ftlMeta[iCh][iWy][iPl]->blocks[dftl->iCurMapBlock])),0);

	nandCmd = composeNandCmd(NAND_CMD_MAP_PROGRAM,
		newAdr, dummyNandAdr, LONG_INVALID_VALUE, NULL);
	setMapBuffer(&nandCmd, (sPgBuf *)(&prevMapData));

	registerNandCmd(nandCmd);

	// set gtd
	dftl->gtd[iGtdIdx].status = GTD_ELEMENT_NAND;
	dftl->gtd[iGtdIdx].iAddr = nandAddrtoInt(newAdr);

	// delete list element and data
#if DEBUG
	assert(evictedLine != NULL);
#else
	ftlAssert(evictedLine != NULL, "evicted line cannot be NULL");
#endif
    ACQUIRE_LOCK(iCh);
	free(evictedLine);
	listDeleteHead(dftl->cmt);
    RELEASE_LOCK(iCh);
}

void mapFlush_dftl(void){
	uint i, j, k;
	//sDftlAdr dummyIdx;
	for (i = 0; i < nandSpec.iChannelCount; i++){
		for (j = 0; j < nandSpec.iWayCount; j++){
			for (k = 0; k < nandSpec.iPlanePerDev; k++){
				sDftl *curMeta = &(dftlMeta[i][j][k]);
				if (curMeta->cmt->iCurSize > 0){
					evictCmt(i, j, k);
				}
			}
		}
	}
}

uint curTotalCmtCnt(){
	uint iCh, iWy, iPl;
	uint iSum = 0;
	for (iCh = 0; iCh < NAND_ARRAY_CHANNEL; iCh++){
		for (iWy = 0; iWy < NAND_ARRAY_WAY; iWy++){
			for (iPl = 0; iPl < NAND_PLANE_PER_WAY; iPl++){
				iSum += dftlMeta[iCh][iWy][iPl].cmt->iCurSize;
			}
		}
	}

	return iSum;
}*/
