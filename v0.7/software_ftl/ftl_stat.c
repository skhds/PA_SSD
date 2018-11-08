#include "ftl_stat.h"

unsigned long long int g_iMapAccCnt = 0;
unsigned long long int g_iMapFetchCnt = 0;
uint (*getCurCmtCnt)(void);

double getAvgEraseCnt(){

	int ch, wy, pl, bk;
	double dErSum = 0;
	uint iNonFreshBlkCnt = 0;

	for (ch = 0; ch < NAND_ARRAY_CHANNEL; ch++)
	{
		for (wy = 0; wy < NAND_ARRAY_WAY; wy++)
		{
			for (pl = 0; pl < NAND_PLANE_PER_WAY; pl++)
			{
				for (bk = 0; bk < NAND_BLOCK_PER_PLANE; bk++)
				{
					dErSum += getEraseCnt(&(ftlMeta[ch][wy][pl]->blocks[bk]));

					if (getEraseCnt(&(ftlMeta[ch][wy][pl]->blocks[bk])) != 0){
						iNonFreshBlkCnt++;
					}
				}
			}
		}
	}

	return (dErSum / (iNonFreshBlkCnt));
}

double getStdDevEraseCnt(double avg){
	int ch, wy, pl, bk;
	double dSum = 0;
	uint iNonFreshBlkCnt = 0;

	for (ch = 0; ch < NAND_ARRAY_CHANNEL; ch++)
	{
		for (wy = 0; wy < NAND_ARRAY_WAY; wy++)
		{
			for (pl = 0; pl < NAND_PLANE_PER_WAY; pl++)
			{
				for (bk = 0; bk < NAND_BLOCK_PER_PLANE; bk++)
				{
					dSum += ((getEraseCnt(&(ftlMeta[ch][wy][pl]->blocks[bk])) - avg) *
						(getEraseCnt(&(ftlMeta[ch][wy][pl]->blocks[bk])) - avg));

					if (getEraseCnt(&(ftlMeta[ch][wy][pl]->blocks[bk])) != 0){
						iNonFreshBlkCnt++;
					}
				}
			}
		}
	}
	return sqrt(dSum / (iNonFreshBlkCnt));
}

void incMapAccCnt(){
#if(_SOFTWARE_SIM_==1)
	swSim_incMapAccCnt();	
#endif
}
void incMapFetchCnt(){
#if(_SOFTWARE_SIM_==1)
	swSim_incMapFetchCnt();
#endif
}
