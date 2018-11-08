#ifndef __STRUCT_META_H__
#define __STRUCT_META_H__

//#include "struct_nand_spec.h"
#include "ftl_spec.h"
#include "struct_dftl.h"

typedef enum eSectorState
{
	SECTOR_INVALID = 0,
	SECTOR_VALID = 1
}eSectorStat;

typedef enum ePageStatus
{
	// 2-bit,
	// LSB : free bit (1=free, 0=used)
	// MSB : valid bit (1=valid, 0=invalid)
	NAND_PAGE_INVALID		= 0,
	NAND_PAGE_FREE			= 1,
	NAND_PAGE_VALID			= 2,
	NAND_PAGE_NOTHING		= 3
}ePageStat;

typedef enum eBlockStatus
{
	// 2-bit
	// LSB - free (1) - used (0)
	// MSB - dead (1)
	NAND_BLOCK_USED = 0,
	NAND_BLOCK_FREE = 1,
	NAND_BLOCK_DEAD = 2,
	NAND_BLOCK_NOTHING = 3,
	NAND_BLOCK_MAP_USED
}eBlockStat;

typedef struct sDramInfo
{
    uint    dram_addr;
    uint    dram_length;
    uint    id;
}sDramInfo;
#pragma pack(push)
#pragma pack(1)
typedef struct sPageMeta
{
	ePageStat			status;
	uchar			    cpValidMap[VALID_BITMAP_SIZE];	
	uint				iLogicalPgAddr;
	//sPgBuf				*pageData;
	//sMapPage			*mapData;
}sPageMeta;
#pragma pack(pop)


typedef struct sBlockMeta
{
	eBlockStat		status;
	sPageMeta			page[NAND_PAGE_PER_BLOCK];
	uint					iValidPageCnt;
	uint					iTopWritablePage;
	uint					iEraseCnt;
	//uint					iWrType;
}sBlockMeta;

typedef struct sMapTable
{
	uint				iTransAddr;
}sMapTable;

typedef struct sFtlMetaData
{	
    sBlockMeta		blocks[NAND_BLOCK_PER_PLANE];
	uint			iCurBlk;
	uint			iCurPg;
	uint			iNumUsedBlk;
	uint			iNumUsedUserBlk;

	sMapTable		l2pTable[AVAIL_PG_CNT];
		
}sFtlMeta;

// sector state
void setSectorValid(sPageMeta *page, uint iOffset);
void setSectorInvalid(sPageMeta *page, uint iOffset);
uchar isSectorValid(sPageMeta *page, uint iOffset);
uchar *getValidMapPtr(sPageMeta *page);
void getValidMap(sPageMeta *page, uchar *validMap);
void setValidMap(sPageMeta *page, uchar *validMap);

// page state
void setPageValid(sPageMeta *page);
void setPageInvalid(sPageMeta *page);
void setPageFree(sPageMeta *page);
uchar isPageValid(sPageMeta *page);
uchar isPageFree(sPageMeta *page);

void setPageLogicalAdr(sPageMeta *page, uint iLogicalAdr);
uint getPageLogicalAdr(sPageMeta *page);



// block state
void setBlockFree(sBlockMeta *blk);
void setBlockUsed(sBlockMeta *blk);
void setBlockDead(sBlockMeta *blk);
uchar isBlockFree(sBlockMeta *blk);
uchar isBlockDead(sBlockMeta *blk);



void incValidPage(sBlockMeta *blk);
void decValidPage(sBlockMeta *blk);
uint getValidPageCnt(sBlockMeta *blk);

void setWritablePage(sBlockMeta *blk, uint iOffset);
uint getWritablePage(sBlockMeta *blk);

void eraseBlock(sBlockMeta *blk);
void setEraseCnt(sBlockMeta *blk, uint iVal);
void incEraseCnt(sBlockMeta *blk);
uint getEraseCnt(sBlockMeta *blk);

uchar isBlockFull(sBlockMeta *blk);




#endif
