/********************************
 *
 * ssd_struct.h
 * :    file defining struct / enum used
 *      by ssd platform
 *
 * author           :   xoqhd1212
 * date             :   2014/01/02
 * last modified    :   2014/01/08
 *
 * ******************************/


#ifndef _SSD_STRUCT_H_
#define _SSD_STRUCT_H_

// maximum number of sectors one request includes
#define MAX_REQ_LEN         8192

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long long int uint64;

// request: transaction given by host to SSD
// command: sub-transaction or memory device transaction




// host request opcode
typedef enum eHOST_REQ_OP{
    HOST_WRITE      =   0,
    HOST_READ       =   1,
    HOST_NOTHING    =   127
}HOST_REQ_OP;

//DRAM opcode
typedef enum eDRAM_CMD{
    DRAM_WRITE      =   0,
    DRAM_READ,
    DRAM_NOTHING    =   31
}DRAM_CMD;

typedef enum eSOURCE_DEVICE{
    HOST        = 0,
    NAND 
}SOURCE_DEVICE;

typedef enum{
    READ_DRAM = 0,
    WRITE_DRAM = 1,
    READ_SLAVE = 2,
    READ_BOTH = 3,
    WRITE_SLAVE = 4,
    UPDATE_SLAVE = 5
}OP;

//Converted CacheCMD from COMMAND struct

typedef struct sCacheTrans_t{
    uint Id;
    uint Addr; //DRAM addr
    uint Op;
    uint Len; 
    uint lba; //LBA
}CacheTrans_t;

typedef struct sCacheCMD{
    CacheTrans_t cmd;
    SOURCE_DEVICE SourceDevice;
    bool isDataReady;

    //////////////////
    bool isCMDTrans;
    //////////////////
}CacheCMD;

//CPU side COMMAND list struct
typedef struct{
    OP op;
    uint slave_addr;
    uint slave_bitmap;
    uint dram_id;
    uint dram_bitmap;
}COMMAND;

// host request struct
typedef struct sReq_t{
    uint            iId;            // unique identification of one req
    HOST_REQ_OP     Op;             // opcode
    uint64          iAddr;          // address (unit: sector)
    uint            iLen;           // length (sector count)
    double          dIssueTime;     // moment when this request comes into SSD
}Req_t;

//Sub Request struct
typedef struct sSubReq_t{
    Req_t           oriReq;         // parent req
    uint64          iStartAddr;     // start address (unit: sector)  
    uint            iLen;           // length (sector count)
    double          dIssueTime;     // when this sub-request issued
    CacheCMD        cmd;
    uint            Id;
}SubReq_t;

typedef struct sReadSubResp_t{
    uint            Id;         // sub-read requests requiring response
    uint            logical_addr; // sub-read requests requiring response
    uint            len;
    uchar*          cpData;         // data pointer, e.g. readResp.cpData = new uchar[readResp.subReq.iLen * SECTOR_SIZE_BYTE]; 
}ReadSubResp_t;

typedef struct sReadResp_t{
    Req_t           hostReq;         // host read requests requiring response
    uchar*          cpData;         // data pointer, e.g. readResp.cpData = new uchar[readResp.subReq.iLen * SECTOR_SIZE_BYTE]; 
}ReadResp_t;

//typedef struct{
//    Req_t           req;            // parent req
//    uint*           ipBitMap;       // bitmap indicating completion of sub-req
//}CMD;


//Buffer request struct (Read Queue)
typedef struct sBufferReq_t{
    sSubReq_t subReq;
    bool      isTrans;
}BufferReq_t;




typedef enum eWriteSlaveState{
    IDLE        =       0,
    CMD                 ,
    NOTHING     =       63
}eWSState;

#define UNIT_OF_REQUEST 512

#endif
