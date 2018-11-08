#ifndef __EXTERN_FTL_GLOBAL_VAR_H__
#define __EXTERN_FTL_GLOBAL_VAR_H__

#include "common_header.h"

extern sNandSpec	nandSpec;
//extern sFtlMeta ftlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];
extern sFtlMeta *ftlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];
extern sNandAddr dummyNandAdr;
extern uint iCount;
extern sDramInfo    DramInfo[100];
extern uint logical_addr;
extern uint logical_length;
extern uint dram_count;
/////////
extern uint ADDR_NAND_MAN_BASE[NAND_ARRAY_CHANNEL];
extern uint OFFSET_FTL_COMPLETE[NAND_ARRAY_CHANNEL];

extern sNandAddr (*addrTrans)(uint);
extern void (*setMap)(uint, sNandAddr);

extern uchar debugFlag;

// wear-leveling

extern uint (*getFreeBlk)(uint iCh, uint iWy, uint iPl);
extern uint (*getTarBlk)(uint iCh, uint iWy, uint iPl, uint adr);
extern uint (*getTarPg)(uint iCh, uint iWy, uint iPl, uint iBlkAdr, uint adr);
extern uint (*getGcVictimBlk)(uint iCh, uint iWy, uint iPl);

extern void (*flushMapCache)(void);


// foreground gc
extern void (*blkReclaim)(uint iCh, uint iWy, uint iPl);

// background gc
extern void (*blkReclaimBg)(uint iCh, uint iWy, uint iPl);

/////////////
extern uint SWPointer;
extern uint SWFlag;
extern uint DeviceSchedOffset;
extern uint FTLOffset;
extern uint CacheSchedOffset;
///////////////

extern sDftl dftlMeta[NAND_ARRAY_CHANNEL][NAND_ARRAY_WAY][NAND_PLANE_PER_WAY];

extern uchar	g_isSequential;

extern double g_dClstSize;
extern int g_iClstCnt;

////////////////////
extern int memorySize;
extern int *a;

#endif
