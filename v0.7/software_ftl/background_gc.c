#include "background_gc.h"

void initBgc(){

}

void doBgc(){
	uint ch, wy, pl;
	for(ch=0; ch < NAND_ARRAY_CHANNEL ; ch++){
		for(wy =0 ; wy < NAND_ARRAY_WAY ; wy++){
			for(pl = 0 ; pl < NAND_PLANE_PER_WAY ; pl++){
				if(isDevBgcReq(ch, wy, pl)){
					//reclaimBlkBg(ch, wy, pl);
					blkReclaimBg(ch, wy, pl);
				}
			}
		}
	}
}

uchar isBgcReq(){
	uint ch, wy, pl;
	for(ch=0; ch < NAND_ARRAY_CHANNEL ; ch++)
		for(wy =0 ; wy < NAND_ARRAY_WAY ; wy++)
			for(pl = 0 ; pl < NAND_PLANE_PER_WAY ; pl++)
				if(isDevBgcReq(ch, wy, pl))
					return UCHAR_TRUE;

	return UCHAR_FALSE;
}

void reclaimBlkBg(uint iCh, uint iWy, uint iPl){
	uint iTargetAddr;
	uint i=0;
	sNandAddr srcPg, desPg, desBk;
	sPageMeta *pSrcPg;
	sNandCmd cmd;

	/// do gc work;

	iTargetAddr = getGcVictimBlk(iCh, iWy, iPl);//getGcVictim(iCh, iWy, iPl);

	for(i=0; i < nandSpec.iPagePerBlock ; i++)
	{
		if(isPageValid(&(ftlMeta[iCh][iWy][iPl]->blocks[iTargetAddr].page[i])))
		{
			pSrcPg = getPgPtr(iCh, iWy, iPl, iTargetAddr, i);
			srcPg = composeNandAddr(iCh, iWy, iPl, iTargetAddr, i, 0);

			if(isBlockFull(getBkPtr(iCh, iWy, iPl, ftlMeta[iCh][iWy][iPl]->iCurBlk))){
				ftlMeta[iCh][iWy][iPl]->iCurBlk = getFreeBlk(iCh, iWy, iPl);//getFreeBlock(iCh, iWy, iPl);
				ftlMeta[iCh][iWy][iPl]->iCurPg = getWritablePage(getBkPtr(iCh, iWy, iPl, ftlMeta[iCh][iWy][iPl]->iCurBlk));
			}

			desPg = composeNandAddr(iCh, iWy, iPl, ftlMeta[iCh][iWy][iPl]->iCurBlk, ftlMeta[iCh][iWy][iPl]->iCurPg++, 0);
			cmd = composeNandCmd(NAND_CMD_COPYBACK, desPg, srcPg, 0, NULL);
			registerNandCmd(cmd);	

			setL2p(pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE, desPg);
		}		
	}

	desBk = composeNandAddr(iCh, iWy, iPl, iTargetAddr, 0, 0);
	cmd = composeNandCmd(NAND_CMD_ERASE, desBk, desBk, 0, NULL);

	registerNandCmd(cmd);
}

uchar isDevBgcReq(uint iCh, uint iWy, uint iPl){
	// should be modified...
	//return ((NAND_BLOCK_PER_PLANE - ftlMeta[iCh][iWy][iPl].iNumUsedBlk) <= BGC_BLK_CNT_THRESHOLD);
	return ((DATA_BLK_CNT - ftlMeta[iCh][iWy][iPl]->iNumUsedUserBlk) <= BGC_BLK_CNT_THRESHOLD);
}
