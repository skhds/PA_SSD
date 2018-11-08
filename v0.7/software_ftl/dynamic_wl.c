#include "dynamic_wl.h"

void initDynamicWl()
{
	int ch, wy, pl;

	for(ch=0; ch < NAND_ARRAY_CHANNEL ; ch++)
	{
		for(wy =0 ; wy < NAND_ARRAY_WAY ; wy++)
		{
			for(pl = 0 ; pl < NAND_PLANE_PER_WAY ; pl++)
			{
				sFtlMeta *curFtl = ftlMeta[ch][wy][pl];

				curFtl->iNumUsedBlk = 0;
				curFtl->iNumUsedUserBlk = 0;
				curFtl->iCurBlk = getFreeBlk(ch, wy, pl);
				curFtl->iCurPg = getWritablePage(&(curFtl->blocks[curFtl->iCurBlk]));
			}			
		}
	}
}

uint getFreeBlock(uint iCh, uint iWy, uint iPl){

	sFtlMeta *curFtl = ftlMeta[iCh][iWy][iPl];
	uint i;
	
	for(i=0; i < nandSpec.iBlockPerPlane ; i++){
		if(isBlockFree(&(curFtl->blocks[i]))){
			setBlockUsed(&(curFtl->blocks[i]));
			curFtl->iNumUsedBlk++;
			curFtl->iNumUsedUserBlk++;
			break;
		}
	}

#if DEBUG
	assert( i < nandSpec.iBlockPerPlane );
#else
	ftlAssert( i < nandSpec.iBlockPerPlane , "found idx should be smaller than block/plane" );
#endif

	return i;
}

uint getCurBlk(uint iCh, uint iWy, uint iPl, uint adr){

	if (isBlockFull(&(ftlMeta[iCh][iWy][iPl]->blocks[ftlMeta[iCh][iWy][iPl]->iCurBlk]))){
		ftlMeta[iCh][iWy][iPl]->iCurBlk = getFreeBlk(iCh, iWy, iPl);
		ftlMeta[iCh][iWy][iPl]->iCurPg = getWritablePage(&(ftlMeta[iCh][iWy][iPl]->blocks[ftlMeta[iCh][iWy][iPl]->iCurBlk]));
        }

	return ftlMeta[iCh][iWy][iPl]->iCurBlk;
}

uint getCurPg(uint iCh, uint iWy, uint iPl, uint iBlkAdr, uint adr){

	ftlAssert(ftlMeta[iCh][iWy][iPl]->iCurBlk == iBlkAdr, "invalid block address\n");
	return getWritablePage(&(ftlMeta[iCh][iWy][iPl]->blocks[ftlMeta[iCh][iWy][iPl]->iCurBlk]));

}

uint getGcVictim(uint iCh, uint iWy, uint iPl)
{
	sFtlMeta *curFtl = ftlMeta[iCh][iWy][iPl];
	uint i, iVictimAddr = INVALID_VALUE;
	uint iMinValid = nandSpec.iPagePerBlock + 1;

	for(i=0 ; i <nandSpec.iBlockPerPlane ; i++){
		if(!isBlockFree(&(curFtl->blocks[i])) &&
			!isBlockDead(&(curFtl->blocks[i])) &&
			getValidPageCnt(&(curFtl->blocks[i])) < iMinValid &&
			curFtl->iCurBlk != i &&
			!isMapBlock(&(curFtl->blocks[i]))		// added for dftl;
			)
		{
			iVictimAddr = i;
			iMinValid = getValidPageCnt(&(curFtl->blocks[i]));
		}
	}

#if DEBUG
	assert(iVictimAddr != INVALID_VALUE);
#else
	ftlAssert(iVictimAddr != INVALID_VALUE, "at least one victim should be found");
#endif

	return iVictimAddr;
}
