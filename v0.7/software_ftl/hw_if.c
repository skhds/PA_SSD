#include "compile_flag.h"
#include "hw_if.h"

void registerNandCmd(sNandCmd cmd)
{
	uchar isSyncRequired = UCHAR_FALSE;

	nandCmd(cmd);

	switch(cmd.op)
	{
	case NAND_CMD_PROGRAM:		
		nandProgram(cmd);
		break;
	case NAND_CMD_READ:
		nandRead(cmd);
		break;
	case NAND_CMD_COPYBACK:
		nandCopyback(cmd);
		break;
	case NAND_CMD_ERASE:
		nandErase(cmd);
		break;
	case NAND_CMD_RNM_READ:
		nandRead(cmd);
		break;
	case NAND_CMD_MAP_READ:
		nandMapRead(cmd);
		break;
	case NAND_CMD_MAP_PROGRAM:
		nandMapProgram(cmd);
		break;
	case NAND_CMD_MAP_COPYBACK:
		nandMapCopyback(cmd);
		break;
	case NAND_CMD_MAP_ERASE:
		nandMapErase(cmd);
		break;
	}

#ifdef _MCSSD_ARM_BARE_METAL_
    activateNand(UCHAR_FALSE, cmd.iLogicalAdr);
#endif

	if(cmd.op == NAND_CMD_RNM_READ ||
		cmd.op == NAND_CMD_MAP_READ	||
		cmd.op == NAND_CMD_MAP_PROGRAM ||
		cmd.op == NAND_CMD_MAP_COPYBACK ||
        cmd.op == NAND_CMD_MAP_ERASE
		){
			isSyncRequired = UCHAR_TRUE;
	}		

	if(isSyncRequired){
		//waitNand(cmd.iLogicalAdr);
	}
    /*
	nandCmd(cmd);
    activateNand(UCHAR_FALSE, cmd.iLogicalAdr);
    */
}

//sNandSpec getNandSpec()
//{
//	sNandSpec spec;
//
//	spec.iChannelCount = NAND_ARRAY_CHANNEL;
//	spec.iWayCount = NAND_ARRAY_WAY;
//	
//	spec.iPlanePerDev = NAND_PLANE_PER_WAY;
//	spec.iBlockPerPlane = NAND_BLOCK_PER_PLANE;
//	spec.iPagePerBlock = NAND_PAGE_PER_BLOCK;
//	spec.iPageSize = NAND_PAGE_BYTE;
//
//	computeSpecBitWidth(&spec);
//
//	spec.tPROG = NAND_TIMING_TPROG;
//	spec.tR = NAND_TIMING_TR;
//	spec.tWC = NAND_TIMING_TWC;
//	spec.tRC = NAND_TIMING_TRC;
//	spec.tBER = NAND_TIMING_TBER;
//
//	return spec;
//}
