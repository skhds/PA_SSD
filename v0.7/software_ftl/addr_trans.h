#ifndef __ADDR_TRANS_H__
#define __ADDR_TRANS_H__

#include "common_header.h"
#include "dftl_trans.h"
#include "dist_ftl_trans.h"


typedef enum eAddrType
{
	SECTOR_ADDR,
	PAGE_ADDR,
	BLOCK_ADDR,
	PLANE_ADDR,
	WAY_ADDR,
	CHANNEL_ADDR,
	ADDR_TYPE_CNT
}eAddrType;

void initAddrTrans();
sNandAddr l2pTranslate(uint adr);
uchar			isRnMRequired(sNandAddr ori, uchar *income);

void setL2p(uint adr, sNandAddr phyAdr);

sNandAddr intToNandAddr(uint adr);
uint		nandAddrtoInt(sNandAddr adr);

uint getTableIdx(uint adr);
uint getCh(uint adr);
uint getWy(uint adr);
uint getPl(uint adr);
uint getPg(uint adr);
uint getBk(uint adr);
uint getSe(uint adr);

void dummyMapCacheFlush();
uint dummayCmtCnt();

#endif