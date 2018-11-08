#ifndef __HOST_MACHINE_H__
#define __HOST_MACHINE_H__

#include <ctime>
#include <string>
//#include "struct_req.h"
#include "sim_header.h"
#include "nand_timing.h"

class cSsdHostMachine// : public cSimModuleBase
{
protected:

	std::ifstream m_fpWorkload;
	DiskReq m_curReq;

	double m_dSimStartTime;

	typedef struct sSimStat{
		
		double dRdRespTime;
		double dWrRespTime;

		double dRdSecLat;
		double dWrSecLat;

		uint64 iTotReqCnt;		//

		uint64 iTotWrCnt;			//
		uint64 iTotRdCnt;			//
		uint64 iTotWrSec;			//
		uint64 iTotRdSec;			//

		uint64 iMapAccCnt;
		uint64 iMapFetchCnt;
		uint64 iMapConvCnt;

		double dCachedTgCnt;	

	}sSimStat;

	sSimStat m_stat;
	COpAnalyzer *m_opAnalyzer;
	unsigned int iMeasureStartCnt;
	
	std::ofstream m_temp;
	std::string	*m_wlName;


public:
	cSsdHostMachine(const char* workloadName);
	~cSsdHostMachine();

	DiskReq getNextReq();
	void setLatency(double dLat);
	virtual void printStat();
	void initStat();

	inline uint64 getFetchedReqCnt() {return m_stat.iTotReqCnt;}
	inline void setOpAnalyzer(COpAnalyzer *op){ m_opAnalyzer = op; }

	/////////////// added for bgc
protected:
	double m_dLastReqIssue;
	double m_dLastReqHandle;

public:
	inline double getLastReqHandle() {return m_dLastReqHandle;}
	inline double getLastReqIssue() {return m_dLastReqIssue;}
	inline DiskReq getCurReq() { return  m_curReq; }

	inline void mapAccCnt() { m_stat.iMapAccCnt++; }
	inline void mapFetchCnt() { m_stat.iMapFetchCnt++; }

	inline void setCurTgCnt(unsigned int val) { m_stat.dCachedTgCnt += val; }
	
};

#endif
