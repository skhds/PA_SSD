#ifndef _NAND_ADDR_H_
#define _NAND_ADDR_H_

//for now, I'll just copy and paste things

//from NAND_Spec
//

#define ADR_NAND_CMD 0x00
#define ADR_NAND_DATA 0x14


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

#endif
