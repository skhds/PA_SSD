#ifndef _CACHE_BUFFER_STRUCT_H_
#define _CACHE_BUFFER_STRUCT_H_

#include "SubReq_def.h"



//typedef struct BitMap_t{
//    uint64 	Addr;			// Start page address
//    uchar	Valid[SECTOR_PER_PAGE];
//}BitMap_t;
//
//typedef struct sSubReqWithValid_t{
//    sSubReq_t *subReq;
//    uchar *cpData;
//    uchar *Valid_bit;
//    uchar *Nand_bit;
//}sSubReqWithValid_t;
//
//typedef struct mSubReqWithData_t{
//    sSubReq_t *subReq;
//    CacheCMD cmd;
//    uchar *cpData;
//    bool check_trans;
//    bool readyTrans;
//}mSubReqWithData_t;
//
/////////////////// TH ///////////////
//typedef enum{
//    READ_DRAM = 0,
//    WRITE_DRAM = 1,
//    READ_SLAVE = 2,
//    WRITE_SLAVE = 3,
//    READ_BOTH = 4,
//    UPDATE_SLAVE = 5
//}OP;
//
//typedef struct{
//    OP op;
//    uint slave_addr;
//    uint slave_bitmap;
//    uint dram_id;
//    uint dram_bitmap;
//}COMMAND;

#endif

