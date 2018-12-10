#ifndef __GLOBAL_HEADER_H__
#define __GLOBAL_HEADER_H__

#include "configurables.h"
#include "global_structs.h"
#include "global_defines.h"
#include "global_variables.h"

#ifdef TRACK_ID_ON
#include "utils/trackByID.h"
#endif

#ifdef DATA_COMPARE_ON
#include "utils/data_compare.h"
#endif


#ifdef TRACK_ID_ON
SSDmetric::trackByID testTrack("../results/test_result.txt");//skhds
#endif







#endif
