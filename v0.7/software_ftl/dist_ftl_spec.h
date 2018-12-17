#ifndef __DIST_FTL_SPEC_H__
#define __DIST_FTL_SPEC_H__

#include "ftl_spec.h"

#if (__ADR_TRANS_SCHEME__==_DIST_FTL_)
#define _TRANS_CACHE_SIZE_		(16)
#elif (__ADR_TRANS_SCHEME__==_TWO_LEVEL_)
#define _TRANS_CACHE_SIZE_		(1)
#else
#define _TRANS_CACHE_SIZE_		(16)
#endif

#define _TRC_SIZE_				(_TRANS_CACHE_SIZE_)

#define _RD_WR_PTR_CNT_			(4)
#define _SEP_SEQ_BLK_			(1)

#if (_SEP_SEQ_BLK_ == 1)
#define _SEQ_TH_				(32)
#else
#define _SEQ_TH_				(99999)
#endif

#define _SORTING_GC_			0


//by skhds
//
#define _FULL_PAGE_             0xffffffff


#endif
