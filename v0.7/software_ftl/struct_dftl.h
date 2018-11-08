#ifndef __STRUCT_DFTL_H__
#define __STRUCT_DFTL_H__

#include "generic_list.h"
#include "typedefs.h"
#include "ftl_spec.h"

//////////// dftl structures

typedef enum GtdElementStatus{
	GTD_ELEMENT_INVALID,
	GTD_ELEMENT_CACHED,
	GTD_ELEMENT_NAND,
	GTD_ELEMENT_STAT_CNT
}eGtdStat;

typedef enum CMTDirtiness{
	CMT_CLEAN,
	CMT_DIRTY
}eCmtDirty;

typedef struct GtdElement{
	eGtdStat	status;
	uint		iAddr;
	//eGtdDirty	dirty;
}sGtdElemnt;

typedef struct CmtElement{
	uint iHeadAddr;
	uint iAddrs[_NUM_ELE_IN_LINE_];
	eCmtDirty dirty;
}sCmtLine;

typedef struct Dftl{
	sGtdElemnt		gtd[GTD_SIZE];
	sListPtr		cmt;
	uint			iNumMapBlock;
	uint			iCurMapBlock;
	sListPtr		mapBlockList;
}sDftl;

typedef struct MapPage{	
	uint addr[_NUM_ELE_IN_PAGE_];
}sMapPage;

//////////// dftl structures

#endif