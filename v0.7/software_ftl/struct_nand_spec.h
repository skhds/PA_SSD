#ifndef __STRUCT_NAND_SPEC_H__
#define __STRUCT_NAND_SPEC_H__

#include "typedefs.h"

typedef struct sNandSpec
{
	// NAND array
	uint iChannelCount;
	uint iWayCount;

	// size parameter
	uint iPageSize;
	uint iPagePerBlock;
	uint iBlockPerPlane;
	uint iPlanePerDev;

    uint64  iTotalByte;
		
	// bit counts
	uint iChannel_Bitwidth;
	uint iWay_Bitwidth;
	uint iPageSize_Bitwidth;
	uint iPagePerBlock_Bitwidth;
	uint iBlockPerPlane_Bitwidth;
	uint iPlanePerDev_Bitwidth;

	double tPROG;
	double tR;
	double tWC;
	double tRC;
	double tBER;

}sNandSpec;


#endif
