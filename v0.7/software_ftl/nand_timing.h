/*
*
*		OpAnalyzer.h
*		generated by Gun, written by rab*
*		class definition of COpAnalyzer
*
*/

#include <list>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "struct_nand_timing.h"

extern "C"{
#include "sim_if.h"
};


//#include "time.h"
//#include "struct_nand_spec.h"
//#include "nand_spec_struct.h"
//#include "ftl_struct.h"
//#include "struct_req.h"

#ifndef __NAND_TIMING_H__
#define __NAND_TIMING_H__

//#define __OPANALYZER_DEBUG__


class COpAnalyzer
{
protected:
	SResult			Result;	
	SNANDchannel	*NANDs;
	uint ***iErCnt;
	//CQueueInt		*essenOp;

	unsigned int iNumCmdCnt[NAND_CMD_TYPE_CNT];

	sNandSpec	spec;
	double tPROG;
	double tR;
	double tWC;
	double tRC;
	double tBER;

	unsigned int CHANNEL;
	unsigned int WAY;
	unsigned int BLOCK;
	unsigned int SECTOR;


	CQueueInt		*m_IncomeOpList;
	sLargeOpQ		*m_LargeOpList;

	//// interfacing with dftl_ums_sw

	void				OpSeparate(SLargeOp op);
	unsigned int	cmdDecode(eNandOp op);
	void				addrDecode(SLargeOp &largeOp, sNandCmd cmd);
	void				srcAddrDecode(SLargeOp &largeOp, sNandCmd cmd);
	
	//sNandSpec		m_nandSpec;

	
	
public:

	
	COpAnalyzer(sNandSpec spec);
	COpAnalyzer();
	~COpAnalyzer();

	void initTimings();

	inline sNandSpec getNandSpec(){ return spec; }

	inline SResult		GetResult()	const { return Result; } // by Gun

	
	double				OpComputeCHTime(int iCh);
	void				SetZeroTime();

	
	//void				OpDecode(int iOp, SLargeOp *op);
	
	void				OpCOpQPush(COpQ *smallQ, double time, SmallOpType type);
	bool				OpAllWayEmpty(SNANDchannel Ch);
	SSmallOp			OpGetSmallOpNPop(COpQ *wayOp);

	inline void			increaseErCnt(int iCh, int iWy, int iBk);

	//CQueueInt			GetEssentialOp();
	//double			Analyze(SReq& Req, CQueueInt OpList); // by rab
	//void				OpSeparate(int Op);

	//// interfacing with dftl_ums_sw

	//unsigned long int	computeTime(SCommand ftlCmd);	
	unsigned long long int computeTime(std::list<sNandCmd> ftlCmd);

	inline void printCmdCnt(){
		std::cout << "Given NAND Cmds: ";
		for(int i=0; i < NAND_CMD_TYPE_CNT ; i++){
			std::cout << iNumCmdCnt[i] << "\t";
		}
		std::cout<<std::endl;
	}

	inline void fileCmdCnt(std::ofstream &fp){
		for (int i = 0; i < NAND_CMD_TYPE_CNT; i++){
			fp << iNumCmdCnt[i] << "\t";
		}		
	}
};

#endif

// gun test
