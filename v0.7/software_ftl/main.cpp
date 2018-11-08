
#include "sim_header.h"
#include "host_machine.h"
#include "nand_timing.h"

extern "C"{
#include "ftl.h"
#include "sim_if.h"
}

using namespace std;

cSsdHostMachine *g_host;
COpAnalyzer *g_nandTiming;

double g_dCurLat;
list<sNandCmd> g_ftlCmd;

void swSim_FtlCmd(sNandCmd cmd){
	g_ftlCmd.push_back(cmd);
}

void clearNandCmds()
{
	if (g_ftlCmd.size() <= 0)
		return;

	//g_dCurLat += g_nandTiming->computeTime(g_ftlCmd);
	list<sNandCmd>::iterator iter;
	for (iter = g_ftlCmd.begin(); iter != g_ftlCmd.end();)
	{
		g_ftlCmd.erase(iter++);
	}
	g_ftlCmd.clear();

}

void swSim_waitNand()
{
	if(g_ftlCmd.size() <= 0)
		return;

	g_dCurLat += g_nandTiming->computeTime(g_ftlCmd);

	list<sNandCmd>::iterator iter;
	for(iter = g_ftlCmd.begin() ; iter != g_ftlCmd.end() ; )
	{
		g_ftlCmd.erase(iter++);
	}
	g_ftlCmd.clear();
}

sNandSpec swSim_getNandSpec(){
	return g_nandTiming->getNandSpec();
}

unsigned long long int swSim_getReqCnt(){
	return g_host->getFetchedReqCnt();
}

DiskReq swSim_getCurReq(){
	return g_host->getCurReq();
}

void swSim_incMapAccCnt(){
	g_host->mapAccCnt();
}

void swSim_incMapFetchCnt(){
	g_host->mapFetchCnt();
}


int main(int argc, char* argv[])
{

	DiskReq curReq;
	// added for background operations
	double dIdle = 0;
	uint iBgcCnt = 0;
	bool isReadAppeared = false;

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//////// quick sort varification

	/*sListPtr testList = listConstruct(0);

	for (int i = 0; i < 2000; i++){
		listPushTail(testList, rand(), NULL);
	}
	
	listQuickSort(testList, 0, testList->iCurSize-1);

	for (int i = 1; i < testList->iCurSize; i++){
		if (listGetNthEle(testList, i - 1)->iKey > listGetNthEle(testList, i)->iKey){
			cout << "list sorting error!!" << endl;
			getchar();
		}
	}*/
	
	g_host = new cSsdHostMachine(argv[1]);
	
	//g_host = new cSsdHostMachine("web2_modified.txt");
	g_nandTiming = new COpAnalyzer();
	g_host->setOpAnalyzer(g_nandTiming);

	initFtl();

	do{

		uint *tempdd;
		eBlockStat *tempblkdd;

		curReq = g_host->getNextReq();
		g_dCurLat = 0;

		// background operation
		///////// compute idle period
		dIdle = curReq.dIssueTime - g_host->getLastReqHandle();

		///////// wait time-out threshold
		dIdle = (dIdle - BGC_TO_THRESHOLD_MS) > 0 ? (dIdle - BGC_TO_THRESHOLD_MS) : 0;
		iBgcCnt = 0;

		///////// if longer than TO, do some background gc.
		//while((dIdle - g_dCurLat) > 20000000){
		while (false){
		//while ((dIdle - g_dCurLat) > 0){
			if (isBgcReq())
			{
				doBgc();
				waitNand();
			}
			else{
				break;
			}
		}

		g_dCurLat = (dIdle - g_dCurLat) < 0 ? (-((dIdle - g_dCurLat))) : 0;

		//tempblkdd = &(ftlMeta[1][3][0].blocks[12].status);
		tempdd = &(ftlMeta[1][3][0]->blocks[12].iTopWritablePage);
		//ftlMeta[1][3][0].blocks[12].status = ftlMeta[1][3][0].blocks[12].status;

		if (g_host->getFetchedReqCnt() == 4){
 			debugFlag = UCHAR_TRUE;
		}

		//if (g_host->getFetchedReqCnt() == 300000){
		//	_CrtDumpMemoryLeaks();
		//	//exit(1);
		//}

		//////////////////////////// foreground operation

		if (curReq.cmd == DISK_CMD_READ && isReadAppeared == false){
			g_host->initStat();
			flushMapCache();
			clearNandCmds();
			isReadAppeared = true;
		}

		/*if ((g_host->getFetchedReqCnt() % 10000) == 0){
			cout << " / " << g_dClstSize << "/" << g_iClstCnt << " / ";
		}*/	



		callFtl(curReq);
		swSim_waitNand();		

		g_host->setLatency(g_dCurLat);
		g_host->setCurTgCnt(getCurCmtCnt());

	}while(1);

	return 0;
}


