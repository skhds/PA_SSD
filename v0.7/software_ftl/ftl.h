#ifndef __FTL_H__
#define __FTL_H__

#include "common_header.h"
#include "dynamic_wl.h"
#include "foreground_gc.h"
#include "background_gc.h"
#include "addr_trans.h"
//#include "dftl_trans.h"
#include "ftl_stat.h"

typedef enum eFtlInternalCommand
{
	FTL_WRITE = 0,
	FTL_READ,
	FTL_NOTHING
}eFtlCmd;

typedef struct sPageRequest
{
	DiskReq req;
	eFtlCmd cmd;
	uint startSector;
	uint	iLen;
	uchar validMap[VALID_BITMAP_SIZE];
}sPageReq;

void initFtl();
void callFtl(DiskReq req);

void ftlWrite(sPageReq pageReq);
void ftlRead(sPageReq pageReq);

//////// meta data management
uint64 getAddressableByte();

//////// hardware connected functions
//void registerNandCmd(sNandCmd cmd);


#endif 
