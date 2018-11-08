#ifndef __DEIVCE_IF_H__
#define __DEIVCE_IF_H__
#include "common_header.h"
#include "sfr_addr.h"


///////// Device interface

extern Req_t deviceIFDATA[32];
////////////

void initDeviceIF(void);

void callDeviceIFSched(uint bufferSize);
int CheckOverlap(uint buffersize, Req_t* temp, int offset);

#endif
