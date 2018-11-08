#ifndef __DIST_FTL_UTILS_H__
#define __DIST_FTL_UTILS_H__

#include "struct_dist_ftl.h"

uchar isContd(uint idx, sTransGp *tg);

uchar isContdPg(uint prev, uint next);
uint getContdLen(uint iStartidx, sTransGp *tg);
uint getEleBitwidth(uint iLow, uint iHigh);

uint getNumOnes(uchar num);

void writeAdrTg(uint iLow, uint iHigh, sTransGp *tg, uint adr, uint adrCnt);
//uint readAdrTg(uint iLow, uint iHigh, sTransGp *tg, uint adrCnt);
uint readAdrTg(uint iLow, uint iHigh, sTransGp *tg, uint iStartOffset, uint iOffset);

uint getAdrIdx(sTransGp *tg, uint idx);
uint getLastStart(sTransGp *tg, uint idx);

uchar isSmaller(sCmtLine *line);
sDistGtd convertToTg(sCmtLine *line, sTransGp *tg, uint ch);
void convertToPlain(sDistGtd gtd, sTransGp *tg, sCmtLine *line);

uchar isTgGtd(sDistGtd *gtd);

uint	curTotalTgCnt();
uint	curAvgTgSize();

#endif
