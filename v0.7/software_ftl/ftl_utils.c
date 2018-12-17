#include "ftl_utils.h"
#include <assert.h>
#include "sfr_addr.h"


void ACQUIRE_LOCK(uint x){
    *((volatile unsigned int *)(_ADDR_MEMCTRL_BASE_ + _OFFSET_REQ_SOURCE_)) = x;
    while(*((volatile unsigned int *)(_ADDR_MEMCTRL_BASE_ + _OFFSET_GRANT_)) != x);

    //barePrintf("lock : ch = %d\n",x);
    
    return;
}

void RELEASE_LOCK(uint x){
    
    *((volatile unsigned int *)(_ADDR_MEMCTRL_BASE_ + _OFFSET_RELEASE_)) = x;
    //barePrintf("release : ch = %d\n",x);
    
    return;
}

uint getBitWidth(uint num)
{
	uint count = 0;
	uint orignal = num;
	uint i=0;

	while((num = (num >> 1))) {
		count++;
	}
	for(i=0; i < count ; i++)
	{
		if((orignal & (0x1 << i)) != 0)
		{
			count++;
			break;
		}
	}
	return count;
}

void computeSpecBitWidth(sNandSpec *spec)
{
	spec->iChannel_Bitwidth = getBitWidth(spec->iChannelCount);
	spec->iWay_Bitwidth = getBitWidth(spec->iWayCount);
	spec->iPageSize_Bitwidth = getBitWidth(spec->iPageSize);
	spec->iPagePerBlock_Bitwidth = getBitWidth(spec->iPagePerBlock);
	spec->iBlockPerPlane_Bitwidth = getBitWidth(spec->iBlockPerPlane);
	spec->iPlanePerDev_Bitwidth = getBitWidth(spec->iPlanePerDev);	

	//spec->iTotalByte = NAND_ARRAY_TOTAL_CAP;
}

//by skhds
//

void setPageReq(uint *startSector, uchar* validMap, DiskReq cmd){
    
    int i;
    uchar tmp;
    *startSector = cmd.sectorAddr;
    
    for(i =0; i<VALID_BITMAP_SIZE; i++){ //copying bitmap
        tmp = ( cmd.bitmap >> (i*8) ) % 0x100; 
        validMap[i] = ((tmp * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;   
    }
    

}


uchar pageChunkGen(uint iStartAdr, uint iSectorCnt, uint iSectorPerPage, uint order, uint *iAdr, uint *iLen, uchar *validMap)
{ //this cuts over-page sized requests into pages. Current SSD architecture cuts requests in the interface, therefore we no longer need this function.

	uint iCurAdr = iStartAdr + (iSectorPerPage * order);
	uint iLow = (iCurAdr / iSectorPerPage) * iSectorPerPage;
	uint iHigh = ((iCurAdr / iSectorPerPage) + 1) * iSectorPerPage;
	uint iOffset;

	ftlAssert(iHigh < (((uint64)DATA_BLK_CNT * NAND_PAGE_PER_BLOCK * NAND_ARRAY_CHANNEL * NAND_ARRAY_WAY * NAND_PLANE_PER_WAY / 512) * NAND_PAGE_BYTE), 
		"Too large address is given from the host\n");

	if(iLow > HIGHER_BOUND(iStartAdr, iSectorCnt))
		return UCHAR_FALSE;

	if(iLow < LOWER_BOUND(iStartAdr, iSectorCnt))
		iLow = LOWER_BOUND(iStartAdr, iSectorCnt);

	if(iHigh > HIGHER_BOUND(iStartAdr, iSectorCnt))
		iHigh = HIGHER_BOUND(iStartAdr, iSectorCnt);

	*iLen = (uint)((iHigh - iLow) + 1);
	*iAdr = iLow;

	memset(validMap, 0x00, VALID_BITMAP_SIZE);
		
	for(iOffset = 0 ; iOffset < *iLen ; iOffset++)
	{
		uint iCurOffset = iLow % SECTOR_PER_PAGE + iOffset;
		validMap[iCurOffset/8] |= ((SECTOR_VALID << 7) >> (iCurOffset%8));
	}


	return UCHAR_TRUE;

}

uchar isDifferentValidMap(uchar *a, uchar *b, uint size){
	uint i;	
	for(i=0 ; i < size/8 + 1 ; i++)
		if(a[i] != b[i])
			return UCHAR_TRUE;

	return UCHAR_FALSE;
}

void mergeValidMap(uchar *target, uchar *a, uchar *b, uint size){
	uint i;
	for( i=0; i < size/8 + 1 ; i++)
		target[i] = a[i] | b[i];	
	
}

sNandAddr composeNandAddr(uint iCh, uint iWy, uint iPl, uint iBk, uint iPg, uint iSe){
	sNandAddr ret;
	ret.isValid = UCHAR_TRUE;
	ret.iChannel = iCh;	ret.iWay = iWy;	ret.iPlane = iPl;
	ret.iBlk = iBk;	ret.iPage = iPg; ret.iSector = iSe;	
	return ret;
}

sNandCmd composeNandCmd(eNandOp op, sNandAddr des, sNandAddr src, uint iLogicalAdr, uchar* validMap){
	sNandCmd ret;

	ret.op = op;	ret.dest = des;	ret.src = src; ret.iLogicalAdr = iLogicalAdr; ret.cpValidMap = validMap;
	ret.dataBuffer = NULL;

	return ret;
}

//void setMapBuffer(sNandCmd *cmd, sMapPage *mapBuffer){
void setMapBuffer(sNandCmd *cmd, sPgBuf *dataBuf){

#if DEBUG
	assert(cmd->dataBuffer == NULL);
#else
	ftlAssert(cmd->dataBuffer == NULL, "another map buffer is used.");
#endif

	//cmd->dataBuffer = mapBuffer;	
	cmd->dataBuffer = dataBuf;
}

void ftlAssert(uchar condition, const char *msg){
    /*
	if(!condition){
		barePrintf("## req(%d) / ", getReqCnt());
		barePrintf("adr(%ld) / len(%d) / cmd(%c): \t", getCurReq().sectorAddr,
			getCurReq().sectorCount, (getCurReq().cmd == DISK_CMD_READ ? 'R' : 'W'));
		barePrintf(msg);
		//getchar();
		exit(1);
	}	
    */
}

