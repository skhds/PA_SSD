#include "dftl_meta.h"
#include "addr_trans.h"

void nandMapProgram(sNandCmd cmd){

	sBlockMeta *blk = getBkPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane, cmd.dest.iBlk);
	sPageMeta *pg = &(blk->page[cmd.dest.iPage]);
	
	sMapPage *mapData;

    ACQUIRE_LOCK(cmd.dest.iChannel);
	mapData = (sMapPage *)malloc(sizeof(sMapPage));
    RELEASE_LOCK(cmd.dest.iChannel);

#if DEBUG
	assert(cmd.op == NAND_CMD_MAP_PROGRAM);	
#else
	ftlAssert(cmd.op == NAND_CMD_MAP_PROGRAM, "cmd is not NAND_CMD_MAP_PGM");
#endif
	cmd.op = NAND_CMD_PROGRAM;
	nandProgram(cmd);
		
	memcpy(mapData, cmd.dataBuffer, sizeof(sMapPage));
	setPageMapData(pg, mapData);	
}

void nandMapRead(sNandCmd cmd){
	sBlockMeta *blk = getBkPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane, cmd.dest.iBlk);
	sPageMeta *pg = &(blk->page[cmd.dest.iPage]);

#if DEBUG
	assert(pg->mapData != NULL);
	assert(cmd.dataBuffer != NULL);
#else
	//ftlAssert(pg->mapData != NULL, "read mapData cannot be NULL");
	//ftlAssert(pg->pageData != NULL, "read mapData cannot be NULL");
	ftlAssert(cmd.dataBuffer != NULL, "read mapBuffer cannot be NULL");
#endif

	//memcpy(cmd.dataBuffer, pg->mapData, sizeof(sMapPage));
	//memcpy(cmd.dataBuffer, pg->pageData, sizeof(sMapPage));
}

void nandMapCopyback(sNandCmd cmd){

	sBlockMeta *srcBlk = getBkPtr(cmd.src.iChannel, cmd.src.iWay, cmd.src.iPlane, cmd.src.iBlk);
	sPageMeta *srcPg = &(srcBlk->page[cmd.src.iPage]);
	sBlockMeta *desBlk = getBkPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane, cmd.dest.iBlk);
	sPageMeta *desPg = &(desBlk->page[cmd.dest.iPage]);

	cmd.op = NAND_CMD_COPYBACK;
	nandCopyback(cmd);

	setPageMapData(desPg, getPageMapData(srcPg));
	//freePageMapData(srcPg);
}

void nandMapErase(sNandCmd cmd){
	cmd.op = NAND_CMD_ERASE;

	// this is temporarily increase # used user block.... should be modified
	// rabtodo:
	getFtlPtr(cmd.dest.iChannel, cmd.dest.iWay, cmd.dest.iPlane)->iNumUsedUserBlk++;

	nandErase(cmd);
	/*dftlMeta[cmd.dest.iChannel][cmd.dest.iWay][cmd.dest.iPlane].iNumMapBlock--;
	listDeleteEle(dftlMeta[cmd.dest.iChannel][cmd.dest.iWay][cmd.dest.iPlane].mapBlockList, listFindKey(dftlMeta[cmd.dest.iChannel][cmd.dest.iWay][cmd.dest.iPlane].mapBlockList,
		getBk(nandAddrtoInt(cmd.dest))));*/
}

void nandMapInvalidate(sNandAddr adr){
	sPageMeta *pg = &(ftlMeta[adr.iChannel][adr.iWay][adr.iPlane]->blocks[adr.iBlk].page[adr.iPage]);
	nandPageInvalidate(adr);
    ACQUIRE_LOCK(adr.iChannel);
	freePageMapData(pg);
    RELEASE_LOCK(adr.iChannel);
}

//// dftl related

//// page related
void setPageMapData(sPageMeta *page, sMapPage *mapData){
	//page->mapData = mapData;
	//page->pageData = (sPgBuf *)mapData;
}

sMapPage *getPageMapData(sPageMeta *page){
	return (sMapPage *)(page); //wrong one, disabled for reducing meta data
	//return (sMapPage *)(page->pageData);
   
}

void freePageMapData(sPageMeta *page){
#if DEBUG
	assert(page->mapData != NULL);
#else
	//ftlAssert(page->mapData != NULL, "freed map data should not be NULL");
	//ftlAssert(page->pageData != NULL, "freed map data should not be NULL");
#endif
	//free((sMapPage *)(page->pageData));
}

///// block related
void setMapBlock(sBlockMeta *blk){
	blk->status = NAND_BLOCK_MAP_USED;
}

uchar isMapBlock(sBlockMeta *blk){
	return (blk->status == NAND_BLOCK_MAP_USED);
}
