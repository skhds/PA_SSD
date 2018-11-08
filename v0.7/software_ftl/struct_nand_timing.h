
/*
*
*		sOpAnalyzer.h
*		written by rab*
*		definition of structures used by OpAnalyzer.
*		plz... arrange these T-T its too dirty......
*
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <queue>

#ifndef __STRUCT_NAND_TIMING_H__
#define __STRUCT_NAND_TIMING_H__

using namespace std;

// NAND timing specification (unit : us) for Rab
//const double tPROG	=	200;
//const double tR		=	25;
//const double tWC	=	0.025;
//const double tRC	=	0.025;
//const double tBER	=	2000;

typedef struct SReq // Req info
{
	double dTime;
	int iLsa, iLength;
	bool bRnw;
	double dQueueTime;
}SReq;

typedef struct SResult
{
	double dTotalTime;
}SResult;

typedef enum SmallOpType
{
	takeBus,
	takeDevice
}SmallOpType;

typedef struct SSmallOp
{
	double			time;
	SmallOpType		type;
}SSmallOp;

typedef struct SLargeOp
{
	int iOp, iCh, iWy, iBk, iPg, iReseverd;
}SLargeOp;

typedef struct SNANDway
{
	double nextReady;
}SNANDwaay;

typedef std::queue<SSmallOp, deque<SSmallOp> > COpQ;
typedef std::queue<SLargeOp, deque<SLargeOp> > sLargeOpQ;
typedef std::queue<int, deque<int> > CQueueInt;

typedef struct SNANDchannel
{
	COpQ				*wayOp;
	SNANDway		*way;
	double		nextBusEnable;
}SNANDchannel;

typedef enum eNandCommand{

	NAND_ERASE = 0,
	NAND_PGM_PAGE,
	NAND_READ_PAGE,
	NAND_CBR_PAGE,
	NAND_CBW_PAGE,

	NAND_CMD_NOTHING = 32

}eNandCmd;

///////////// connection between nand and dftl_ums

#endif
