#include "host_machine.h"
#include "ftl_spec.h"
#include "dist_ftl_spec.h"
#include <string>

using namespace std;

#define _MILISEC_ 1000000

cSsdHostMachine::cSsdHostMachine(const char* workloadName)
	//: cSimModuleBase(MODULE_HOST_MACHINE)
{
	m_fpWorkload.open(workloadName, ios::in);
	m_wlName = new string(workloadName);

	assert(m_fpWorkload.is_open());

	memset(&m_stat, 0x00, sizeof(sSimStat));

	m_temp.open("result.txt", ios::app);

	time_t timeValue;
	time(&timeValue);

	//m_temp << timeValue << "\t";

	//m_dSimStartTime = time()

	//////
	m_dLastReqIssue = 0;
	m_dLastReqHandle = 0;
	iMeasureStartCnt = 0;
}

cSsdHostMachine::~cSsdHostMachine(){
	this->m_fpWorkload.close();
	printStat();
	delete m_wlName;
}

void 
cSsdHostMachine::initStat(){
	uint64 prevTotReqCnt = m_stat.iTotReqCnt;
	memset(&m_stat, 0x00, sizeof(sSimStat));
	m_stat.iTotReqCnt = prevTotReqCnt;
	iMeasureStartCnt = prevTotReqCnt;
}

DiskReq
cSsdHostMachine::getNextReq(){

	DiskReq newReq;
	double dTime;
	int timeInt;
	int timeFloatTmp;
	double timeFloat;
	int iCmd;
	std::string lineBuf;
	std::string strTime, strLba, strLen, strCmd;
	std::string strInt, strFloat;

	//m_fpWorkload >> dTime >> newReq.sectorAddr >> newReq.sectorCount >> iCmd >> endl;


	getline(m_fpWorkload, lineBuf);

	strTime = lineBuf.substr(0, lineBuf.find('\t'));
	lineBuf = lineBuf.substr(lineBuf.find('\t')+1);
	
	strLba = lineBuf.substr(0, lineBuf.find('\t'));
	lineBuf = lineBuf.substr(lineBuf.find('\t') + 1);

	strLen = lineBuf.substr(0, lineBuf.find('\t'));
	lineBuf = lineBuf.substr(lineBuf.find('\t') + 1);

	strCmd = lineBuf;



	if (strTime.find('.') != std::string::npos){
		strInt = strTime.substr(0, strTime.find('.'));
		strFloat = strTime.substr(strTime.find('.') + 1);
		timeInt = atoi(strInt.c_str());
		timeFloatTmp = atoi(strFloat.c_str());
		timeFloat = timeFloatTmp;
		for (int k = 0; k < strFloat.size(); k++)
			timeFloat /= 10;

		dTime = timeInt + timeFloat;
	}
	else{
		timeInt = atoi(strInt.c_str());
		dTime = timeInt;
	}

	newReq.sectorAddr = atoi(strLba.c_str());
	newReq.sectorCount = atoi(strLen.c_str());
	iCmd = atoi(strCmd.c_str());

	m_dLastReqIssue += (dTime);

	if(m_dLastReqIssue < m_dLastReqHandle){
		m_dLastReqIssue = m_dLastReqHandle;
	}


	newReq.dIssueTime = m_dLastReqIssue;

	if(m_fpWorkload.eof()){
		m_fpWorkload.close();
		printStat();
		cout << " ###### Simulation ends" << endl;
		exit(1);
	}

	// @140826 0 is now write!!
	//newReq.cmd = iCmd == 1 ? DISK_CMD_WRITE : DISK_CMD_READ;
	newReq.cmd = iCmd == 0 ? DISK_CMD_WRITE : DISK_CMD_READ;

	this->m_stat.iTotReqCnt++;

	if(newReq.cmd == DISK_CMD_WRITE){

		this->m_stat.iTotWrCnt++;
		this->m_stat.iTotWrSec += newReq.sectorCount;

	}else if(newReq.cmd == DISK_CMD_READ){

		this->m_stat.iTotRdCnt++;
		this->m_stat.iTotRdSec += newReq.sectorCount;

	}else{
		assert(0);
	}

	if ((this->m_stat.iTotReqCnt % 10000) == 0)
		cout << "### " << m_stat.iTotReqCnt << "th request is fetched";// << endl;

	m_curReq = newReq;
	return newReq;
}

void
cSsdHostMachine::setLatency(double dLat){

	if(m_curReq.cmd == DISK_CMD_WRITE){

		m_stat.dWrRespTime += dLat;
		m_stat.dWrSecLat += dLat / m_curReq.sectorCount;

	}
	else if(m_curReq.cmd == DISK_CMD_READ){

		m_stat.dRdRespTime += dLat;
		m_stat.dRdSecLat += dLat / m_curReq.sectorCount;
	}

	if((this->m_stat.iTotReqCnt % 10000) == 0)
	{
		cout << " Lat per Sec (R/W): " << m_stat.dRdRespTime / m_stat.iTotRdSec
			<< "/" << m_stat.dWrRespTime / m_stat.iTotWrSec << " ns" << endl;
		//cout << " #### curTime: " << m_dLastReqHandle << " ms" << endl;
		
	}
	m_dLastReqHandle = m_dLastReqIssue + (dLat/_MILISEC_);
}

void
cSsdHostMachine::printStat(){

	/// adr trans scheme

#if(__ADR_TRANS_SCHEME__==_BASELINE_PAGE_MAPPING_)
	m_temp << "baseline\t";
#elif(__ADR_TRANS_SCHEME__==_DFTL_)
	m_temp << "dftl\t";
#elif(__ADR_TRANS_SCHEME__==_TWO_LEVEL_)
	m_temp << "twoLevel\t";
#elif(__ADR_TRANS_SCHEME__==_DIST_FTL_)
	m_temp << "proposed";
	m_temp << _RD_WR_PTR_CNT_ << "\t";
#endif

#if(__ADR_TRANS_SCHEME__==_DFTL_)
	/// CMT_SIZE
	m_temp << _CMT_SIZE_ / 512 << "\t";
#else
	/// CMT_SIZE
	m_temp << _CMT_SIZE_ / 1024 << "\t";
#endif

	/// trace
	
	std::string trName(m_wlName->c_str());

	while (trName.find('/') != std::string::npos){
		trName = trName.substr(trName.find('/') + 1);
	};

	m_temp << trName.c_str() << "\t";

	/// response time

	m_temp << m_stat.dRdRespTime / m_stat.iTotRdSec << "\t";
	m_temp << m_stat.dWrRespTime / m_stat.iTotWrSec << "\t";

	/// sector latency

	m_temp << m_stat.dRdSecLat / m_stat.iTotRdCnt << "\t";
	m_temp << m_stat.dWrSecLat / m_stat.iTotWrCnt << "\t";

	m_temp << getAvgEraseCnt() << "\t";
	m_temp << getStdDevEraseCnt(getAvgEraseCnt()) << "\t";
	m_temp << (double)m_stat.iMapFetchCnt / m_stat.iMapAccCnt << "\t";
	m_temp << m_stat.dCachedTgCnt / m_stat.iTotReqCnt << "\t";
	//m_temp << getMapMissRatio() << "\t";


	m_opAnalyzer->fileCmdCnt(m_temp);
	//m_opAnalyzer->printCmdCnt();

	
	m_temp << endl;
	m_temp.close();

}


