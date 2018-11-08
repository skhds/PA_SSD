#ifndef __BACKGROUND_GC_H__
#define __BACKGROUND_GC_H__

#include "common_header.h"
#include "foreground_gc.h"
#include "bg_gc_dist_ftl.h"

#define BGC_BLK_CNT_THRESHOLD (RSVED_BLK_CNT)
#define BGC_TO_THRESHOLD_MS		15//(1000 * 1000000)
#define BGC_CNT_THRESHOLD			(10)

// initialization
void initBgc();

// outer
void doBgc();
uchar isBgcReq();

// inner
void reclaimBlkBg(uint iCh, uint iWy, uint iPl);
uchar isDevBgcReq(uint iCh, uint iWy, uint iPl);

#endif
