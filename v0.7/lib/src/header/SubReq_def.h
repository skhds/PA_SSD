#ifndef _SUBREQ_DEF_H_
#define _SUBREQ_DEF_H_

#include "ssd_struct.h"

#define KILO                    1024
#define CLOCK_PERIOD_NS         5       // Assuming 200MHz? it should be parameterized

/// Todo: find definition from other files
#define SECTOR_PER_PAGE         32


#define _ADDR_DRAM_CMD_ 0xd0000000 
#define _ADDR_DRAM_DATA_ 0x20000000
#define _ADDR_NAND_CMD_ 0xf0000000
#define _ADDR_NAND_DATA_ 0x30000000
#define _OFFSET_DATA_REGION_ 0x4000000

///////// TH ////////
typedef enum{
    CombRead = 0,
    CombWrite = 1
}CombType;

typedef enum{
    dummyReq = 0,
    hostReq = 1,
    combineReq = 2
}ReqType;


/////////////////////

typedef struct sIntReq_t{
    Req_t           req;                // parent req
    uint            iNextSector;        // indicate the next sector to be handled 
    bool            state;
    uchar*          cpBitMap;           // bitmap indicating completion of sub-req
    uchar*          cpDataBuf;          // data buffer for each register        
   }sIntReq_t;

////////////cache struct
typedef struct sSubReqWithData_t{
    sSubReq_t *subReq;
    uchar *cpData;
    ///////TH//////
    //CacheCMD cmd;       //cache command
    ReqType    reqType;    //
    //uint Id;            //request id
    //////////////
}sSubReqWithData_t;

typedef struct BitMap_t{
    uint64 	Addr;			// Start page address
    uchar	Valid[SECTOR_PER_PAGE];
}BitMap_t;
/*
///////////nand struct not used
typedef struct mSubReqWithData_t{
    sSubReq_t *subReq;
    uchar *cpData;
    ///////TH////////
    bool check_trans;   
    bool readyTrans;
    /////////////////
}mSubReqWithData_t;



typedef struct sSubReqWithValid_t{
    /////////TH
    CombType type;
    uint Id;
    /////////////
    sSubReq_t *subReq;
    uchar *cpData;
    uchar Valid_bit;
    uchar Nand_bit;
}sSubReqWithValid_t;

*/

#endif
