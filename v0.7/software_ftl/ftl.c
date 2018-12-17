#include "ftl.h"
#include "sfr_addr.h"

#define _FTLMETA_OFFSET_ 0x20 // temporary
#define _FTLMETA_SIZE_  (920848) //size of ftlMeta


void initFtl()
{
	int ch, wy, pl, bk, pg, se;

	g_dClstSize = 0;

    //by Th 0103
    //barePrintf("meta size = %d\n", sizeof(ftlMeta));
	for(ch=0; ch < NAND_ARRAY_CHANNEL ; ch++)
	{
		for(wy =0 ; wy < NAND_ARRAY_WAY ; wy++)
		{
			for(pl = 0 ; pl < NAND_PLANE_PER_WAY ; pl++)
			{
                ftlMeta[ch][wy][pl] = (sFtlMeta *)(_ADDR_CACHE_BUFFER_BASE_ + _FTLMETA_OFFSET_ + 
                        (ch*NAND_ARRAY_WAY * NAND_PLANE_PER_WAY +  
                        wy * NAND_PLANE_PER_WAY + pl)* _FTLMETA_SIZE_ );
            }
        }
    }

                barePrintf("iCurBlk : : %d, last ele : %d\n", (uint)&(ftlMeta[0][0][0]->blocks[0].page[0]) - _ADDR_CACHE_BUFFER_BASE_, (uint)&(ftlMeta[0][0][0]->blocks[0].page[1]) - _ADDR_CACHE_BUFFER_BASE_, (uint)&(ftlMeta[0][0][0]->l2pTable[AVAIL_PG_CNT-1]) + 4 - _ADDR_CACHE_BUFFER_BASE_);
                
                barePrintf("base addr : %d, iCurBlk : : %d, last ele : %d\n", (uint)(ftlMeta[0][0][0]) - _ADDR_CACHE_BUFFER_BASE_, (uint)&(ftlMeta[0][0][0]->iCurBlk) - _ADDR_CACHE_BUFFER_BASE_, (uint)&(ftlMeta[0][0][0]->l2pTable[AVAIL_PG_CNT-1]) + 4 - _ADDR_CACHE_BUFFER_BASE_);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	nandSpec = getNandSpec();

	dummyNandAdr.isValid = UCHAR_FALSE;
	dummyNandAdr.iChannel = INVALID_VALUE;
	dummyNandAdr.iWay = INVALID_VALUE;
	dummyNandAdr.iPlane = INVALID_VALUE;
	dummyNandAdr.iBlk = INVALID_VALUE;
	dummyNandAdr.iPage = INVALID_VALUE;
		
	///// init meta;
	for(ch=0; ch < NAND_ARRAY_CHANNEL ; ch++)
	{
		for(wy =0 ; wy < NAND_ARRAY_WAY ; wy++)
		{
			for(pl = 0 ; pl < NAND_PLANE_PER_WAY ; pl++)
			{
				memset(ftlMeta[ch][wy][pl]->l2pTable, 0xff, sizeof(sMapTable) * AVAIL_PG_CNT);
					
                barePrintf("ch wy pl : %d, %d, %d\n", ch, wy, pl);

                for(bk = 0 ; bk < NAND_BLOCK_PER_PLANE ; bk++)
				{					
					setBlockFree(&(ftlMeta[ch][wy][pl]->blocks[bk]));
                    setEraseCnt(&(ftlMeta[ch][wy][pl]->blocks[bk]), 0);
					for(pg = 0 ; pg < NAND_PAGE_PER_BLOCK ; pg++)
					{
						setPageFree(&(ftlMeta[ch][wy][pl]->blocks[bk].page[pg]));
                //if( (ch==0)&& (wy==1)&& (pl==0) ) barePrintf("page addr : %x, l2p addr : %x, l2p value : %x\n",  &(ftlMeta[ch][wy][pl]->blocks[bk].page[pg]), &(ftlMeta[0][0][0]->l2pTable[0]), ftlMeta[0][0][0]->l2pTable[0] ); //0, 1, 0, 13, 29  memleak
						for(se = 0 ; se < SECTOR_PER_PAGE ; se++)
						{
							setSectorInvalid(&(ftlMeta[ch][wy][pl]->blocks[bk].page[pg]), se);
						}						
					}					
				}
			}			
		}
	}

    ///// init nand manager address
    for(ch=0; ch < NAND_ARRAY_CHANNEL; ch++){
        //by TH 0103
        //ADDR_NAND_MAN_BASE[ch] = _ADDR_NAND_MAN_BASE_ + ch * 0x1000;
        ADDR_NAND_MAN_BASE[ch] = _ADDR_NAND_MAN_BASE_;
        OFFSET_FTL_COMPLETE[ch] = 0x14 + ch * 0x4;
    }

	/// set function pointers

#if			(__ADR_TRANS_SCHEME__==_BASELINE_PAGE_MAPPING_)
	// address translation
	addrTrans = l2pTranslate;
	setMap = setL2p;
	// dynamic wear-leveling
	getFreeBlk = getFreeBlock;
	getTarBlk = getCurBlk;
	getTarPg = getCurPg;
	getGcVictimBlk = getGcVictim;
	//foreground gc
	blkReclaim = reclaimBlk;
	//background gc
	blkReclaimBg = reclaimBlkBg;

	// mapflush
	flushMapCache = dummyMapCacheFlush;
	getCurCmtCnt = dummayCmtCnt;
	
#elif		(__ADR_TRANS_SCHEME__==_DFTL_)
	// address translation
	addrTrans = dftlL2pTrans;
	setMap = DftlSetL2p;
	// dynamic wear-leveling
	getFreeBlk = getFreeBlock;	
	getTarBlk = getCurBlk;
	getTarPg = getCurPg;
	getGcVictimBlk = getGcVictim;
	// foreground gc
	blkReclaim = reclaimBlk;
	// background gc
	blkReclaimBg = reclaimBlkBg;

	// mapflush
	flushMapCache = mapFlush_dftl;
	getCurCmtCnt = curTotalCmtCnt;
	
#elif		(__ADR_TRANS_SCHEME__==_DIST_FTL_)
	// address translation
	addrTrans = distFtlL2pTrans;
	setMap = distFtlSetL2p;
	// dynamic wear-leveling
	//getFreeBlk = getFreeBlock;
	getFreeBlk = getFreeBlk_wrPtrDummy;
	//getTarBlk = getCurBlk;
	getTarBlk = getWriteBlk_wrPtr;
	//getTarPg = getCurPg;
	getTarPg = getWritePg_distFtl;
	//getGcVictimBlk = getGcVictim;
	getGcVictimBlk = getUserGcVictim_wrPtr;
	// dynamic wear-leveling
	/*getFreeBlk = getFreeBlk_distFtl;
	getTarPg = getWritePg_distFtl;
	getTarBlk = getWriteBlk_distFtl;
	getGcVictimBlk = getUserBlockVictim_distFtl;*/

	// foreground gc
#if (_SORTING_GC_==0)
	blkReclaim = userBlkGc_distFtl;		// gc without sorting
#elif (_SORTING_GC_==1)
	//blkReclaim = userBlkGc_wrPtr;		// gc with sorting
#else
	ftlAssert(0, "SORTING_GC should be defined\n")
#endif
	// background gc
	blkReclaimBg = bgc_distFtl;

	// mapflush
	flushMapCache = mapFlush_distFtl;

	// stat
	getCurCmtCnt = curTotalTgCnt;
#elif		(__ADR_TRANS_SCHEME__==_TWO_LEVEL_)

	// address translation
	addrTrans = distFtlL2pTrans;
	setMap = distFtlSetL2p;
	// dynamic wear-leveling
	getFreeBlk = getFreeBlk_distFtl;
	getTarPg = getWritePg_distFtl;
	getTarBlk = getWriteBlk_distFtl;
	getGcVictimBlk = getUserBlockVictim_distFtl;

	// foreground gc
	blkReclaim = userBlkGc_distFtl;
	// background gc
	blkReclaimBg = bgc_distFtl;

	// mapflush
	flushMapCache = mapFlush_distFtl;

	// address translation
	//addrTrans = dftlL2pTrans;
	//setMap = DftlSetL2p;
	//// dynamic wear-leveling
	//getFreeBlk = getFreeBlock;
	//getTarBlk = getCurBlk;
	//getTarPg = getCurPg;
	//getGcVictimBlk = getGcVictim;
	//// foreground gc
	//blkReclaim = reclaimBlk;
	//// background gc
	//blkReclaimBg = reclaimBlkBg;
	//// mapflush
	//flushMapCache = mapFlush_dftl;

	getCurCmtCnt = curTotalCmtCnt;

#else
#endif
	

#if			(__ADR_TRANS_SCHEME__==_BASELINE_PAGE_MAPPING_)
#elif		(__ADR_TRANS_SCHEME__==_DFTL_)
	initDftl();	
#elif		(__ADR_TRANS_SCHEME__==_DIST_FTL_)
	initDistFtl();
#elif		(__ADR_TRANS_SCHEME__==_TWO_LEVEL_)
	initDistFtl();
#else
#endif

	initDynamicWl();
	initGc();
	initAddrTrans();
	
}

void callFtl(DiskReq req)
{
	uint order = 0;
	sPageReq curReq;
	curReq.req = req;
/*
	if (req.sectorCount >= _SEQ_TH_){
		g_isSequential = UCHAR_TRUE;
	}
	else{
		g_isSequential = UCHAR_FALSE;
	}
*/
    // modified to accept bitmaps
    if (req.bitmap == _FULL_PAGE_){
        g_isSequential = UCHAR_TRUE;
    }
    else{
        g_isSequential = UCHAR_FALSE;
    }

    //
/*
	for(order = 0;pageChunkGen(req.sectorAddr, req.sectorCount, SECTOR_PER_PAGE, order, &curReq.startSector, &curReq.iLen, curReq.validMap); order++)
	{
		switch(curReq.req.cmd)
		{
		case DISK_CMD_WRITE:
			curReq.cmd = FTL_WRITE;
			ftlWrite(curReq);
			break;
		case DISK_CMD_READ:
			curReq.cmd = FTL_READ;
			ftlRead(curReq);
			
			break;
		default:
			break;
		}
	}
    */
	//	
    setPageReq(&(curReq.startSector), curReq.validMap, req);
    switch(curReq.req.cmd)
		{
		case DISK_CMD_WRITE:
			curReq.cmd = FTL_WRITE;
			ftlWrite(curReq);
			break;
		case DISK_CMD_READ:
			curReq.cmd = FTL_READ;
			ftlRead(curReq);
			
			break;
		default:
			break;
		}


}

void ftlWrite(sPageReq pageReq){

	sNandAddr phyAdr;
	sNandAddr newPhyAdr;
	uint iCh = getCh(pageReq.startSector);
	uint iWy = getWy(pageReq.startSector);
	uint iPl = getPl(pageReq.startSector);
	sNandCmd nandCmd;
	uchar cIsRnM = UCHAR_FALSE;
	uchar cPrevValidMap[VALID_BITMAP_SIZE] = {0};
	uint iTarBlk, iTarPg;
    ////////// if free block is not enough, do gc
    //ACQUIRE_LOCK(iCh);

	while(!isEnoughFreeBlk(iCh, iWy, iPl)){
		blkReclaim(iCh, iWy, iPl);//reclaimBlk(iCh, iWy, iPl);
	}
    //RELEASE_LOCK(iCh);

	//phyAdr = l2pTranslate(pageReq.startSector);
    //ACQUIRE_LOCK(iCh);
	phyAdr = addrTrans(pageReq.startSector);
	// get free page for incoming data
	iTarBlk = getTarBlk(iCh, iWy, iPl, pageReq.startSector);
	iTarPg = getTarPg(iCh, iWy, iPl, iTarBlk, pageReq.startSector);
    //RELEASE_LOCK(iCh);
	////////// if selected block is full, get another block from dynamic wl
	//if(isBlockFull(&(ftlMeta[iCh][iWy][iPl].blocks[ftlMeta[iCh][iWy][iPl].iCurBlk]))){
	//	ftlMeta[iCh][iWy][iPl].iCurBlk = getFreeBlk(iCh, iWy, iPl);//getFreeBlock(iCh, iWy, iPl);
	//	ftlMeta[iCh][iWy][iPl].iCurPg = getWritablePage(&(ftlMeta[iCh][iWy][iPl].blocks[ftlMeta[iCh][iWy][iPl].iCurBlk]));
	//}
	//////////// target address composing

	//iTarBlk = ftlMeta[iCh][iWy][iPl].iCurBlk;
	//iTarPg = ftlMeta[iCh][iWy][iPl].iCurPg;
    //ACQUIRE_LOCK(iCh);
	newPhyAdr = composeNandAddr(iCh, iWy, iPl, iTarBlk, iTarPg, getSe(pageReq.startSector));
	
    ////////// read-and-modify + invalid previously written page
	if(phyAdr.isValid == UCHAR_TRUE)
	{
		cIsRnM = isRnMRequired(phyAdr, pageReq.validMap);
        if(cIsRnM){
			getValidMap(getPgPtr(iCh, iWy, iPl, phyAdr.iBlk, phyAdr.iPage), cPrevValidMap);			
			nandCmd = composeNandCmd(NAND_CMD_RNM_READ,
				phyAdr, dummyNandAdr, pageReq.startSector/SECTOR_PER_PAGE, NULL);
			registerNandCmd(nandCmd);
		}

		nandPageInvalidate(phyAdr);
	}

    //RELEASE_LOCK(iCh);
	////////// composing nand command

    nandCmd = composeNandCmd(NAND_CMD_PROGRAM,
            newPhyAdr, dummyNandAdr, pageReq.startSector/SECTOR_PER_PAGE, pageReq.validMap);
	
    ////////// execuite the composed command
	registerNandCmd(nandCmd);
	////////// meta data management
	ftlMeta[iCh][iWy][iPl]->iCurPg++;

	if(cIsRnM){
		mergeValidMap(cPrevValidMap, pageReq.validMap, cPrevValidMap, SECTOR_PER_PAGE);
		setValidMap(getPgPtr(iCh, iWy, iPl, newPhyAdr.iBlk, newPhyAdr.iPage), cPrevValidMap);
	}

	////////// mapping table 	
	//setL2p(pageReq.startSector, newPhyAdr);
    //ACQUIRE_LOCK(iCh);
	setMap(pageReq.startSector, newPhyAdr);
    //RELEASE_LOCK(iCh);
/*
    nandCmd = composeNandCmd(NAND_CMD_PROGRAM,
		newPhyAdr, dummyNandAdr, pageReq.startSector/SECTOR_PER_PAGE, pageReq.validMap);
	
	registerNandCmd(nandCmd);
    */
}

void ftlRead(sPageReq pageReq){
	//sNandAddr phyAdr = l2pTranslate(pageReq.startSector);
	sNandAddr phyAdr = addrTrans(pageReq.startSector);
	sNandCmd nandCmd;

#if DEBUG
	assert(phyAdr.isValid == UCHAR_TRUE);
#else
	ftlAssert(phyAdr.isValid == UCHAR_TRUE, "read page should be valid");
#endif	

	nandCmd = composeNandCmd(NAND_CMD_READ,
		phyAdr, phyAdr, pageReq.startSector/SECTOR_PER_PAGE, pageReq.validMap);

	registerNandCmd(nandCmd);
}

uint64 getAddressableByte(){
	return ((uint64)AVAIL_PG_CNT * nandSpec.iChannelCount * nandSpec.iWayCount * nandSpec.iPlanePerDev);
}







