#ifndef __FTL_SPEC_H__
#define __FTL_SPEC_H__

#include "sim_if.h"
#include "ftl_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// dftl specs

///////////////////////////////
////// configuring cmt
///////////////////////////////


#define _SINGLE_ELE_BYTE_				(4)
#if (__ADR_TRANS_SCHEME__==_BASELINE_PAGE_MAPPING_)
#define _NUM_ELE_IN_LINE_				(1)
#define _NUM_CMT_IN_CACHE_				2048
#elif (__ADR_TRANS_SCHEME__==_DFTL_)
#define _NUM_ELE_IN_LINE_				(1)
#define _NUM_CMT_IN_CACHE_				(8192)//2048//// 8192 is 32KB!!!!
#elif (__ADR_TRANS_SCHEME__==_TWO_LEVEL_)
#define _NUM_ELE_IN_LINE_				(((NAND_PAGE_BYTE) / (_SINGLE_ELE_BYTE_)))
#define _NUM_CMT_IN_CACHE_				(2)
#elif (__ADR_TRANS_SCHEME__==_DIST_FTL_)
#define _NUM_ELE_IN_LINE_				(((NAND_PAGE_BYTE) / (_SINGLE_ELE_BYTE_)))
#define _NUM_CMT_IN_CACHE_				(2)
#endif
#define _CMT_LINE_SIZE_					((_SINGLE_ELE_BYTE_) * (_NUM_ELE_IN_LINE_))
#define _CMT_SIZE_						(_CMT_LINE_SIZE_ * _NUM_CMT_IN_CACHE_)

///////////////////////////////
//////////// configuring GTD
///////////////////////////////

#define _MAP_NAND_PAGE_SIZE_		(NAND_PAGE_BYTE)
#define _NUM_ELE_IN_PAGE_				((_MAP_NAND_PAGE_SIZE_)/(_SINGLE_ELE_BYTE_))
#define _NAND_PAGE_CNT_					(NAND_ARRAY_CHANNEL * NAND_ARRAY_WAY * NAND_PLANE_PER_WAY * NAND_BLOCK_PER_PLANE * NAND_PAGE_PER_BLOCK)
#define _NUM_GTD_ELEMENT_				((_NAND_PAGE_CNT_ / _NUM_ELE_IN_PAGE_) + 1)
#define GTD_SIZE								(_NUM_GTD_ELEMENT_)

///////////////////////////////
//////////// configuring Map blocks
///////////////////////////////

#define _MIN_MAP_BLOCK_					((_NUM_GTD_ELEMENT_ / NAND_PAGE_PER_BLOCK) + 1)
#define _MAP_BLOCK_OV_PROV_			(1)
#define _MAX_MAP_BLOCK_					(_MIN_MAP_BLOCK_ + _MAP_BLOCK_OV_PROV_)
#define _MAP_BLOCK_PER_PL_			(((_MAX_MAP_BLOCK_) / (NAND_ARRAY_CHANNEL * NAND_ARRAY_WAY * NAND_PLANE_PER_WAY)) + 1)

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// ftl specs
/////////////////////////////////////////////////////////////////////////////////////////////////

#define		SECTOR_SIZE_BYTE			(512)
#define		SECTOR_PER_PAGE				(NAND_PAGE_BYTE / SECTOR_SIZE_BYTE)
#define		VALID_BITMAP_SIZE			(SECTOR_PER_PAGE/8+1)

#define		RSVED_BLK_RATIO				(0.05)
#define		INV_RSVED_BLK_RATIO			(20)

#define		RSVED_BLK_CNT					(NAND_BLOCK_PER_PLANE/INV_RSVED_BLK_RATIO)

#if			(__ADR_TRANS_SCHEME__==_BASELINE_PAGE_MAPPING_)
#define		AVAIL_BLK_CNT					(NAND_BLOCK_PER_PLANE - RSVED_BLK_CNT)
#define		DATA_BLK_CNT					(NAND_BLOCK_PER_PLANE)
#elif		(__ADR_TRANS_SCHEME__==_DFTL_)
#define		AVAIL_BLK_CNT					(NAND_BLOCK_PER_PLANE - RSVED_BLK_CNT - _MAP_BLOCK_PER_PL_)
#define		DATA_BLK_CNT					(NAND_BLOCK_PER_PLANE - _MAP_BLOCK_PER_PL_)
#elif		(__ADR_TRANS_SCHEME__==_DIST_FTL_)
#define		AVAIL_BLK_CNT					(NAND_BLOCK_PER_PLANE - RSVED_BLK_CNT - _MAP_BLOCK_PER_PL_)
#define		DATA_BLK_CNT					(NAND_BLOCK_PER_PLANE - _MAP_BLOCK_PER_PL_)
#elif		(__ADR_TRANS_SCHEME__==_TWO_LEVEL_)
#define		AVAIL_BLK_CNT					(NAND_BLOCK_PER_PLANE - RSVED_BLK_CNT - _MAP_BLOCK_PER_PL_)
#define		DATA_BLK_CNT					(NAND_BLOCK_PER_PLANE - _MAP_BLOCK_PER_PL_)
#else
#endif

#define		AVAIL_PG_CNT					((AVAIL_BLK_CNT * NAND_PAGE_PER_BLOCK))

// address bit paritioning from the LSB
#define		ADDR_BIT_PARTITION		{SECTOR_ADDR, CHANNEL_ADDR, WAY_ADDR, PLANE_ADDR, PAGE_ADDR, BLOCK_ADDR}


#endif
