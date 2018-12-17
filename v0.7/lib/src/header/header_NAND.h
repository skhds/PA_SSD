#ifndef __HEADER_NAND_H__
#define __HEADER_NAND_H__

#include "utils/Queue.h"

#define BWC

#define BITWIDTH_NAND_CTRL		32
#define NAND_CTRL_CLK_PERIOD 5
#define NAND_CLK_PERIOD   5

typedef enum NAND_TIMING
{
    T_PROG = 660000,
    T_R = 45000,
    T_BER = 3500000
}NAND_TIMING;


//for masking
int int_to_bit(int v){ //http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightLinear
    int c;
    for(c=0;v;c++) v>>=1;
    return c - 1;
}

unsigned int iFileNumber;


const int BYTE_PER_SECTOR_BIT = int_to_bit(SECTOR_BYTES);
const int SECTOR_PER_PAGE_BIT = int_to_bit(SECTOR_PER_PAGE);
const int PAGE_PER_BLOCK_BIT  = int_to_bit(PAGE_PER_BLOCK);
const int BLOCK_PER_WAY_BIT   = int_to_bit(BLOCK_PER_WAY);
const int WAY_PER_CHANNEL_BIT = int_to_bit(WAY_PER_CHANNEL);
const int CHANNEL_PER_SSD_BIT = int_to_bit(CHANNEL_PER_SSD);

#ifndef BWC
// Bit offset for each attribute (Channel at MSB)
const int BIT_FOR_PAGE = SECTOR_PER_PAGE_BIT;
const int BIT_FOR_BLOCK = BIT_FOR_PAGE + PAGE_PER_BLOCK_BIT;
const int BIT_FOR_WAY = BIT_FOR_BLOCK + BLOCK_PER_WAY_BIT;
const int BIT_FOR_CHANNEL = BIT_FOR_WAY + WAY_PER_CHANNEL_BIT;
#else
// Bit offset for each attribute (Channel at LSB)
const int BIT_FOR_CHANNEL = SECTOR_PER_PAGE_BIT;       //4
const int BIT_FOR_WAY = BIT_FOR_CHANNEL + CHANNEL_PER_SSD_BIT;  //7
const int BIT_FOR_PAGE = BIT_FOR_WAY + WAY_PER_CHANNEL_BIT; //10
const int BIT_FOR_BLOCK = BIT_FOR_PAGE + PAGE_PER_BLOCK_BIT; //16
#endif

// address maskings
const int MASK_CHANNEL = ((0x1 << CHANNEL_PER_SSD_BIT) - 1) << BIT_FOR_CHANNEL;
const int MASK_WAY = ((0x1 << WAY_PER_CHANNEL_BIT) - 1) << BIT_FOR_WAY;
const int MASK_BLOCK = ((0x1 << BLOCK_PER_WAY_BIT) - 1) << BIT_FOR_BLOCK;
const int MASK_PAGE = ((0x1 << PAGE_PER_BLOCK_BIT) - 1) << BIT_FOR_PAGE;


//im too lazy to fix _ctrl and _dev.....
#define SPARE_PAGE_SIZE 128
#define NAND_I_O_BITWIDTH 8
#define DATA_PAGE_SIZE PAGE_BYTES
#define NUM_OF_PAGE_PER_BLOCK PAGE_PER_BLOCK
#define NUM_OF_CHANNEL CHANNEL_PER_SSD
#define NUM_OF_WAY WAY_PER_CHANNEL
#define NUM_OF_BLOCK_PER_WAY BLOCK_PER_WAY
#define NUM_OF_PAGE_PER_BLOCK PAGE_PER_BLOCK
#define SECTOR_SIZE_BYTE SECTOR_BYTES

const int PAGE_SIZE = DATA_PAGE_SIZE + SPARE_PAGE_SIZE;
const int BLOCK_SIZE = PAGE_SIZE * NUM_OF_PAGE_PER_BLOCK;
const int TOTAL_WAY_SIZE = PAGE_SIZE * NUM_OF_PAGE_PER_BLOCK * NUM_OF_BLOCK_PER_WAY;

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

    //prolly a hotfix
    RNM_Read = 13, //for requests less than a page
    RNM_Program = 14,

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
    unsigned int    bitmap;
}NAND_Cmd_Buffer;

typedef enum eDATA_STATE{

    BUFFER_FREE = 0,
    RNM_WAITING = 1,
    OCCUPIED = 2

}DATA_STATE;

typedef struct sDataBufferEntry{
    
    unsigned char data[DATA_PAGE_SIZE];
    DATA_STATE state;
    unsigned int    DRAM_id;
    unsigned int    bitmap;

}NANDData_t;


typedef enum{

    DONE = 0,
    IN_TRANSACTION = 1

}eIRQState;

typedef struct sIRQReq_t{

    unsigned int id;
    unsigned int addr;
    unsigned int bitmap;
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
    unsigned int bitmap; 

}directNANDReq_t;



#define ADR_NAND_CMD 0x00
#define ADR_NAND_DATA 0x14


#define _ADDR_NUMS_BASE_    0x10000000
#define _ADDR_DEVIF_BASE_   0xC0000000
#define _ADDR_IRQ_BASE_     0x80000000
#define _ADDR_MEMCON_BASE_  0x11000000
#define _ADDR_SUB_BASE_     0xE0000000
#define _ADDR_DRAM_BASE_    0xD0000000

/* BUS CMD MASTER OFFSETS */

#define _CMDMASTER_OFFSET_NANDSIDE_ 0x50


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
#define _OFFSET_IRQ_BITMAP_      0x008
#define _OFFSET_IRQ_DONE_        0x00C
#define _OFFSET_IRQ_MAX_         0x010

#define _OFFSET_DIRECT_         0x020
#define _OFFSET_HOST_ID_        0x024
#define _OFFSET_HOST_ADDR_      0x028
#define _OFFSET_HOST_BITMAP_    0x02C
#define _OFFSET_DIRECT_MAX_     0x030

#define _OFFSET_NAND_COUNT_     0x040
#define _OFFSET_RAM_COUNT_      0x044


#endif
