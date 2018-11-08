#ifndef __IF_SPEC_H__
#define __IF_SPEC_H__

#include "NAND_Spec.h"

#define CLOCK_PERIOD        2.5           // 400Mhz
#define IF_BANDWIDTH        (768)  //768MB/s (6Gbps) : SATA 3
#define WIRE_WIDTH          4           //4Byte (32bit)
#define UNIT_OF_REQUEST     512

//interface queue size
#define Q_SIZE          32

//port address
#define REQUEST_ADDR    0x0
#define DATA_ADDR       0x28

#define MEMORY_SIZE     ((DATA_PAGE_SIZE * NUM_OF_PAGE_PER_BLOCK * NUM_OF_BLOCK_PER_WAY) * NUM_OF_WAY * NUM_OF_CHANNEL)
//#define MAX_ADDR        (62272 * 8)
//#define MAX_ADDR        (496128)
//#define MAX_ADDR        (8192)
#define MAX_ADDR        (33554432)

//data comparison
#define COMPARISON  0   //0:OFF, 1:ON 

#endif
