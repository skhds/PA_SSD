#ifndef __DYNAMIC_WL_H__
#define __DYNAMIC_WL_H__

#include "common_header.h"
#include "foreground_gc.h"
#include "dy_wl_dist_ftl.h"

void initDynamicWl();
uint getFreeBlock(uint iCh, uint iWy, uint iPl);
uint getGcVictim(uint iCh, uint iWy, uint iPl);


uint getCurBlk(uint iCh, uint iWy, uint iPl, uint adr);
uint getCurPg(uint iCh, uint iWy, uint iPl, uint iBlkAdr, uint adr);


#endif