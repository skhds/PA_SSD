#ifndef __LOW_LEVEL_NAND_SPEC_H__
#define __LOW_LEVEL_NAND_SPEC_H__

#define BITWIDTH_NAND_CTRL			32
#define NAND_CTRL_CLK_PERIOD		6
#define NAND_CLK_PERIOD				25

// NAND configuration from PA
#define _LLN_NUM_WAY_				4
#define _LLN_NUM_CHANNEL_			4
//#define _LLN_NUM_CHANNEL_			4
#define _LLN_NAND_BITWIDTH_			8
#define _LLN_BLOCK_PER_WAY_			64
#define _LLN_PAGE_PER_BLOCK_		64
#define _LLN_PAGE_DATE_SIZE_		16384
#define _LLN_PAGE_SPARE_SIZE_		128
#define _LLN_PAGE_SIZE_				(_LLN_PAGE_DATE_SIZE_ + _LLN_PAGE_SPARE_SIZE_)
#define _LLN_TOTAL_WAY_SIZE_		(_LLN_PAGE_SIZE_ * _LLN_PAGE_PER_BLOCK_ * _LLN_BLOCK_PER_WAY_)
#define _LLN_BLOCK_SIZE_			(_LLN_PAGE_SIZE_ * _LLN_PAGE_PER_BLOCK_)

/*
// NAND configuration, check with
#define _LLN_NUM_WAY_								1 
//#define _LLN_NUM_CHANNEL_						1
#define _LLN_NUM_CHANNEL_						4
#define _LLN_NAND_BITWIDTH_					8
#define _LLN_BLOCK_PER_WAY_					4096
#define _LLN_PAGE_PER_BLOCK_				128
#define _LLN_PAGE_DATE_SIZE_				32768
#define _LLN_PAGE_SPARE_SIZE_				128
#define _LLN_PAGE_SIZE_							(_LLN_PAGE_DATE_SIZE_ + _LLN_PAGE_SPARE_SIZE_)
#define _LLN_TOTAL_WAY_SIZE_				(_LLN_PAGE_SIZE_ * _LLN_PAGE_PER_BLOCK_ * _LLN_BLOCK_PER_WAY_)
#define _LLN_BLOCK_SIZE_						(_LLN_PAGE_SIZE_ * _LLN_PAGE_PER_BLOCK_)*/

// low level nand addressing - fit 32-bit
#define _LLN_BYTE_PER_SECTOR_BIT_		9
#define _LLN_SECTOR_PER_PAGE_BIT_		5
#define _LLN_PAGE_PER_BLOCK_BIT_		6
#define _LLN_BLOCK_PER_WAY_BIT_			6
#define _LLN_WAY_PER_CHANNEL_BIT_		2
#define _LLN_CHANNEL_PER_SSD_BIT_		2
//#define _LLN_CHANNEL_PER_SSD_BIT_		2

#define _LLN_BIT_FOR_CHANNEL_				(_LLN_SECTOR_PER_PAGE_BIT_)
#define _LLN_BIT_FOR_WAY_						(_LLN_BIT_FOR_CHANNEL_ + _LLN_CHANNEL_PER_SSD_BIT_)
#define _LLN_BIT_FOR_PAGE_					(_LLN_BIT_FOR_WAY_ + _LLN_WAY_PER_CHANNEL_BIT_)
#define _LLN_BIT_FOR_BLOCK_					(_LLN_BIT_FOR_PAGE_ + _LLN_PAGE_PER_BLOCK_BIT_)

#define _LLN_MASK_CHANNEL_					(((0x1 << _LLN_CHANNEL_PER_SSD_BIT_) - 1) << _LLN_BIT_FOR_CHANNEL_)
#define _LLN_MASK_WAY_							(((0x1 << _LLN_WAY_PER_CHANNEL_BIT_) - 1) << _LLN_BIT_FOR_WAY_)
#define _LLN_MASK_BLOCK_						(((0x1 << _LLN_BLOCK_PER_WAY_BIT_) - 1) << _LLN_BIT_FOR_BLOCK_)
#define _LLN_MASK_PAGE_							(((0x1 << _LLN_PAGE_PER_BLOCK_BIT_) - 1) << _LLN_BIT_FOR_PAGE_)

#endif
