#ifndef __COMPILE_FLAG_H__
#define __COMPILE_FLAG_H__

#define _SOFTWARE_SIM_						0
#if(_SOFTWARE_SIM_ == 0)

#define	_MCSSD_ARM_BARE_METAL_
#define _DEBUG_PRINT_LV_					1

#else

#define _DEBUG_PRINT_LV_					1

#endif
#endif
