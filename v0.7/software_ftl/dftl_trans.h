#ifndef __DFTL_TRANS_H__
#define __DFTL_TRANS_H__

#include "common_header.h"
#include "addr_trans.h"

#define _NUM_CMT_IN_PAGE_						((_NUM_ELE_IN_PAGE_ / _NUM_ELE_IN_LINE_))

#define getGtdIndex(x)							( (x / _NUM_ELE_IN_PAGE_) )
#define getCmtKey(x)							( (x / _NUM_ELE_IN_LINE_) )
#define getOffsetInLine(x)						( (x % _NUM_ELE_IN_LINE_) )
#define getLineOffsetInPage(x)					( (getCmtKey(x) % _NUM_CMT_IN_PAGE_) )
#define getOffsetInPage(x)						( (x % _NUM_ELE_IN_PAGE_))

typedef struct DftlAderess{
	/*

	|                    Address                        |
	| GTD Index |  Line Offset in Page | Offset in Line |
	|         Cmt Key value            | Offset in Line |
	| GTD Index |            Offset in Page             |
	
	*/

	uint		iGtdIdx;							// gtd index
	uint		iCmtKey;							// cmt key value, cmt idx
	uint			iOffsetInLine;				// offset in line
	uint			iLineOffsetInPage;		// line offset in page
	uint			iOffsetInPage;				// offset in page

}sDftlAdr;

void initDftl(void);
sNandAddr dftlL2pTrans(uint adr);
void DftlSetL2p(uint adr, sNandAddr phyAdr);

sNandAddr fetchCmt(uint iCh, uint iWy, uint iPl, sGtdElemnt *gtd, sDftlAdr idx);

void addToCmt(uint iCh, uint iWy, uint iPl, sCmtLine *newLine);
void evictCmt(uint iCh, uint iWy, uint iPl);
sDftlAdr adrIntoDftlAdr(uint adr);

void mapFlush_dftl(void);

uint curTotalCmtCnt();

#endif