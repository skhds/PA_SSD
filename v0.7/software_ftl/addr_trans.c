#include "addr_trans.h"

const eAddrType		addrScheme[ADDR_TYPE_CNT] = ADDR_BIT_PARTITION;
uint						iAddrMask[ADDR_TYPE_CNT];
uint							iAddrShift[ADDR_TYPE_CNT];

void initAddrTrans(){
	
	uint i;
	uint shiftSum = 0;
	uint curBitWidth;

	for(i=0; i < ADDR_TYPE_CNT ; i++)
	{
		iAddrShift[addrScheme[i]] = shiftSum;

		switch(addrScheme[i])
		{
		case SECTOR_ADDR:
			curBitWidth = getBitWidth((uint)SECTOR_PER_PAGE);
			break;
		case PAGE_ADDR:
			curBitWidth = nandSpec.iPagePerBlock_Bitwidth;
			break;
		case BLOCK_ADDR:
			curBitWidth = nandSpec.iBlockPerPlane_Bitwidth;
			break;
		case PLANE_ADDR:
			curBitWidth = nandSpec.iPlanePerDev_Bitwidth;
			break;
		case WAY_ADDR:
			curBitWidth = nandSpec.iWay_Bitwidth;
			break;
		case CHANNEL_ADDR:
			curBitWidth = nandSpec.iChannel_Bitwidth;
			break;
		case ADDR_TYPE_CNT:
		default:
      assert(0);
			break;
		}
		
		shiftSum += curBitWidth;
		iAddrMask[addrScheme[i]] = ((1 << curBitWidth) - 1);
	}
}

sNandAddr l2pTranslate(uint adr)
{
	uint iTbIdx = getTableIdx(adr);

	return intToNandAddr(getFtlPtr(getCh(adr), getWy(adr), getPl(adr))->l2pTable[iTbIdx].iTransAddr);
}

uchar isRnMRequired(sNandAddr ori, uchar *income){
	uchar *cpOri = getValidMapPtr(&(ftlMeta[ori.iChannel][ori.iWay][ori.iPlane]->blocks[ori.iBlk].page[ori.iPage]));
	return isDifferentValidMap(cpOri, income, SECTOR_PER_PAGE);
}

void setL2p(uint adr, sNandAddr phyAdr)
{
	uint iTbIdx = getTableIdx(adr);
    ftlMeta[getCh(adr)][getWy(adr)][getPl(adr)]->l2pTable[iTbIdx].iTransAddr = 
		nandAddrtoInt(phyAdr);
}

sNandAddr intToNandAddr(uint adr)
{
	sNandAddr retVal;
	uint *target;
	uint i;

    //barePrintf("adr = %d\n", adr);
	if(adr == INVALID_VALUE)
	{
		retVal.isValid = UCHAR_FALSE;
	}
	else
	{
		retVal.isValid = UCHAR_TRUE;
		for(i=0; i < ADDR_TYPE_CNT ; i++)
		{
			switch(addrScheme[i])
			{
			case SECTOR_ADDR:
				target = &(retVal.iSector);
				break;
			case PAGE_ADDR:
				target = &(retVal.iPage);
				break;
			case BLOCK_ADDR:
				target = &(retVal.iBlk);
				break;
			case PLANE_ADDR:
				target = &(retVal.iPlane);
				break;
			case WAY_ADDR:
				target = &(retVal.iWay);
				break;
			case CHANNEL_ADDR:
				target = &(retVal.iChannel);
				break;
			}
			*target = (uint)((adr >> iAddrShift[addrScheme[i]]) & iAddrMask[addrScheme[i]]);
		}
	}
    
    return retVal;
}

uint getCh(uint adr) { return (uint)((adr >> iAddrShift[CHANNEL_ADDR]) & iAddrMask[CHANNEL_ADDR]); }
uint getWy(uint adr) { return (uint)((adr >> iAddrShift[WAY_ADDR]) & iAddrMask[WAY_ADDR]); }
uint getPl(uint adr) { return (uint)((adr >> iAddrShift[PLANE_ADDR]) & iAddrMask[PLANE_ADDR]); }
uint getPg(uint adr) { return (uint)((adr >> iAddrShift[PAGE_ADDR]) & iAddrMask[PAGE_ADDR]); }
uint getBk(uint adr) { return (uint)((adr >> iAddrShift[BLOCK_ADDR]) & iAddrMask[BLOCK_ADDR]); }
uint getSe(uint adr) { return (uint)((adr >> iAddrShift[SECTOR_ADDR]) & iAddrMask[SECTOR_ADDR]); }

uint getTableIdx(uint adr){
	uint retVal = 0;

    retVal += getPg(adr);
	retVal += (getBk(adr) << nandSpec.iPagePerBlock_Bitwidth);
	return retVal;
}

uint nandAddrtoInt(sNandAddr adr){
	uint retVal = 0;
	retVal += adr.iChannel << iAddrShift[CHANNEL_ADDR];
	retVal += adr.iWay << iAddrShift[WAY_ADDR];
	retVal += adr.iPlane << iAddrShift[PLANE_ADDR];
	retVal += adr.iBlk << iAddrShift[BLOCK_ADDR];
	retVal += adr.iPage << iAddrShift[PAGE_ADDR];
	retVal += adr.iSector << iAddrShift[SECTOR_ADDR];
	return retVal;
}

void dummyMapCacheFlush(){
	return;
}

uint dummayCmtCnt(){
	return 0;
}
