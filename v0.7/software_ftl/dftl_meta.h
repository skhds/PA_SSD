#ifndef __DFTL_META_H__
#define __DFTL_META_H__

#include "ftl_meta.h"

void nandMapProgram(sNandCmd cmd);
void nandMapRead(sNandCmd cmd);
void nandMapCopyback(sNandCmd cmd);
void nandMapErase(sNandCmd cmd);
void nandMapInvalidate(sNandAddr adr);

/// dftl related;
///// page related
void setPageMapData(sPageMeta *page, sMapPage *mapData);
sMapPage *getPageMapData(sPageMeta *page);
void freePageMapData(sPageMeta *page);

///// block related
void setMapBlock(sBlockMeta *blk);
uchar isMapBlock(sBlockMeta *blk);

#endif