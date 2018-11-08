#include "struct_meta.h"

// sector state
void setSectorValid(sPageMeta *page, uint iOffset){
	page->cpValidMap[iOffset/8] |= ((SECTOR_VALID << 7) >> (iOffset%8));
}

void setSectorInvalid(sPageMeta *page, uint iOffset){
	page->cpValidMap[iOffset/8] &= (0xff ^ ((SECTOR_VALID << 7) >> (iOffset%8)));
}

uchar isSectorValid(sPageMeta *page, uint iOffset){
	return ((page->cpValidMap[iOffset/8] & ((SECTOR_VALID << 7) >> (iOffset%8))) != 0);
}

uchar *getValidMapPtr(sPageMeta *page){
	return page->cpValidMap;
}

void getValidMap(sPageMeta *page, uchar *validMap){
	uint i=0;
	for(i=0; i < VALID_BITMAP_SIZE ; i++)
		validMap[i] = page->cpValidMap[i];
}

void setValidMap(sPageMeta *page, uchar *validMap){
	uint i=0;
	for(i=0; i < VALID_BITMAP_SIZE ; i++)
		page->cpValidMap[i] = validMap[i];		
}

// page state
void setPageValid(sPageMeta *page){
	page->status = NAND_PAGE_VALID;
}

void setPageInvalid(sPageMeta *page){
	page->status = NAND_PAGE_INVALID;
}

void setPageFree(sPageMeta *page){
	uchar cEmptyValidMap[VALID_BITMAP_SIZE] = {0};
	setValidMap(page, cEmptyValidMap);
	
	page->status = NAND_PAGE_FREE;
	page->iLogicalPgAddr = INVALID_VALUE;	
}

uchar isPageValid(sPageMeta *page){
	return ((page->status & NAND_PAGE_VALID) != 0);
}

uchar isPageFree(sPageMeta *page){
	return ((page->status & NAND_PAGE_FREE) != 0);
}

void setPageLogicalAdr(sPageMeta *page, uint iLogicalAdr){
	page->iLogicalPgAddr = iLogicalAdr;
}

uint getPageLogicalAdr(sPageMeta *page){
	return page->iLogicalPgAddr;
}

// block state
void setBlockFree(sBlockMeta *blk){
	blk->status = NAND_BLOCK_FREE;
	blk->iTopWritablePage = 0;
	blk->iValidPageCnt = 0;
}
void setBlockUsed(sBlockMeta *blk){
	blk->status = NAND_BLOCK_USED;
}

void setBlockDead(sBlockMeta *blk){
	blk->status = NAND_BLOCK_DEAD;
}

uchar isBlockFree(sBlockMeta *blk){
	return ((blk->status == NAND_BLOCK_FREE));
}

uchar isBlockDead(sBlockMeta *blk){
	return ((blk->status == NAND_BLOCK_DEAD));
}

void incValidPage(sBlockMeta *blk){
	blk->iValidPageCnt++;
}

void decValidPage(sBlockMeta *blk){
	blk->iValidPageCnt--;
}

uint getValidPageCnt(sBlockMeta *blk){
	return blk->iValidPageCnt;
}

void setWritablePage(sBlockMeta *blk, uint iOffset){
	blk->iTopWritablePage = iOffset;
}

uint getWritablePage(sBlockMeta *blk){
	return blk->iTopWritablePage;
}

void eraseBlock(sBlockMeta *blk){
	uint i;
	for(i=0 ; i < NAND_PAGE_PER_BLOCK ; i++)
		setPageFree(&(blk->page[i]));

	incEraseCnt(blk);
	setBlockFree(blk);
}

void setEraseCnt(sBlockMeta *blk, uint iVal){
	blk->iEraseCnt = iVal;
}

void incEraseCnt(sBlockMeta *blk){
	blk->iEraseCnt++;
}

uint getEraseCnt(sBlockMeta *blk){
	return blk->iEraseCnt;
}

uchar isBlockFull(sBlockMeta *blk){
	return (getWritablePage(blk) >= NAND_PAGE_PER_BLOCK);
}



