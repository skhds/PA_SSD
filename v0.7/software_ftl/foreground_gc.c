#include "foreground_gc.h"

void initGc()
{
}

void reclaimBlk(uint iCh, uint iWy, uint iPl)
{
	uint iTargetAddr = getGcVictimBlk(iCh, iWy, iPl);//getGcVictim(iCh, iWy, iPl);
	uint i=0;
	sNandAddr srcPg, desPg, desBk;
	sPageMeta *pSrcPg;
	sNandCmd cmd;
	
	/// do gc work;
		
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

			//setL2p(pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE, desPg);
			setMap(pSrcPg->iLogicalPgAddr * SECTOR_PER_PAGE, desPg);
		}		
	}

	desBk = composeNandAddr(iCh, iWy, iPl, iTargetAddr, 0, 0);
	cmd = composeNandCmd(NAND_CMD_ERASE, desBk, desBk, 0, NULL);
	registerNandCmd(cmd);
}


uchar isEnoughFreeBlk(uint iCh, uint iWy, uint iPl){
	return ((DATA_BLK_CNT - getFtlPtr(iCh, iWy, iPl)->iNumUsedUserBlk) > 2);
	//return ((AVAIL_BLK_CNT - getFtlPtr(iCh, iWy, iPl)->iNumUsedUserBlk) > 2);
	//return (ftlMeta[iCh][iWy][iPl].iNumUsedBlk < AVAIL_BLK_CNT);
}

