#include "ftl_meta.h"

sPageMeta *getPgPtr(uint ch, uint wy, uint pl, uint bk, uint pg){
	return &(ftlMeta[ch][wy][pl]->blocks[bk].page[pg]);
}

sBlockMeta *getBkPtr(uint ch, uint wy, uint pl, uint bk){
	return &(ftlMeta[ch][wy][pl]->blocks[bk]);
}
sFtlMeta *getFtlPtr(uint ch, uint wy, uint pl){
	return ftlMeta[ch][wy][pl];
}

void nandProgram(sNandCmd cmd){

	sBlockMeta *blk = getBkPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane, cmd.dest.iBlk);
	sPageMeta *pg = &(blk->page[cmd.dest.iPage]);
/*
#if DEBUG
	assert(cmd.op == NAND_CMD_PROGRAM);	
	assert(isPageFree(pg));
	assert(getWritablePage(blk) <= cmd.dest.iPage);
#else
	ftlAssert(cmd.dest.iPage < nandSpec.iPagePerBlock, "too large page is given to PGM");
	ftlAssert(cmd.dest.iBlk < nandSpec.iBlockPerPlane, "too large block is given to PGM");
	ftlAssert(!isBlockFree(blk), "block cannot be free while programing");
	ftlAssert(cmd.op == NAND_CMD_PROGRAM, "incoming cmd is not NAND_CMD_PROGRAM");	
	ftlAssert(isPageFree(pg), "page is not free while trying program");
	ftlAssert(getWritablePage(blk) <= cmd.dest.iPage, "this block has already written higher page");
#endif
*/		
	/// page meta
	setPageValid(pg);
	setPageLogicalAdr(pg, cmd.iLogicalAdr);
	
	if(cmd.cpValidMap != NULL)
		setValidMap(pg, cmd.cpValidMap);

	/// block meta
	incValidPage(blk);
	setWritablePage(blk, getWritablePage(blk)+1);

}
void nandRead(sNandCmd cmd){

	sBlockMeta *blk = getBkPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane, cmd.dest.iBlk);
	sPageMeta *pg = &(blk->page[cmd.dest.iPage]);

#if DEBUG
	assert(getPageLogicalAdr(pg) == cmd.iLogicalAdr &&
		isPageValid(pg));		
#else
	ftlAssert((getPageLogicalAdr(pg) == cmd.iLogicalAdr &&
		isPageValid(pg)), "wrong page is read");		
#endif
}

void nandCopyback(sNandCmd cmd){

	sBlockMeta *srcBlk = getBkPtr(cmd.src.iChannel, cmd.src.iWay, cmd.src.iPlane, cmd.src.iBlk);
	sPageMeta *srcPg = &(srcBlk->page[cmd.src.iPage]);
	uchar cPrevValidMap[VALID_BITMAP_SIZE];
	sNandCmd subCmd = composeNandCmd(NAND_CMD_READ,
		cmd.src, dummyNandAdr, getPageLogicalAdr(srcPg), NULL);

	getValidMap(srcPg, cPrevValidMap);

	//registerNandCmd(subCmd);
	nandRead(subCmd);

	nandPageInvalidate(cmd.src);
	decValidPage(srcBlk);

	subCmd = composeNandCmd(NAND_CMD_PROGRAM,
		cmd.dest, dummyNandAdr, getPageLogicalAdr(srcPg), cPrevValidMap);

	//registerNandCmd(subCmd);
	nandProgram(subCmd);
}
void nandErase(sNandCmd cmd){
	sBlockMeta *blk = getBkPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane, cmd.dest.iBlk);
	eraseBlock(blk);

	getFtlPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane)->iNumUsedBlk--;
	getFtlPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane)->iNumUsedUserBlk--;
	
}

void nandPageInvalidate(sNandAddr adr){
	sBlockMeta *blk = &(ftlMeta[adr.iChannel][adr.iWay][adr.iPlane]->blocks[adr.iBlk]);
	sPageMeta *pg = &(ftlMeta[adr.iChannel][adr.iWay][adr.iPlane]->blocks[adr.iBlk].page[adr.iPage]);
	
	setPageInvalid(pg);
	decValidPage(blk);
}

