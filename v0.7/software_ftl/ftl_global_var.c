#include "common_header.h"

//////////by TH for maptable//////////

sFtlMeta *ftlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];

//sFtlMeta    ftlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];
//////////////////////////////////////
sNandSpec	nandSpec;
sNandAddr	dummyNandAdr;
uint ADDR_NAND_MAN_BASE[NAND_ARRAY_CHANNEL];
uint OFFSET_FTL_COMPLETE[NAND_ARRAY_CHANNEL];
uint iCount;
sDramInfo   DramInfo[100];
uint logical_addr;
uint logical_length;
uint dram_count;

///////////////////////
int memorySize;

sNandAddr (*addrTrans)(uint);
void (*setMap)(uint, sNandAddr);

uint    (*getFreeBlk)(uint iCh, uint iWy, uint iPl);
uint	(*getTarBlk)(uint iCh, uint iWy, uint iPl, uint adr);
uint	(*getTarPg)(uint iCh, uint iWy, uint iPl, uint iBlkAdr, uint adr);
uint	(*getGcVictimBlk)(uint iCh, uint iWy, uint iPl);

void	(*blkReclaim)(uint iCh, uint iWy, uint iPl);
void	(*blkReclaimBg)(uint iCh, uint iWy, uint iPl);

void	(*flushMapCache)(void);

uchar debugFlag = UCHAR_FALSE;

//////// Device interface
uint SWPointer;
uint SWFlag;
uint DeviceSchedOffset;
uint FTLOffset;
uint CacheSchedOffset;
////////

uchar	g_isSequential;


double g_dClstSize;
int g_iClstCnt;

int *a;




