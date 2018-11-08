#include "foreground_gc_dftl.h"
#include "dynamic_wl_dftl.h"
//
//void reclaimMapBlk(uint iCh, uint iWy, uint iPl){
//
//	uint iTargetMapAdr = getMapBlockVictim(iCh, iWy, iPl);
//	uint i, j;
//	sNandAddr srcPgAdr, desPgAdr, desBkAdr;
//	sPageMeta *pSrcPg;
//	sNandCmd cmd;
//	sFtlMeta *ftl = &(ftlMeta[iCh][iWy][iPl]);
//	sDftl *dftl = &(dftlMeta[iCh][iWy][iPl]);
//
//#if DEBUG
//	assert((dftl->iNumMapBlock >= _MAP_BLOCK_PER_PL_));
//#else
//	ftlAssert((dftl->iNumMapBlock >= _MAP_BLOCK_PER_PL_), "# map block should be larger than threshold");
//#endif
//
//	for(i=0; i < nandSpec.iPagePerBlock ; i++)
//	{
//		if(isPageValid(&(ftl->blocks[iTargetMapAdr].page[i]))){
//			pSrcPg = getPgPtr(iCh, iWy, iPl, iTargetMapAdr, i);
//			srcPgAdr = composeNandAddr(iCh, iWy, iPl, iTargetMapAdr, i, 0);
//
//			if(isBlockFull(&(ftl->blocks[dftl->iCurMapBlock]))){
//				dftl->iCurMapBlock = getNewMapBlock(iCh, iWy, iPl);				
//			}
//
//			desPgAdr = composeNandAddr(iCh, iWy, iPl,
//				dftl->iCurMapBlock, getWritablePage(&(ftl->blocks[dftl->iCurMapBlock])), 0);
//
//			cmd = composeNandCmd(NAND_CMD_MAP_COPYBACK,
//				desPgAdr, srcPgAdr, LONG_INVALID_VALUE, NULL);
//
//			registerNandCmd(cmd);
//
//			for(j=0; j < GTD_SIZE ; j++){
//				if(dftl->gtd[j].iAddr == nandAddrtoInt(srcPgAdr)){
//#if DEBUG
//					assert(dftl->gtd[j].status == GTD_ELEMENT_NAND);
//#else
//					ftlAssert(dftl->gtd[j].status == GTD_ELEMENT_NAND, "found GTD element should be valid");
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
//
//			dftl->gtd[j].iAddr = nandAddrtoInt(desPgAdr);			
//		}
//	}
//
//	desBkAdr = composeNandAddr(iCh, iWy, iPl, iTargetMapAdr, 0, 0);
//	cmd = composeNandCmd(NAND_CMD_MAP_ERASE, desBkAdr, dummyNandAdr, 0, NULL);
//	registerNandCmd(cmd);
//
//	dftl->iNumMapBlock--;
//	listDeleteEle(dftl->mapBlockList, listFindKey(dftl->mapBlockList, getBk(nandAddrtoInt(cmd.dest))));	
//}
//
//uchar isMapBlkGcRequired(uint iCh, uint iWy, uint iPl){
//	return (dftlMeta[iCh][iWy][iPl].iNumMapBlock > _MAP_BLOCK_PER_PL_) ? UCHAR_TRUE : UCHAR_FALSE;
//}
