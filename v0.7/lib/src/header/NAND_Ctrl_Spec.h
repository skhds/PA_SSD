#ifndef _NAND_SPEC_H_
#define _NAND_SPEC_H_

#define BWC

// NAND timinig, in nanoseconds

#include "NAND_Struct.h"
#include "header/global_flag.h"

#define ADR_NAND_CMD 0x00
#define ADR_NAND_DATA 0x14

#define BITWIDTH_NAND_CTRL		32
#define NAND_CTRL_CLK_PERIOD 5
#define NAND_CLK_PERIOD   5


/*
typedef enum NAND_TIMING
{
    //T_PROG = 800000,
    T_PROG = 300000,
    T_R = 25000,
    T_BER = 2000000
}NAND_TIMING;
*/
typedef enum NAND_TIMING
{
    //T_PROG = 800000,
    T_PROG = 660000,
    T_R = 45000,
    T_BER = 3500000
}NAND_TIMING;



unsigned int iFileNumber;

// NAND configuration
const int NUM_OF_WAY = 4; 
const int NUM_OF_CHANNEL = 4;
//const int NUM_OF_CHANNEL = 1;
const int NAND_I_O_BITWIDTH = 8;
const int NUM_OF_BLOCK_PER_WAY = 64;
const int NUM_OF_PAGE_PER_BLOCK = 64;
const int DATA_PAGE_SIZE = 16384;
const int SPARE_PAGE_SIZE = 128;
const int PAGE_SIZE = (DATA_PAGE_SIZE + SPARE_PAGE_SIZE);
const int TOTAL_WAY_SIZE = (PAGE_SIZE * NUM_OF_PAGE_PER_BLOCK * NUM_OF_BLOCK_PER_WAY);
const int BLOCK_SIZE = (PAGE_SIZE * NUM_OF_PAGE_PER_BLOCK);

// Maximum number of bits this SSD can handle for 32-bit address
const int BYTE_PER_SECTOR_BIT = 9;
const int SECTOR_PER_PAGE_BIT = 5;
const int PAGE_PER_BLOCK_BIT = 6;
const int BLOCK_PER_WAY_BIT = 6;
const int WAY_PER_CHANNEL_BIT = 2;
//const int CHANNEL_PER_SSD_BIT = 3;
const int CHANNEL_PER_SSD_BIT = 2;

#ifndef BWC
// Bit offset for each attribute (Channel at MSB)
const int BIT_FOR_PAGE = SECTOR_PER_PAGE_BIT;
const int BIT_FOR_BLOCK = BIT_FOR_PAGE + PAGE_PER_BLOCK_BIT;
const int BIT_FOR_WAY = BIT_FOR_BLOCK + BLOCK_PER_WAY_BIT;
const int BIT_FOR_CHANNEL = BIT_FOR_WAY + WAY_PER_CHANNEL_BIT;
#else
// Bit offset for each attribute (Channel at LSB)
const int BIT_FOR_CHANNEL = SECTOR_PER_PAGE_BIT;       //5
const int BIT_FOR_WAY = BIT_FOR_CHANNEL + CHANNEL_PER_SSD_BIT;  //7
const int BIT_FOR_PAGE = BIT_FOR_WAY + WAY_PER_CHANNEL_BIT; //9
const int BIT_FOR_BLOCK = BIT_FOR_PAGE + PAGE_PER_BLOCK_BIT; //15
#endif

// address maskings
const int MASK_CHANNEL = ((0x1 << CHANNEL_PER_SSD_BIT) - 1) << BIT_FOR_CHANNEL;
const int MASK_WAY = ((0x1 << WAY_PER_CHANNEL_BIT) - 1) << BIT_FOR_WAY;
const int MASK_BLOCK = ((0x1 << BLOCK_PER_WAY_BIT) - 1) << BIT_FOR_BLOCK;
const int MASK_PAGE = ((0x1 << PAGE_PER_BLOCK_BIT) - 1) << BIT_FOR_PAGE;

#endif
