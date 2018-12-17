#ifndef __NAND_CMD_LAYER_H__
#define __NAND_CMD_LAYER_H__

#include "compile_flag.h"
#include "low_level_nand_spec.h"
//#include "struct_nand_spec.h"
#include "hw_if.h"
#include "sfr_addr.h"
#include "bare_utils.h"

/////////TH
#include "device_if.h"
#include "cache_buffer.h"

#ifdef _MCSSD_ARM_BARE_METAL_

/////////// enums and structrues

typedef enum lowLevelNandOpCode
{
	LLN_NOTHING = 0,
	LLN_PROGRAM = 1,
	LLN_READ = 2,
	LLN_ERASE = 3,
	LLN_COPYBACK = 4,
	LLN_COPYBACK_PGM = 5,
	LLN_COPYBACK_RD = 6,
    LLN_MAP_READ = 7,
    LLN_MAP_PROGRAM = 8,

    LLN_MAP_COPYBACK = 9,
    LLN_MAP_COPYBACK_PGM = 10,
    LLN_MAP_COPYBACK_RD = 11,

    LLN_MAP_ERASE = 12,

    //hotfix
    LLN_RNM_READ = 13,

	LLN_CHIP_SEL = 99,
	LLN_WAITING = 100

}eLowNandOp;

typedef enum nandCmdBufferStatus
{

	NAND_CMD_BUF_BUSY = 0,
	NAND_CMD_BUF_READY = 1,
	NAND_CMD_BUF_MAP_BUSY = 2,
	NAND_CMD_BUF_MAP_READY = 3

}eNandCmdBufStat;

typedef struct lowLevelNandCmd
{
	eLowNandOp	opCode;
	uint				iAddr1;
	uint				iAddr2;
	uint				iAddr3;
	uint				iAddr4;
}sLowNandCmd;

typedef struct nandCmdBuffer
{
	sLowNandCmd			cmd;
	uint						iIsMapTable;
	uint						iMapAddr;
}sNandCmdBuf;

void			testLowNandSpec(sNandSpec *spec);


///////////////////////////////////////////////
////// command converting
///////////////////////////////////////////////

eLowNandOp convertOp(eNandOp op);
uint				convertAddr(sNandAddr addr);
sLowNandCmd toLlnCmd(sNandCmd cmd);

///////////////////////////////////////////////
////// communication w/ hardware
///////////////////////////////////////////////

void			regLlnCmd(sLowNandCmd cmd, uint logicalAddr);
void			activateNand(uchar cIsLast, uint logicalAddr);
void			setFtlComplete(uint ch);
void			waitLln(uint logicalAddr);
//void			irqHandler(void);
void 			setFtlReady(void);

///////
#endif

#endif

