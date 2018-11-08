#include "dist_ftl_utils.h"
#include "ftl_utils.h"
#include "addr_trans.h"
#include "math.h"

unsigned char gNumOnes[16]={
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
};

#define LEtoBE(x)( \
((x & 0x00000000000000ff) << 56) |	\
((x & 0x000000000000ff00) << 40) |	\
((x & 0x0000000000ff0000) << 24) |	\
((x & 0x00000000ff000000) << 8) |	\
((x & 0x000000ff00000000) >> 8) |	\
((x & 0x0000ff0000000000) >> 24) |	\
((x & 0x00ff000000000000) >> 40) |	\
((x & 0xff00000000000000) >> 56)	\
)

extern sNandSpec nandSpec;
extern sDistFtl distFtlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];

uchar isContd(uint idx, sTransGp *tg){
	return (((tg->cpCbm[idx/8]) >> (7-(idx%8))) & 0x1) ? UCHAR_TRUE : UCHAR_FALSE;    
}

uint getContdLen(uint iStartIdx, sTransGp *tg){
	uint idx = iStartIdx/8;
	uint ofs = iStartIdx%8;
	uint iContdLen = 1; 

	assert((tg->cpCbm[idx] >> (7-(idx%8))) & 0x1);

	do{
		if(!isContd(iStartIdx+iContdLen, tg))
			break;
		iContdLen++;
	}while(1);

	return iContdLen;
}

uint getEleBitwidth(uint iLow, uint iHigh){

	return getBitWidth((iHigh+1)-iLow)+1;	

}

uint getNumOnes(uchar num){
	return (gNumOnes[num >> 4] + gNumOnes[num & 0xf]);
}

uchar isSmaller(sCmtLine *line){
#if (__ADR_TRANS_SCHEME__==_DIST_FTL_)

	uchar transRequired = UCHAR_FALSE;
	uint iCbmLen = (uint)(ceil((float)_NUM_ELE_IN_LINE_ / 8)), iConCnt = 0;
	uint i;
	uint iPrevAdr = line->iAddrs[0];
	uint iBitWidth;
	uint iLowPpa = INVALID_VALUE, iHighPpa = 0;
	uint iAdrListCnt;
	uint iValidMapSize = iCbmLen;
	uint iValidAdrCnt = 0;

	if (line->iAddrs[0] != INVALID_VALUE){
		iValidAdrCnt++;
	}

	for (i = 1; i < _NUM_ELE_IN_LINE_; i++){		
		if (line->iAddrs[i] != INVALID_VALUE){

			iValidAdrCnt++;

			if (isContdPg(iPrevAdr, line->iAddrs[i])){
				iConCnt++;
			}

			if (line->iAddrs[i] < iLowPpa) { iLowPpa = line->iAddrs[i]; }
			if (line->iAddrs[i] > iHighPpa) { iHighPpa = line->iAddrs[i]; }

			iPrevAdr = line->iAddrs[i];
		}		
	}

	iBitWidth = getEleBitwidth(iLowPpa, iHighPpa);	
		
	//iAdrListCnt = ((((_NUM_ELE_IN_LINE_ - iConCnt) * iBitWidth) + iConCnt) + 7) / 8;
	iAdrListCnt = ((((iValidAdrCnt - iConCnt) * iBitWidth) + iConCnt) + 7) / 8;


	/// analytical modeling is required.

	if ((iAdrListCnt + iCbmLen + iValidMapSize) < _NUM_ELE_IN_LINE_ * _SINGLE_ELE_BYTE_){
		transRequired = UCHAR_TRUE;
	}	

	return transRequired;
#elif		(__ADR_TRANS_SCHEME__==_TWO_LEVEL_)
	return UCHAR_FALSE;
#endif
}

uint getAdrIdx(sTransGp *tg, uint oriIdx){
	uint i;
	uint iRetVal = 0;

	for (i = 0; i <= oriIdx; i++){
		if (!(tg->cpCbm[(i / 8)] & (0x1 << (7 - (i % 8))))){
			iRetVal++;
		}
	}
	
	return (iRetVal-1);
}

uint getLastStart(sTransGp *tg, uint idx){
	uint i;
	uint iRetVal = 0;

	for (i = 0; i < idx; i++){
		if (!(tg->cpCbm[(i / 8)] & (0x1 << (7 - (i % 8))))){
			iRetVal=i;
		}
	}

	return (iRetVal);

}

void convertToPlain(sDistGtd gtd, sTransGp *tg, sCmtLine *line){
	//uint iLowPpa = LONG_INVALID_VALUE, iHighPpa = 0x0;
	//uint iBitWidth;
	uint iCbmLen = (uint)(ceil((float)_NUM_ELE_IN_LINE_ / 8)), iConCnt = 0;
	uint iAdrListCnt = 0, iReadAdr = 0;
	//uint iPrevAdr;
	uint iLow = gtd.iLowPpa, iHigh = gtd.iHighPpa;
	int i;

	for (i = 0; i < _NUM_ELE_IN_LINE_; i++){
		uint idx = i / 8;
		uint offset = i % 8;	
		if (!(tg->cpCbm[idx] & (0x1 << (7 - offset)))){
			//record this!
			line->iAddrs[i] = readAdrTg(iLow, iHigh, tg, iReadAdr, i);
			iReadAdr++;			
		}else{
			sNandAddr tmpNand = intToNandAddr(line->iAddrs[i - 1]);
			if (tmpNand.iPage == nandSpec.iPagePerBlock - 1){
				tmpNand.iBlk++;
				tmpNand.iPage = 0;
			}
			else{
				tmpNand.iPage++;
			}

			line->iAddrs[i] = nandAddrtoInt(tmpNand);
			//line->iAddrs[i] = line->iAddrs[i - 1] + 1;
		}
	}
}

sDistGtd convertToTg(sCmtLine *line, sTransGp *tg, uint ch){
	uint iLowPpa = LONG_INVALID_VALUE, iHighPpa = 0x0;
	uint iBitWidth, iCbmLen = (uint)(ceil((float)_NUM_ELE_IN_LINE_ / 8)), iConCnt = 0;
	uint iAdrListCnt = 0, iRecordedAdr = 0;
	uint iPrevAdr;
	//uint iSize;
	sDistGtd newGtd;
	int iValidAdrCnt = 0;
	uint iValidMapSize = iCbmLen;
	
	int i;

	ftlAssert(tg != NULL, "TG is not allocated\n");

	// find high and low ppa + bitwidth
	for (i = 0; i < _NUM_ELE_IN_LINE_; i++){
		if (line->iAddrs[i] != INVALID_VALUE){
			if (line->iAddrs[i] < iLowPpa)	{ iLowPpa = line->iAddrs[i]; }
			if (line->iAddrs[i] > iHighPpa)	{ iHighPpa = line->iAddrs[i]; }
		}		
	}

	iBitWidth = getEleBitwidth(iLowPpa, iHighPpa);

	// allocate and initialize CBM
	ftlAssert(tg->cpCbm == NULL, "TG is not initialized yet!");
    ftlAssert(iCbmLen > 0, "length of cbm is calculated as zero\n");

    ACQUIRE_LOCK(ch);
    //barePrintf("3: ch = %d\n", ch);
    tg->cpCbm = (char *)malloc(iCbmLen);
    RELEASE_LOCK(ch);

	memset(tg->cpCbm, 0x00, iCbmLen);
    memorySize += 1;
	iPrevAdr = line->iAddrs[0];
	if (line->iAddrs[0] != INVALID_VALUE){
		iValidAdrCnt++;
	}

	for (i = 1; i < _NUM_ELE_IN_LINE_; i++){
		uint idx = i / 8;
		uint offset = i % 8;

		if (line->iAddrs[i] != INVALID_VALUE){
			iValidAdrCnt++;
		}

		if (line->iAddrs[i] != INVALID_VALUE && isContdPg(iPrevAdr, line->iAddrs[i])){
			tg->cpCbm[idx] |= (0x1 << (7 - offset));
			iConCnt++;
		}
		iPrevAdr = line->iAddrs[i];
	}

	// allocate and initialize addr list
	iAdrListCnt = ((((_NUM_ELE_IN_LINE_ - iConCnt) * iBitWidth) + iConCnt) + 7) / 8;
	ftlAssert(tg->cpAdrList == NULL, "TG is not initialized yet! (adrlist)");
    ACQUIRE_LOCK(ch);
    //barePrintf("4: ch = %d\n", ch);
	tg->cpAdrList = (char *)malloc(iAdrListCnt);
    RELEASE_LOCK(ch);
	
	for (i = 0; i < _NUM_ELE_IN_LINE_; i++){
		uint idx = i / 8;
		uint offset = i % 8;

		if (!(tg->cpCbm[idx] & (0x1 << (7 - offset)))){			
			writeAdrTg(iLowPpa, iHighPpa, tg, line->iAddrs[i], iRecordedAdr++);
			//iRecordedAdr++;				
		}		
	}

	ftlAssert(iValidAdrCnt > iConCnt, "wrong size calculation!!");
	iValidAdrCnt = ((((iValidAdrCnt - iConCnt) * iBitWidth) + iConCnt) + 7) / 8;
	ftlAssert(iValidAdrCnt >= 0, "negative table size calculation!!");
	

	newGtd.iHighPpa = iHighPpa;
	newGtd.iLowPpa = iLowPpa;
	//newGtd.iSize = (iCbmLen + iAdrListCnt);
	newGtd.iSize = (iCbmLen + iValidMapSize + iValidAdrCnt);

	return newGtd;
}

void writeAdrTg(uint iLow, uint iHigh, sTransGp *tg, uint adr, uint adrCnt){
	
	uint val = adr - iLow;
	uint iBitWidth = getEleBitwidth(iLow, iHigh);
	uint iHeadShift = (adrCnt * iBitWidth) % 8;
	uint iTailShift = 7-(((adrCnt + 1) * iBitWidth -1) % 8);
	uint iTotalBitWidth = iBitWidth + iTailShift + iHeadShift;
	uint nByte = (iTotalBitWidth) / 8;
	uchar *oriVal = &(tg->cpAdrList[(adrCnt * iBitWidth) / 8]);	
	uint newVal;
	uint oneMask = ((((uint)0x1 << iBitWidth) - 1) << (64 - iBitWidth)) >> iHeadShift;
	uint zeroMask = LONG_INVALID_VALUE ^ oneMask;
		
	ftlAssert(iTotalBitWidth % 8 == 0, "bit-width must be multiplication of 8\n");

	if (adr == INVALID_VALUE){
		val = oneMask;
	}
	else{
		val = (val << (64 - iBitWidth)) >> iHeadShift;
	}

	memcpy((&newVal), oriVal, nByte);
	newVal = LEtoBE(newVal);
	newVal = ((newVal & zeroMask) | val);
	newVal = LEtoBE(newVal);
	memcpy(oriVal, (&newVal), nByte);	
}

uint readAdrTg(uint iLow, uint iHigh, sTransGp *tg, uint iStartOffset, uint iOffset){
	
	uint iBitWidth = getEleBitwidth(iLow, iHigh);
	uint iHeadShift = (iStartOffset * iBitWidth) % 8;
	uint iTailShift = 7 - (((iStartOffset + 1) * iBitWidth - 1) % 8);
	uint iTotalBitWidth = iBitWidth + iTailShift + iHeadShift;
	uint nByte = (iTotalBitWidth) / 8;
	uchar *oriVal = &(tg->cpAdrList[(iStartOffset * iBitWidth) / 8]);
	
	uint val = 0;
	uint oneMask = ((uint)0x1 << iBitWidth) - 1;
	uint iDifference;
	uint i;
	sNandAddr tmpNand;
	uint cbmIdx = iStartOffset / 8;
	uint cbmOffset = iStartOffset % 8;
	
	ftlAssert(iTotalBitWidth % 8 == 0, "bit-width must be multiplication of 8\n");
	ftlAssert(iOffset >= iStartOffset, "iOffset should be larger than iStartOffset");
	/*ftlAssert(!(tg->cpCbm[cbmIdx] & (0x1 << (7 - (cbmOffset)))),
		"Invalid iStartOffset\n");*/

	memcpy(&val, oriVal, nByte);
	val = (((LEtoBE(val)) >> (64 - (iTotalBitWidth))) >> iTailShift) & oneMask;

	if (val == oneMask)
		return LONG_INVALID_VALUE;

	if (!(tg->cpCbm[iOffset / 8] & (0x1 << (7 - (iOffset % 8))))){
		return iLow + val;
	}
	else{	

		iDifference = iOffset - getLastStart(tg, iOffset);

		for (i = 0; i < iDifference; i++){
			tmpNand = intToNandAddr(iLow + val);
			if (tmpNand.iPage == nandSpec.iPagePerBlock - 1){
				tmpNand.iPage = 0;
				tmpNand.iBlk++;
			}
			else{
				tmpNand.iPage++;
			}
			val = (nandAddrtoInt(tmpNand) - iLow);
		}		
	}	
	
	return iLow + val;	
}

uchar isTgGtd(sDistGtd *gtd){

	if ((gtd->iHighPpa != gtd->iLowPpa) &&
		gtd->iHighPpa != LONG_INVALID_VALUE &&
		gtd->iLowPpa != LONG_INVALID_VALUE){
		return UCHAR_TRUE;
	}
	return UCHAR_FALSE;
}

uchar isContdPg(uint prev, uint next){

	sNandAddr prevNand = intToNandAddr(prev);
	sNandAddr nextNand = intToNandAddr(next);

	if (nextNand.iPage == 0){
		nextNand.iPage = nandSpec.iPagePerBlock - 1;
		nextNand.iBlk--;
	}
	else{
		nextNand.iPage--;
	}	

	if (prevNand.iBlk == nextNand.iBlk &&
		prevNand.iChannel == nextNand.iChannel &&
		prevNand.iWay == nextNand.iWay &&
		prevNand.iPlane == nextNand.iPlane &&
		prevNand.iPage == nextNand.iPage
		)
		return UCHAR_TRUE;

	return UCHAR_FALSE;
}

uint curTotalTgCnt(){
	uint iCh, iWy, iPl;
	uint iSum = 0;
	for (iCh = 0; iCh < NAND_ARRAY_CHANNEL; iCh++){
		for (iWy = 0; iWy < NAND_ARRAY_WAY; iWy++){
			for (iPl = 0; iPl < NAND_PLANE_PER_WAY; iPl++){
				iSum += distFtlMeta[iCh][iWy][iPl].cmt->iCurSize;
			}
		}
	}
	return iSum;
}

