#ifndef __FOREGROUND_GC_H__
#define __FOREGROUND_GC_H__

#include "common_header.h"
#include "dynamic_wl.h"
#include "hw_if.h"
#include "addr_trans.h"
#include "fg_gc_dist_ftl.h"

#define FOREGROUND_GC_THRESHOLD		2

void initGc();
void reclaimBlk(uint iCh, uint iWy, uint iPl);
uchar isEnoughFreeBlk(uint iCh, uint iWy, uint iPl);

#endif