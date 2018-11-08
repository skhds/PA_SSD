#ifndef __SFR_ADDR_H__
#define __SFR_ADDR_H__

#include "compile_flag.h"

typedef volatile unsigned int vuint;
typedef volatile unsigned char vuchar;
typedef volatile double	vdouble;

#if (_SOFTWARE_SIM_ == 0)
#ifdef _MCSSD_ARM_BARE_METAL_

///////// ROM 
#define _ADDR_ROM_BASE_							(0x00000000)

///////// RAM for map tables
#define _ADDR_RAM_BASE_							(0x2000000)

#define _ADDR_SUB_REQ_BASE_					(0xe0000000)
#define _OFFSET_SUB_REQ_COMPLETE_           (0x000)
#define _OFFSET_ISTARTADDR_ADDR_            (0x004)
#define _OFFSET_ILEN_ADDR_                  (0x008)
#define _OFFSET_OP_ADDR_                    (0x00c)
#define _OFFSET_CMD_SIZE_                   (0x014)
#define _OFFSET_SUB_REQ_CACHE_DATA_	        (0x01C)

#define _OFFSET_CPU2_                       (0x100)
#define _OFFSET_CPU2_OP_                    (0x000)
#define _OFFSET_CPU2_ADDR_                  (0x004)
#define _OFFSET_CPU2_LENGTH_                (0x008)
#define _OFFSET_CPU2_DRAM_ADDR_             (0x00C)
#define _OFFSET_CPU2_DRAM_LENGTH_           (0x010)
#define _OFFSET_CPU2_ID_                    (0x014)

#define _ADDR_NAND_MAN_BASE_				(0xf0000000)
#define _ADDR_CPU2_BASE_                    (0x000)
#define _OFFSET_IRQ_BASE_                   (0x080)
#define _OFFSET_IRQ_ID_                     (0x000)
#define _OFFSET_IRQ_ADDR_                   (0x004)
#define _OFFSET_IRQ_DONE_                   (0x008)

#define _OFFSET_R_BUS_						(0x000)
#define _OFFSET_R_BUS_ID_					(0x000)
#define _OFFSET_R_BUS_OP_					(0x004)
#define _OFFSET_R_BUS_ADDR_					(0x008)
#define _OFFSET_R_BUS_LEN_					(0x00C)
#define _OFFSET_R_BUS_DUMMY1_				(0x010)
#define _OFFSET_R_BUS_DUMMY2_				(0x014)
#define _OFFSET_R_BUS_DUMMY3_				(0x018)
#define _OFFSET_R_BUS_DUMMY4_				(0x01C)

#define _OFFSET_R_BUS_NAND_CMD_				(0x020)
#define _OFFSET_R_BUS_NAND_ADDR1_			(0x024)
#define _OFFSET_R_BUS_NAND_ADDR2_			(0x028)
#define _OFFSET_R_BUS_NAND_ADDR3_			(0x02C)
#define _OFFSET_R_BUS_NAND_ADDR4_			(0x030)

#define _OFFSET_R_BUS_NAND_MAP_				(0x034)
#define _OFFSET_R_BUS_NAND_MAP_ADDR_		(0x038)

#define _OFFSET_R_BUS_DRAM_ADDR_			(0x03C)
#define _OFFSET_R_BUS_DRAM_LENGTH_			(0x040)
#define _OFFSET_R_BUS_REQ_ID_		    	(0x044)
#define _OFFSET_FTL_COMPLETE_               (0x048)
#define _OFFSET_R_BUS_NAND_CMD_STATUS_		(0x420)
#define _OFFSEt_R_BUS_INT_					(0x424)							

#define _OFFSET_R_BUS_NAND_FULL             (0x100)

///////// Memory allocation controller
#define _ADDR_MEMCTRL_BASE_                 (0x11000000)
#define _OFFSET_REQ_SOURCE_                 (0x0)
#define _OFFSET_GRANT_                      (0x4)
#define _OFFSET_RELEASE_                    (0x8)

///////// Interrupt controller
#define _ADDR_INTCTRL_BASE_					(0x80000000)
#define _OFFSET_INT_TYPE_					(0x4)
#define _OFFSET_INT_SOURCE_ 				(0x8)
#define _OFFSET_DEVICEIF_COMPLETE_          (0xc)
#define _OFFSET_CACHE_BUFFER_COMPLETE_      (0x10)
#define _OFFSET_FTL_0_COMPLETE_               (0x14)
#define _OFFSET_FTL_1_COMPLETE_               (0x18)
#define _OFFSET_FTL_2_COMPLETE_               (0x1c)
#define _OFFSET_FTL_3_COMPLETE_               (0x20)
#define _OFFSET_INITIAL_COMPLETE_           (0x24)  

//////// Device interface
#define _ADDR_DEV_IF_BASE_					(0xc0000000)
#define _INTERVAL_R_DATA_					(0x4)
#define _OFFSET_R_SIZE_						(0X000)
#define _OFFSET_R_COMPLETE_					(0X4)
#define _OFFSET_R_DATA_						(0X8)

//////// Cache buffer controller
#define _ADDR_CACHE_BUFFER_BASE_		(0xd0000000)
#define _OFFSET_CACHE_COMPLETE_			(0x0)
#define _OFFSET_CACHE_ADDR_				(0x4)
#define _OFFSET_CACHE_LEN_				(0x8)
#define _OFFSET_CACHE_OP_				(0xc)
#define _OFFSET_CACHE_SIZE_				(0x10)
#define _OFFSET_CACHE_DATA_				(0x14)
#define _OFFSET_CACHE_QUEUE_SIZE_		(0x18)
#define _INTERVAL_CACHE_DATA_ 			(0x4)

#endif
#endif

#endif

