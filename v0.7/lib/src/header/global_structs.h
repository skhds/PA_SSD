#ifndef __GLOBAL_STRUCTS_H__
#define __GLOBAL_STRUCTS_H__


typedef enum eHOST_REQ_OP{
        
    HOST_WRITE = 0,
    HOST_READ = 1,
    HOST_NOTHING = 127
    
}HOST_REQ_OP;


typedef enum eSUB_REQ_OP{
    
    SUB_WRITE = 0,
    SUB_READ = 1,
    SUB_WRITE_RAM = 2,
    SUB_READ_RAM = 3,
    SUB_WRITE_NAND = 4,
    SUB_READ_NAND = 5,
    SUB_NOTHING = 127
    
}SUB_REQ_OP;

typedef struct sReq_t{

    uint            iId;            // unique identification of one req
    HOST_REQ_OP     Op;             // opcode
    uint64          iAddr;          // address (unit: sector)
    uint            iLen;           // length (sector count)
    double          dIssueTime;     // moment when this request comes into SSD

}Req_t;

typedef struct sSubReq_t{
    uint            iId;
    uint64          iStartAddr;     // start address (unit: sector)  
    uint            iLen;           // length (sector count)
    SUB_REQ_OP      op;
    int             dst;            //-1 : NAND, >0 : DRAM id
    uint            bitmap;
}SubReq_t;




//i don't like this, i might as well remove it later on...
typedef struct sPageBuf{

    unsigned char buf[PAGE_BYTES];

}PageBuf_t;



#endif
