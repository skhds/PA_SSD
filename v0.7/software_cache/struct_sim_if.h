#ifndef __STRUCT_SIM_IF_H__
#define __STRUCT_SIM_IF_H__

#include "common_header.h"

typedef enum{
    Unknown = 0,
    DRAM = 1,
    NAND = 2
}DestType;

typedef enum eHOST_REQ_OP{
	HOST_WRITE = 0,
	HOST_READ = 1,
	HOST_NOTHING = 127
}HOST_REQ_OP;


typedef struct sReq_t{
	uint		iId;
	HOST_REQ_OP Op;
	uint64		iAddr;
	uint		iLen;
	double		dIssueTime;
}Req_t;

typedef struct sSubReq_t{
    Req_t       oriReq;
    uint64      iStartAddr;
    uint        iLen;
    double      dIssueTime;
    uint        DRAM_Addr;
}SubReq_t;

///////////////////////////
#endif

