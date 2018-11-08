#ifndef __FTL_META_H__
#define __FTL_META_H__

#include <assert.h>
#include "extern_ftl_global_var.h"
#include "hw_if.h"

sPageMeta *getPgPtr(uint ch, uint wy, uint pl, uint bk, uint pg);
sBlockMeta *getBkPtr(uint ch, uint wy, uint pl, uint bk);
sFtlMeta *getFtlPtr(uint ch, uint wy, uint pl);

void nandProgram(sNandCmd cmd);
void nandRead(sNandCmd cmd);
void nandCopyback(sNandCmd cmd);
void nandErase(sNandCmd cmd);
void nandPageInvalidate(sNandAddr adr);



#endif