#ifndef __STRUCT_SIM_IF_H__
#define __STRUCT_SIM_IF_H__

#include "compile_flag.h"
#include "struct_nand_spec.h"

typedef enum sDiskCmd
{
	DISK_CMD_WRITE = 0,
	DISK_CMD_READ,
	DISK_CMD_BGC,
	DISK_CMD_NOTHING = 127                   // for extension
}DiskCmd;

typedef struct sDiskRequest
{
	uint sectorAddr;	            // addr 
	uint sectorCount;               // sector count
	DiskCmd cmd;                    // command
	uint li_issuedTick;           // 64-bit int for gem5 tick, do not need to use
	double dIssueTime;
}DiskReq;


typedef struct sNandAddress
{
	uchar isValid;
	uint iChannel;
	uint iWay;
	uint iPlane;
	uint iBlk;
	uint iPage;
	uint iSector;
}sNandAddr;

typedef enum
{
    NAND_CMD_PROGRAM,
	NAND_CMD_READ,
	NAND_CMD_COPYBACK,
	NAND_CMD_ERASE,
	NAND_CMD_COPYBACK_W,
	NAND_CMD_COPYBACK_R,

	NAND_CMD_RNM_READ,

	/// map_table_operations

	NAND_CMD_MAP_READ,
	NAND_CMD_MAP_PROGRAM,
	NAND_CMD_MAP_COPYBACK,
	NAND_CMD_MAP_COPYBACK_W,
	NAND_CMD_MAP_COPYBACK_R,
	NAND_CMD_MAP_ERASE,
	NAND_CMD_TYPE_CNT,

    NAND_CMD_TYPE_DUMMY = 4294967295

}eNandOp;

typedef struct sPageDataBuffer{
	uchar buf[NAND_PAGE_BYTE];
}sPgBuf;

/////////////////////TH
///////////// Device interface structure
typedef enum eHOST_REQ_OP{
	HOST_WRITE = 0,
	HOST_READ = 1,
	HOST_NOTHING = 127
}HOST_REQ_OP;

typedef struct sReq_t{
	uint		iId;
	HOST_REQ_OP Op;
	uint		iLen;
	uint		iAddr;
	double		dIssueTime;
}Req_t;
///////////////////////////
#if(_SOFTWARE_SIM_ == 1)
typedef struct sNandCommand
#else
#ifdef _MCSSD_ARM_BARE_METAL_
typedef __align(16) struct sNandCommand
#endif
#endif
{
	eNandOp			op;			// nand op code
	sNandAddr		dest;		// address for opcode, typically using this address only
	sNandAddr		src;		// source address for opcode like COPYBACK
	uint			iLogicalAdr;
	uchar				*cpValidMap;
	sPgBuf			*dataBuffer;	
}sNandCmd;

#endif
