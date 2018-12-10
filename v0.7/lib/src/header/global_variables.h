#ifndef __GLOBAL_VARIABLES_H__
#define __GLOBAL_VARIABLES_H__





bool        g_isFtlReady = false;
bool        g_isCacheReady = false;
bool        g_initialize_end = false;

unsigned int buffer_write_count = 0;
unsigned int meta_write_count;
unsigned int NAND_write_count;
unsigned int cache_bypass_count = 0;
unsigned int total_req_count = 0;

#endif
