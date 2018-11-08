#ifndef __FTL_STAT_H__
#define __FTL_STAT_H__
#include <math.h>
#include "common_header.h"
#include "sim_if.h"

extern unsigned long long int g_iMapAccCnt;
extern unsigned long long int g_iMapFetchCnt;

double getAvgEraseCnt();
double getStdDevEraseCnt(double avg);

void incMapAccCnt();
void incMapFetchCnt();

extern uint (*getCurCmtCnt)(void);

#endif
