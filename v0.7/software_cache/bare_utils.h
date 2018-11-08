#ifndef __BAREUTILS_H__
#define __BAREUTILS_H__

#include <stdarg.h>
#include "sfr_addr.h"

///////// PRINT
#define _ADDR_PRINT_CHAR_				0x10000000
#define _ADDR_PRINT_INT_				0x10000004
#define _ADDR_PRINT_FLOAT_				0x10000008
// added by BryanKim 171208 
#define _ADDR_PRINT_HEX_                0x1000000C
#define _ADDR_PRINT_INT64_MSB_          0x10000010
#define _ADDR_PRINT_INT64_LSB_          0x10000014
#define _ADDR_PRINT_DOUBLE_MSB_         0x10000018
#define _ADDR_PRINT_DOUBLE_LSB_         0x1000001C
#define _ADDR_PRINT_INT64HEX_MSB_       0x10000020
#define _ADDR_PRINT_INT64HEX_LSB_       0x10000024
//Available memory address is 0xf8000000 ~ 0xf80000cf

/* deleted by BryanKim 171208
#define _ADDR_SW_ROUTINE_START_			0xf8000008
#define _ADDR_SW_ROUTINE_END_			0xf800000c
*/

///////// HEAP

#define _ADDR_HEAP_BASE_                0x300000
#define _ADDR_HEAP_LIMIT_               0x7000000
#define _ADDR_HEAP_SIZE_                (_ADDR_HEAP_LIMIT_ - _ADDR_HEAP_BASE_)


////////CASTING////////
#define UINT64(a) UINT64_(a,1),UINT64_(a,0)
#define DOUBLE(a) DOUBLE_(a,1),DOUBLE_(a,0)
unsigned int FLOAT(float f);
unsigned int UINT64_(unsigned long long int uint64, unsigned int b);
unsigned int DOUBLE_(double d, unsigned int b);

typedef enum eSoftwareRoutineId{

	SW_RT_FTL = 0,
	SW_RT_ADDR_TRANS,
	SW_RT_FORE_GC,
	SW_RT_DYN_WL

}eSwRtId;

typedef struct __heap_extent
{
	unsigned base, range;
}sHeap;

void			barePrintf(const char* sentence, ...);
void			bareSetStart(unsigned int iId);

#endif

