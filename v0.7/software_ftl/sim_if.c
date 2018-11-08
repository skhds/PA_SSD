#include "sim_if.h"
#include "nand_cmd_layer.h"

DiskReq g_curReq;

void nandCmd(sNandCmd cmd){
#if (_SOFTWARE_SIM_==1)
	swSim_FtlCmd(cmd);
#else
#ifdef _MCSSD_ARM_BARE_METAL_
    sLowNandCmd llnCmd = toLlnCmd(cmd);
    regLlnCmd(llnCmd, cmd.iLogicalAdr);
#endif
#endif
}

void waitNand(uint addr){
#if (_SOFTWARE_SIM_==1)
	swSim_waitNand();
#else
#ifdef _MCSSD_ARM_BARE_METAL_
	waitLln(addr);
#endif
#endif
}

unsigned long long int getReqCnt(){
#if(_SOFTWARE_SIM_==1)
	return swSim_getReqCnt();
#endif
}

DiskReq getCurReq(){
#if(_SOFTWARE_SIM_==1)
	return swSim_getCurReq();
#else
#ifdef _MCSSD_ARM_BARE_METAL_
	return g_curReq;
#endif
#endif
}

void ftlComplete(){

}

sNandSpec getNandSpec(){
#if(_SOFTWARE_SIM_==1)
		return swSim_getNandSpec();
#else
		sNandSpec spec;
	
		spec.iChannelCount = NAND_ARRAY_CHANNEL;
		spec.iWayCount = NAND_ARRAY_WAY;
		
		spec.iPlanePerDev = NAND_PLANE_PER_WAY;
		spec.iBlockPerPlane = NAND_BLOCK_PER_PLANE;
		spec.iPagePerBlock = NAND_PAGE_PER_BLOCK;
		spec.iPageSize = NAND_PAGE_BYTE;
	
		computeSpecBitWidth(&spec);
	
		spec.tPROG = NAND_TIMING_TPROG;
		spec.tR = NAND_TIMING_TR;
		spec.tWC = NAND_TIMING_TWC;
		spec.tRC = NAND_TIMING_TRC;
		spec.tBER = NAND_TIMING_TBER;
	
		return spec;
#endif
}
