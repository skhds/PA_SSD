////////////////////////////////////////////////
/*

Using this file, define APIs for FTL which can be
easily ported for various NAND/SSD simulators.
(e.g. software simulator, SystemC-based simulator...)

*/
////////////////////////////////////////////////

#ifndef __SIM_IF_H__
#define __SIM_IF_H__

#include <stdio.h>
#include <assert.h>

#include "compile_flag.h"
#include "nand_spec.h"
#include "struct_sim_if.h"

//// Simulator-related functions, externs, and definitions should be here!

// software simulation
extern void swSim_FtlCmd(sNandCmd cmd);
extern void swSim_waitNand();
extern sNandSpec swSim_getNandSpec();
extern unsigned long long int swSim_getReqCnt();
extern DiskReq swSim_getCurReq();
extern void swSim_incMapAccCnt();
extern void swSim_incMapFetchCnt();
/////////////////


/////
/***************** APIs **********************/

////// Handle a nand command to the simulator
void nandCmd(sNandCmd cmd);

////// Wait the simulator to handle all the commands given before
void waitNand(uint addr);

////// signal to simulator to indicate ftl computation is finished
void ftlComplete();

////// get nand specification from simulator to configure ftl
sNandSpec getNandSpec();

unsigned long long int getReqCnt();

DiskReq getCurReq();

/*********************************************/
#endif
