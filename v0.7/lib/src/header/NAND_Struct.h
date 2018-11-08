#ifndef _NAND_STRUCT_H_
#define _NAND_STRUCT_H_

#include "ssd_struct.h"


#define NAND_BUF_ENTRY 8
#define NAND_CMD_QUEUE_SIZE 8


/* BUS CMD MASTER MEMORY MAP */

#define _ADDR_NUMS_BASE_    0x10000000
#define _ADDR_DEVIF_BASE_   0xC0000000
#define _ADDR_IRQ_BASE_     0x80000000
#define _ADDR_MEMCON_BASE_  0x11000000
#define _ADDR_SUB_BASE_     0xE0000000
#define _ADDR_DRAM_BASE_    0xD0000000

/* BUS CMD MASTER OFFSETS */

#define _CMDMASTER_OFFSET_NANDSIDE_ 0x4


/* BUS DATA MASTER MEMORY MAP */

#define _ADDR_DATA_DRAM_ 0x20000000
#define _ADDR_DATA_SUBM_ 0x10000000

/* BUS DATA MASTER OFFSETS */

#define _DATAMASTER_OFFSET_NANDSIDE_ 0x4



/* BUS MEMORY OFFSETS */

#define _ADDR_CPU2_BASE_        0x000

#define _OFFSET_DRAM_ADDR_      0x03C
#define _OFFSET_DRAM_LENGTH_    0x040
#define _OFFSET_DRAM_REQ_ID_    0x044
#define _OFFSET_NAND_CMD_       0x020
#define _OFFSET_NAND_ADDR1_     0x024
#define _OFFSET_NAND_ADDR2_     0x028
#define _OFFSET_NAND_ADDR3_     0x02C
#define _OFFSET_NAND_ADDR4_     0x030
#define _OFFSET_FTL_MAX_        0x050

//for eviction
#define _OFFSET_IRQ_BASE_       0x080

#define _ADDR_CPU1_BASE_         0x100
#define _OFFSET_IRQ_ID_          0x000
#define _OFFSET_IRQ_ADDR_        0x004
#define _OFFSET_IRQ_DONE_        0x00C
#define _OFFSET_IRQ_MAX_         0x010

#define _OFFSET_DIRECT_         0x020
#define _OFFSET_HOST_ID_        0x024
#define _OFFSET_HOST_ADDR_      0x028
#define _OFFSET_DIRECT_MAX_     0x030

#define _OFFSET_NAND_COUNT_     0x040
#define _OFFSET_RAM_COUNT_      0x044

/* OTHER DEFINITIONS */

#define _NAND_DATA_BUFFER_MAX_SIZE 20*DATA_PAGE_SIZE

typedef enum NAND_CMD_SET
{
	Nothing             = 0,
	Program             = 1,
	Read                = 2,
	Erase               = 3,
	Copyback            = 4,
	Copyback_Program    = 5,
	Copyback_Read       = 6,

    MapTable_Read       = 7, // iAddr1: nand // iAddr2: memory // iAddr3: offset (byte) // iAddr4: length (byte)
    MapTable_Program    = 8,
    MapTable_Copyback   = 9,
    MapTable_Copyback_Pgm = 10,
    MapTable_Copyback_Rd = 11,
    MapTable_Erase = 12,

	Waiting             = 100,

	Chip_Select         = 99 // Added by jun
}NAND_CMD_SET;


typedef struct NAND_Cmd{
    NAND_CMD_SET opCode; 
    unsigned int iAddr1; //PBA
    unsigned int iAddr2; //nothing
    unsigned int iAddr3; //LBA
    unsigned int iAddr4; //len
}NAND_Cmd;

typedef struct NAND_Cmd_Buffer{
    NAND_Cmd        cmd;
    unsigned int    DRAM_id;
}NAND_Cmd_Buffer;

typedef enum eDATA_STATE{

    BUFFER_FREE = 0,
    OCCUPIED = 1

}DATA_STATE;

typedef struct sDataBufferEntry{
    
    unsigned char data[16384];
    DATA_STATE state;
    unsigned int    DRAM_id;

}NANDData_t;


typedef enum{

    DONE = 0,
    IN_TRANSACTION = 1

}eIRQState;

typedef struct sIRQReq_t{

    unsigned int id;
    unsigned int addr;
    eIRQState state;

}IRQReq_t;

typedef enum sReqHOSTState{
    REQ_EMPTY = 0,
    NOT_READY = 1,
    REQ_READY = 2
}reqHOSTState;

typedef struct sDirectNANDReq{
    reqHOSTState state;
    unsigned int id;
    unsigned int addr;
    

}directNANDReq_t;


#endif
